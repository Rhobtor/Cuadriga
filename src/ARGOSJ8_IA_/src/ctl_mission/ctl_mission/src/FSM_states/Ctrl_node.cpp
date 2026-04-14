/**
 * @file Ctrl_node.cpp
 * @brief Lifecycle state node that turns a local trajectory into velocity commands (cmd_vel).
 *
 * ## Role in the J8 mission FSM
 * `CtrlNode` is the "controller" state node. It consumes a local path/trajectory (typically
 * produced by `PathFollowingNode` or other planners) and produces `geometry_msgs/Twist` commands
 * for the platform.
 *
 * The mission/orchestrator (`CtlMissionNode`) activates/deactivates this node as part of the
 * global FSM. When inactive, lifecycle publishers are deactivated, so this node should not
 * command the vehicle.
 *
 * ## Controllers supported
 * The controller algorithm is selected at runtime via the `ChangeController` service.
 * Current expected values (see `available_controller_types_`):
 * - `pure_pursuit`
 * - `regulated_pure_pursuit`
 * - `dynamic_pure_pursuit`
 * - `dynamic_la_pure_pursuit`
 *
 * Each controller has its own configuration service (see ROS contract below).
 *
 * ## ROS contract (topics / services / params)
 * ### Subscriptions
 * - `local_trajectory_topic` (default: `local_trajectory`) [nav_msgs/msg/Path]
 * - distance-to-goal input (topic name is part of the broader stack; see `getDistLastObj()`)
 * - odometry input (used by dynamic look-ahead version)
 *
 * ### Publications
 * - `cmd_vel_topic_name` (default: `cmd_vel_test`) [geometry_msgs/msg/Twist]
 * - `pub_local_path` [nav_msgs/msg/Path] local path transformed into `robot_frame`
 * - `pub_cte` [std_msgs/msg/Float32] cross-track error value
 * - `pub_look_ahead`, `pub_look_ahead_distance`, `pub_la_pose` (debug/telemetry)
 *
 * ### Services (servers)
 * - `change_controller_srv_name` (default: `change_controller_type`) [ctl_mission_interfaces/srv/ChangeController]
 * - `pure_pursuit.config_pure_pursuit_ctrl_srv_name` (default: `config_pure_pursuit`) [ConfigPurePursuitCtrl]
 * - `regulated_pure_pursuit.config_regulated_pure_ctrl_srv_name` (default: `config_regulated_pure`) [ConfigRegulatedPureCtrl]
 * - `dynamic_pp.config_dynamic_pure_ctrl_srv_name` (default: `config_dynamic_pure`) [ConfigDynamicPureCtrl]
 * - `dynamic_la_pp.config_dynamic_la_pure_ctrl_srv_name` (default: `config_dynamic_la_pure`) [ConfigDynamicLAPureCtrl]
 *
 * ### Key parameters
 * - Frames: `robot_frame` (default: `FP_POI`), `local_fixed_frame` (default: `FP_ENU0`)
 * - `controller_frequency` (default: 50 Hz)
 * - Controller-specific params under namespaces like `pure_pursuit.*`, `dynamic_pp.*`, etc.
 *
 * ## Coordinate frames / TF
 * Incoming path is assumed to be in `fixed_local_frame`. It is transformed into `robot_frame`
 * before computing tracking errors / commands.
 */

#include "ctl_mission/CtrlNode.hpp"

CtrlNode::CtrlNode(const std::string & node_name, bool intra_process_comms)
: rclcpp_lifecycle::LifecycleNode(node_name, rclcpp::NodeOptions().use_intra_process_comms(intra_process_comms)) {
    // Declare parameters with namespace
    this->declare_parameter("local_trajectory_topic", "local_trajectory");
    this->declare_parameter("cmd_vel_topic_name", "cmd_vel_test");
    this->declare_parameter("robot_frame", "FP_POI");
    this->declare_parameter("local_fixed_frame", "FP_ENU0");
    this->declare_parameter("type", "pure_pursuit");
    this->declare_parameter("controller_frequency", 50.0);
    this->declare_parameter("min_speed", 0.5);
    this->declare_parameter("breaking_acc", 1.0);
    this->declare_parameter("regulated_pure_pursuit.l_ahead", 2.0);
    this->declare_parameter("regulated_pure_pursuit.v_forward", 1.0);
    this->declare_parameter("regulated_pure_pursuit.r_min", 3.0);
    this->declare_parameter("pure_pursuit.K_ang_err", 0.2);
    this->declare_parameter("pure_pursuit.K_linear_err", 1.0);
    this->declare_parameter("pure_pursuit.v_forward", 1.0);
    this->declare_parameter("pure_pursuit.l_ahead", 3.0);
    this->declare_parameter("pure_pursuit.point_window_size", 40);
    this->declare_parameter("dynamic_pp.look_ahead_distance", 2.0);
    this->declare_parameter("dynamic_pp.max_speed", 3.0);
    this->declare_parameter("dynamic_pp.max_ang_acc", 0.4);
    this->declare_parameter("dynamic_pp.max_ang_dec", 2.0);
    this->declare_parameter("dynamic_pp.max_lin_acc", 0.52);
    this->declare_parameter("dynamic_pp.max_lin_dec", 2.0);
    this->declare_parameter("dynamic_la_pp.look_ahead_v_gain", 2.0);
    this->declare_parameter("dynamic_la_pp.max_speed", 3.0);
    this->declare_parameter("dynamic_la_pp.max_ang_acc", 0.4);
    this->declare_parameter("dynamic_la_pp.max_ang_dec", 2.0);
    this->declare_parameter("dynamic_la_pp.max_lin_acc", 0.52);
    this->declare_parameter("dynamic_la_pp.max_lin_dec", 2.0);
    this->declare_parameter("dynamic_la_pp.min_speed", 0.5);
    this->declare_parameter("services.config_simple_ctl_srv_name", "config_controller");
    this->declare_parameter("change_controller_srv_name", "change_controller_type");
    this->declare_parameter("pure_pursuit.config_pure_pursuit_ctrl_srv_name", "config_pure_pursuit");
    this->declare_parameter("dynamic_pp.config_dynamic_pure_ctrl_srv_name", "config_dynamic_pure");
    this->declare_parameter("dynamic_la_pp.config_dynamic_la_pure_ctrl_srv_name", "config_dynamic_la_pure");
    this->declare_parameter("regulated_pure_pursuit.config_regulated_pure_ctrl_srv_name", "config_regulated_pure");

    // Initialize other variables or perform additional setup here
}

nav_msgs::msg::Path CtrlNode::transform_path_to_robot_frame(const nav_msgs::msg::Path& path_in) {
    nav_msgs::msg::Path path_out;

    // Set the header for the output path
    path_out.header.frame_id = robot_frame;
    path_out.header.stamp = this->get_clock()->now();

    for(const auto& pose_in : path_in.poses) {
        geometry_msgs::msg::PoseStamped pose_out;

        geometry_msgs::msg::TransformStamped t;
        if (tf_buffer_->canTransform(robot_frame, fixed_local_frame, tf2::TimePointZero)) {
            try {
                t = tf_buffer_->lookupTransform(robot_frame, fixed_local_frame, tf2::TimePointZero);
            } catch (const tf2::TransformException & ex) {
                RCLCPP_INFO(this->get_logger(), "Transform failed: %s", ex.what());
                continue; // Skip this iteration
            }

            tf2::doTransform(pose_in, pose_out, t);
            path_out.poses.push_back(pose_out);
        } else {
            RCLCPP_INFO(this->get_logger(), "Waiting for transform from %s to %s", fixed_local_frame.c_str(), robot_frame.c_str());
            // Optionally wait or handle the absence of the transform
        }


    }

    return path_out;
}

void CtrlNode::getTrajectory(const nav_msgs::msg::Path::SharedPtr msg) {
    got_path = true;
    path_msg = *msg;
    // Update to use the timestamp from the message header
    path_msg_timestamp = msg->header.stamp;
}

void CtrlNode::getDistLastObj(const std_msgs::msg::Float32::SharedPtr msg) {
    dist_last_obj = msg->data;
}

void CtrlNode::getOdom(const nav_msgs::msg::Odometry::SharedPtr msg) {
    v_x_odom = msg->twist.twist.linear.x;
    w_z_odom = msg->twist.twist.angular.z;

}


void CtrlNode::publish() {

        if (!got_path) {
            std::cerr << "No path received yet." << std::endl;
            return;
        }

        auto now = this->get_clock()->now();
        // Calculate the age of the message using the header timestamp
        rclcpp::Duration msg_age = now - path_msg_timestamp;
        const rclcpp::Duration threshold(std::chrono::milliseconds(500));

        if (msg_age > threshold) {
            //RCLCPP_ERROR(this->get_logger(), "Path message is too old: %ld seconds old.", msg_age.seconds());
            //return;
        }

        auto path_transformed = transform_path_to_robot_frame(path_msg);
        std::cerr << "path size t." << path_msg.poses.size()<<std::endl;

        // Calculate the cross-track error in the robot frame
        double cte = calculateCrossTrackErrorInRobotFrame(path_transformed);
        std_msgs::msg::Float32 cte_msg;
        cte_msg.data = cte;
        pub_cte->publish(cte_msg);
        pub_local_path->publish(path_transformed);
        if (current_controler == "pure_pursuit"){
            std::cout<<"pure"<<std::endl;
            pure_pursuit_controller.calc_pure_pursuit_actions(path_transformed, dist_last_obj);
            cmd_msg.linear.x = pure_pursuit_controller.pure_pursuit_output.linear_sp;
            cmd_msg.angular.z = -pure_pursuit_controller.pure_pursuit_output.angular_sp;
            std_msgs::msg::Int32 index_msg ;
            index_msg.data = pure_pursuit_controller.pure_pursuit_output.goal_idx;
            pub_look_ahead->publish(index_msg);
            std_msgs::msg::Float32 distance_msg;
            distance_msg.data = pure_pursuit_controller.pure_pursuit_output.cte;
            pub_look_ahead_distance->publish(distance_msg);
            
        }

        if (current_controler == "regulated_pure_pursuit"){
            std::cout<<"regulated_pure_pursuit"<<std::endl;
            regulated_pure_pursuit_controller.calc_regulated_pure_pursuit_actions(path_transformed, dist_last_obj);
            cmd_msg.linear.x = regulated_pure_pursuit_controller.regulated_pure_pursuit_output.linear_sp;
            cmd_msg.angular.z = -regulated_pure_pursuit_controller.regulated_pure_pursuit_output.angular_sp;
            std_msgs::msg::Float32 distance_msg;
            distance_msg.data = regulated_pure_pursuit_controller.regulated_pure_pursuit_output.cte;
            pub_look_ahead_distance->publish(distance_msg);

        }
        if (current_controler == "dynamic_pure_pursuit"){
            std::cout<<"dynamic_pure_pursuit"<<std::endl;
            dynamic_pp_controller.calc_dynamic_pp_actions(path_transformed);
            cmd_msg.linear.x = dynamic_pp_controller.dynamic_pp_output.linear_sp;
            cmd_msg.angular.z = dynamic_pp_controller.dynamic_pp_output.angular_sp;
            std_msgs::msg::Int32 index_msg ;
            index_msg.data = dynamic_pp_controller.dynamic_pp_output.goal_idx;
            pub_look_ahead->publish(index_msg);

        }

        if (current_controler == "dynamic_la_pure_pursuit"){
            dynamic_la_pp_controller.calc_dynamic_lapp_actions(path_transformed, dist_last_obj,  v_x_odom,  w_z_odom);
            //std::cout<<"dynamic_la_pure_pursuit"<<std::endl;
            cmd_msg.linear.x = dynamic_la_pp_controller.dynamic_lapp_output.linear_sp;
            cmd_msg.angular.z = dynamic_la_pp_controller.dynamic_lapp_output.angular_sp;
            std_msgs::msg::Int32 index_msg ;
            index_msg.data = dynamic_la_pp_controller.dynamic_lapp_output.goal_idx;
            pub_look_ahead->publish(index_msg);
            std_msgs::msg::Float32 distance_msg;
            distance_msg.data = dynamic_la_pp_controller.dynamic_lapp_output.cte;
            pub_look_ahead_distance->publish(distance_msg);
            pub_la_pose->publish(dynamic_la_pp_controller.dynamic_lapp_output.look_ahead_pose);
        }
        
        pub_cmd_vel->publish(cmd_msg);
    }





    void CtrlNode::changeControllerType(
        const std::shared_ptr<ctl_mission_interfaces::srv::ChangeController::Request> request,
        std::shared_ptr<ctl_mission_interfaces::srv::ChangeController::Response> response)
    {
       // Check if the provided controller type is among the available ones
        if (std::find(available_controller_types_.begin(), available_controller_types_.end(), request->controller_type) !=
            available_controller_types_.end())
        {
            current_controler = request->controller_type;
            response->success = true;
            RCLCPP_INFO(this->get_logger(), "controller Changed ");

        }
        else
        {
            response->success = false;
            RCLCPP_INFO(this->get_logger(), "controller name invalid ");

        }
    }

void CtrlNode::configPurePursuit(
        const std::shared_ptr<ctl_mission_interfaces::srv::ConfigPurePursuitCtrl::Request> request,
        std::shared_ptr<ctl_mission_interfaces::srv::ConfigPurePursuitCtrl::Response> response)
    {
        RCLCPP_INFO(this->get_logger(), "Pure pursuit controller config");
        pure_pursuit_controller.control_config(request->v_forward, request->l_ahead_dist);

    }

void CtrlNode::configRegulatedPure(
        const std::shared_ptr<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl::Request> request,
        std::shared_ptr<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl::Response> response)
    {
        RCLCPP_INFO(this->get_logger(), "Regulated Pure config");
        regulated_pure_pursuit_controller.control_config(request->look_ahead_dis, request->v_forward, request->r_min);

    }

void CtrlNode::configDynamic(
        const std::shared_ptr<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl::Request> request,
        std::shared_ptr<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl::Response> response)
    {
        RCLCPP_INFO(this->get_logger(), "Dynamic pure controller config");
        dynamic_pp_controller.params_init(request->look_ahead_dis,  request->max_v_forward,  request->max_ang_acc,  request->max_ang_dec, 
                                    request->max_lin_acc,  request->max_lin_dec); 
    }

void CtrlNode::configDynamicLA(
        const std::shared_ptr<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl::Request> request,
        std::shared_ptr<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl::Response> response)
    {
        RCLCPP_INFO(this->get_logger(), "Dynamic LA pure controller config");
        dynamic_la_pp_controller.params_init(request->look_ahead_v_gain,  request->max_v_forward,  request->max_ang_acc,  request->max_ang_dec, 
                                    request->max_lin_acc,  request->max_lin_dec,  request->speed_pow, request->min_look_ahead_d); 
    }

// Add this method to your CtrlNode class
double CtrlNode::calculateCrossTrackErrorInRobotFrame(const nav_msgs::msg::Path& path) {
    double min_distance = std::numeric_limits<double>::max();
    double cte = 0.0;

    // The robot is at (0, 0) in its own frame, so we only need to find the closest point on the path
    for (const auto& pose_stamped : path.poses) {
        const auto& path_pose = pose_stamped.pose;

        // Calculate the Euclidean distance between the robot (0, 0) and the path position
        double distance = std::hypot(path_pose.position.x, path_pose.position.y);

        // Find the minimum distance and the corresponding CTE
        if (distance < min_distance) {
            min_distance = distance;
            cte = path_pose.position.y; // CTE is the y-coordinate in the robot frame
        }
    }

    return cte;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn CtrlNode::on_configure(const rclcpp_lifecycle::State &) {
    // This callback is supposed to be used for initialization and
    // configuring purposes.
    RCLCPP_INFO(get_logger(), "on_configure() is called.");
    tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
    this->get_parameter("local_trajectory_topic", trajectory_topic_name);
    this->get_parameter("cmd_vel_topic_name", cmd_vel_name);
    this->get_parameter("robot_frame", robot_frame);
    this->get_parameter("local_fixed_frame", fixed_local_frame);
    this->get_parameter("regulated_pure_pursuit.l_ahead", regulated_pure_pursuit_l_ahead);
    this->get_parameter("regulated_pure_pursuit.v_forward", regulated_pure_pursuit_v_forward);
    this->get_parameter("regulated_pure_pursuit.r_min", regulated_pure_pursuit_r_min);
    this->get_parameter("pure_pursuit.v_forward", pp_linear_speed);
    this->get_parameter("pure_pursuit.l_ahead", pp_look_ahead_dist);
    this->get_parameter("dynamic_pp.look_ahead_distance", dyn_pp_look_ahead_distance);
    this->get_parameter("dynamic_pp.max_speed", dyn_pp_max_speed);
    this->get_parameter("dynamic_pp.max_ang_acc", dyn_pp_max_ang_acc);
    this->get_parameter("dynamic_pp.max_ang_dec", dyn_pp_max_ang_dec);
    this->get_parameter("dynamic_pp.max_lin_acc", dyn_pp_max_lin_acc);
    this->get_parameter("dynamic_pp.max_lin_dec", dyn_pp_max_lin_dec);
    this->get_parameter("controller_frequency", controller_frequency);
    this->get_parameter("dynamic_la_pp.look_ahead_v_gain", dyn_la_pp_look_ahead_v_gain);
    this->get_parameter("dynamic_la_pp.max_speed", dyn_la_pp_max_speed);
    this->get_parameter("dynamic_la_pp.max_ang_acc", dyn_la_pp_max_ang_acc);
    this->get_parameter("dynamic_la_pp.max_ang_dec", dyn_la_pp_max_ang_dec);
    this->get_parameter("dynamic_la_pp.max_lin_acc", dyn_la_pp_max_lin_acc);
    this->get_parameter("dynamic_la_pp.max_lin_dec", dyn_la_pp_max_lin_dec);
    this->get_parameter("dynamic_la_pp.min_speed", dyn_la_pp_min_speed);
    this->get_parameter("min_speed", min_speed);
    this->get_parameter("breaking_acc", breaking_acc);
    this->get_parameter("change_controller_srv_name", change_ctrl_srv_name);
    this->get_parameter("pure_pursuit.config_pure_pursuit_ctrl_srv_name", config_pp_srv_name);
    this->get_parameter("dynamic_pp.config_dynamic_pure_ctrl_srv_name", config_dynamic_pp_srv_name);
    this->get_parameter("dynamic_la_pp.config_dynamic_la_pure_ctrl_srv_name",config_dynamic_la_pp_srv_name);
    this->get_parameter("regulated_pure_pursuit.config_regulated_pure_ctrl_srv_name", config_regulated_srv_name);

    trajectoryToFollow = this->create_subscription<nav_msgs::msg::Path>
    (trajectory_topic_name,10,std::bind(&CtrlNode::getTrajectory, this,std::placeholders::_1));
    dist_last_obj_sub = this->create_subscription<std_msgs::msg::Float32>
    ("dist_last_obj",10,std::bind(&CtrlNode::getDistLastObj, this,std::placeholders::_1));
    pub_look_ahead_distance  = this->create_publisher<std_msgs::msg::Float32>("look_ahead_distance", 10);
        odometry_sub = this->create_subscription<nav_msgs::msg::Odometry>
    ("/fixposition/odometry",10,std::bind(&CtrlNode::getOdom, this,std::placeholders::_1));
    current_controler = this->get_parameter("type").get_parameter_value().get<std::string>();
    pub_cmd_vel = this->create_publisher<geometry_msgs::msg::Twist>(cmd_vel_name, 10);
    pub_look_ahead = this->create_publisher<std_msgs::msg::Int32>("look_ahead_point", 10);
    pub_cte = this->create_publisher<std_msgs::msg::Float32>("cte", 10);
    pub_la_distance = this->create_publisher<std_msgs::msg::Float32>("la_distance", 10);
    pub_la_pose = this->create_publisher<geometry_msgs::msg::PoseStamped>("pose_stamped", 10);

    changeControllersrv = this->create_service<ctl_mission_interfaces::srv::ChangeController>(
            change_ctrl_srv_name,
            std::bind(&CtrlNode::changeControllerType, this, std::placeholders::_1, std::placeholders::_2));
   configPurePursuitCtrlsrv = this->create_service<ctl_mission_interfaces::srv::ConfigPurePursuitCtrl>(
            config_pp_srv_name,
            std::bind(&CtrlNode::configPurePursuit, this, std::placeholders::_1, std::placeholders::_2));
    configDynamicPuresrv = this->create_service<ctl_mission_interfaces::srv::ConfigDynamicPureCtrl>(
            config_dynamic_pp_srv_name,
            std::bind(&CtrlNode::configDynamic, this, std::placeholders::_1, std::placeholders::_2));
    configDynamicLAPuresrv = this->create_service<ctl_mission_interfaces::srv::ConfigDynamicLAPureCtrl>(
            config_dynamic_la_pp_srv_name,
            std::bind(&CtrlNode::configDynamicLA, this, std::placeholders::_1, std::placeholders::_2));        
    configPureRegulatedCtrlsrv =  this->create_service<ctl_mission_interfaces::srv::ConfigRegulatedPureCtrl>(
            config_regulated_srv_name,
            std::bind(&CtrlNode::configRegulatedPure, this, std::placeholders::_1, std::placeholders::_2));    
    //start stanley controller
    double timer_period = (double) 1/controller_frequency;
    pub_local_path = this->create_publisher<nav_msgs::msg::Path>("ctrl_local_path", 10);

    regulated_pure_pursuit_controller.params_init(timer_period, regulated_pure_pursuit_l_ahead, regulated_pure_pursuit_v_forward, 
                                                  0.5, breaking_acc, regulated_pure_pursuit_r_min);
    //start pure pursuit controller
    pure_pursuit_controller.control_init(timer_period, pp_look_ahead_dist, pp_linear_speed, min_speed, breaking_acc);
    //start dynamic pp controller
    dynamic_pp_controller.control_init(dyn_pp_look_ahead_distance,  dyn_pp_max_speed,  dyn_pp_max_ang_acc,  dyn_pp_max_ang_dec, 
                                       dyn_pp_max_lin_acc,  dyn_pp_max_lin_dec, timer_period, min_speed); 
    ///start dynamic_la_pp_ controller;
    dynamic_la_pp_controller.control_init(dyn_la_pp_look_ahead_v_gain,  dyn_la_pp_max_speed,  dyn_la_pp_max_ang_acc,  dyn_la_pp_max_ang_dec, 
                                          dyn_la_pp_max_lin_acc,  dyn_la_pp_max_lin_dec, timer_period, dyn_la_pp_min_speed); 
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn CtrlNode::on_activate(const rclcpp_lifecycle::State & state) {
    dist_last_obj = std::numeric_limits<float>::max();
    dynamic_la_pp_controller.controller_reinit();
    auto timer_period = std::chrono::duration<double>(1.0 / controller_frequency);
    timer_ = this->create_wall_timer(timer_period, [this]() {return this->publish();});
    LifecycleNode::on_activate(state);
    RCLCPP_INFO(get_logger(), "on_activate() is called.");
    // We return a success and hence invoke the transition to the next
    // step: "active".

    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn CtrlNode::on_deactivate(const rclcpp_lifecycle::State & state) {

    timer_.reset();
    LifecycleNode::on_deactivate(state);
    RCLCPP_INFO(get_logger(), "on_deactivate() is called.");
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn CtrlNode::on_cleanup(const rclcpp_lifecycle::State &) {

    timer_.reset();
    RCLCPP_INFO(get_logger(), "on cleanup is called.");
    // We return a success and hence invoke the transition to the next
    // step: "unconfigured".
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn CtrlNode::on_shutdown(const rclcpp_lifecycle::State & state) {

    timer_.reset();
    RCLCPP_INFO(get_logger(), "on shutdown is called from state %s.", state.label().c_str());
    // We return a success and hence invoke the transition to the next
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}
