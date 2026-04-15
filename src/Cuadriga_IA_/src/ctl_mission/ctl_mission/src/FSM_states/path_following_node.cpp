/**
 * @file path_following_node.cpp
 * @brief Lifecycle state node that computes / publishes a local path segment to follow.
 *
 * ## Role in the cuadriga mission FSM
 * This node represents the "PathFollowing" state/capability. The mission/orchestrator
 * activates it when the FSM enters the corresponding mode.
 *
 * The node is responsible for:
 * - Requesting a global path via `path_manager`.
 * - Transforming that path to the robot frame and local fixed frame using TF2.
 * - Publishing a truncated look-ahead segment ("planner path") so a controller can track it.
 * - Publishing diagnostic metrics like distance-to-goal and min distance to global path.
 *
 * When the remaining distance to the end of the path becomes small, it requests an FSM
 * mode change via the mission service client.
 *
 * ## ROS contract (topics / services / params)
 * ### Services (clients)
 * - `path_service_name` (default: `get_fix_frame_path`) [path_manager_interfaces/srv/GetFixFramePath]
 *      Used to fetch the current path to follow.
 * - `fsm.change_fsm_mode_srv_name` (default: `change_fsm_mode`) [ctl_mission_interfaces/srv/ChangeMode]
 *      Used to request a transition when the goal is reached.
 *
 * ### Subscriptions
 * - `/fixposition/odometry_enu` [nav_msgs/msg/Odometry]
 * - `joystick_topic_name` (default: `joy`) [sensor_msgs/msg/Joy]
 * - `gps_topic` (default: `/fixposition/navsatfix`) [sensor_msgs/msg/NavSatFix]
 *
 * ### Publications
 * - `cmd_vel_topic_name` (default: `cmd_vel_test`) [geometry_msgs/msg/Twist]
 * - `local_path` [nav_msgs/msg/Path] (segment in local fixed frame)
 * - `poi_path` [nav_msgs/msg/Path] (path in robot frame / POI frame)
 * - `ecef_path` [nav_msgs/msg/Path] (as received / assumed ECEF)
 * - `enu_path` [nav_msgs/msg/Path] (path in local ENU)
 * - `dist_last_obj` [std_msgs/msg/Float32] distance remaining to final waypoint
 * - `min_distance_to_path` [std_msgs/msg/Float32] min distance between current GPS fix and path
 *
 * ### Parameters
 * - Frames: `robot_frame`, `local_fixed_frame`, `global_fixed_frame`
 * - `target_distance` (m): length of the truncated path segment published.
 * - `node_frequency` (Hz): timer rate while active.
 * - Joystick: `joystick_topic_name`.
 *
 * ## Gotchas
 * - Parts of this implementation mix frames and reuse `nav_msgs/Path` to convey paths in
 *   different frames. Always inspect `header.frame_id` to interpret points.
 * - The code currently contains a Twist publisher that is not always used (publishing commented
 *   out in `joy_callback`). This is intentional in the current version.
 */

#include  "ctl_mission/PathFollowingNode.hpp"
PathFollowingNode::PathFollowingNode(const std::string &node_name, bool intra_process_comms)
: rclcpp_lifecycle::LifecycleNode(node_name, rclcpp::NodeOptions().use_intra_process_comms(intra_process_comms)) {
    // Constructor implementation
    this->declare_parameter("path_service_name", "get_fix_frame_path");
    this->declare_parameter("robot_frame", "FP_POI");
    this->declare_parameter("local_fixed_frame", "FP_ENU0");
    this->declare_parameter("global_fixed_frame", "ECEF");
    this->declare_parameter("path_params.look_points_ahead", 15);
    this->declare_parameter("cmd_vel_topic_name", "cmd_vel_test");
    this->declare_parameter("joystick_topic_name", "joy");
    this->declare_parameter("gps_topic", "/fixposition/navsatfix");
    this->declare_parameter("target_distance", 30.0);
    this->declare_parameter("node_frequency", 10.0);
    this->declare_parameter("fsm.change_fsm_mode_srv_name", "change_fsm_mode");

}
  void PathFollowingNode::timer_transform_cb() {
        if (got_path) {
            //RCLCPP_INFO(this->get_logger(), "Following path!");
            nav_msgs::msg::Path path_robot_frame = transform_path_to_robot_frame(robot_path, robot_frame);
            nav_msgs::msg::Path path_local_frame = transform_path_to_robot_frame(robot_path, fixed_local_frame);
            distances_to_wp(path_local_frame, odometry_msg);
            double accumulatedDistance = 0.0;
            int i = min_point_idx + 1; //We add 1, so the point is garanteed in front of the robot
            nav_msgs::msg::Path plannerpath;
            plannerpath.header.frame_id = fixed_local_frame;
            plannerpath.header.stamp = this->get_clock()->now();
            for (; i < path_robot_frame.poses.size() -1 && accumulatedDistance < target_distance; ++i) {
                accumulatedDistance += calculateDistance(path_robot_frame.poses[i], path_robot_frame.poses[i + 1]);
                plannerpath.poses.push_back(path_local_frame.poses[i]);
                if (accumulatedDistance >= target_distance) break;
            }
            nav_msgs::msg::Path wholepath;
            nav_msgs::msg::Path transformed_ecef;
            transformed_ecef = robot_path;

            wholepath = path_robot_frame;
            wholepath.header.frame_id = fixed_local_frame;
            wholepath.header.stamp = this->get_clock()->now();
            for (i = 0; i < path_robot_frame.poses.size() -1 ; ++i) {
                wholepath.poses.push_back(wholepath.poses[i]);
            }
            double lastGoalDistance = 0.0;
            for (int m = min_point_idx;m < path_robot_frame.poses.size() -1; ++m) {
                lastGoalDistance += calculateDistance(path_robot_frame.poses[m], path_robot_frame.poses[m + 1]);
            }
            std_msgs::msg::Float32 dist_last_goal_msg;
            dist_last_goal_msg.data = lastGoalDistance;
            pub_dist_last_obj->publish(dist_last_goal_msg);
            if (i == path_robot_frame.poses.size() - 1 && accumulatedDistance < target_distance) {
                plannerpath.poses.push_back(path_local_frame.poses[i]); // Add the last point if needed
            }

            if (lastGoalDistance < 1.0){
                sendChangeModeRequest(FSM_client, 1); // Change mode if necessary
            }
            pub_enu_path->publish(path_local_frame);
            pub_path->publish(plannerpath);
            pub_ecef_path->publish(transformed_ecef);
            pub_poi_path->publish(path_robot_frame);


        } else {
            RCLCPP_INFO(this->get_logger(), "No path to follow! Go to ready state and send a path!");
        }
    }

    


void PathFollowingNode::sendChangeModeRequest(
    const rclcpp::Client<ctl_mission_interfaces::srv::ChangeMode>::SharedPtr& client,
    int transition_id) 
{
    if (!client) {
        RCLCPP_ERROR(this->get_logger(), "ChangeMode client is not initialized");
        return;
    }

    auto request = std::make_shared<ctl_mission_interfaces::srv::ChangeMode::Request>();
    request->transition = transition_id;

    service_future_fsm = client->async_send_request(request);

    future_check_timer_fsm = this->create_wall_timer(
        300ms, std::bind(&PathFollowingNode::checkChangeModeFuture, this));
}


void PathFollowingNode::checkChangeModeFuture() {
    if (service_future_fsm.valid() && service_future_fsm.wait_for(0s) == std::future_status::ready) {
        auto response = service_future_fsm.get();
        // Process the response
        future_check_timer_fsm->cancel();
        // Handle the response from ChangeMode service
        if(response->success) {
            // Handle successful transition
        } else {
            // Handle failed transition
        }
    }
}


void PathFollowingNode::wait_path() {
    callGetFixFramePathService();
}

void PathFollowingNode::callGetFixFramePathService() {
      std::cout << "srv callback " << robot_path.poses.size() << std::endl;

    if (!getRPathClient->wait_for_service(20s)) {
        RCLCPP_ERROR(this->get_logger(), "Service not available after waiting");
        return;
    }

    auto request = std::make_shared<path_manager_interfaces::srv::GetFixFramePath::Request>();
    request->ask = true;
    service_future_ = getRPathClient->async_send_request(request);
    future_check_timer_ = this->create_wall_timer(100ms, std::bind(&PathFollowingNode::checkFuture, this));
}

void PathFollowingNode::checkFuture() {
    if (service_future_.valid() && service_future_.wait_for(0s) == std::future_status::ready) {
        auto response = service_future_.get();
        robot_path = response->path;

        if (robot_path.poses.size() > 0) {
            got_path = true;
            future_check_timer_->cancel();

            RCLCPP_INFO(this->get_logger(), "Received path");
        }
    }
}

void PathFollowingNode::fix_callback(const sensor_msgs::msg::NavSatFix::SharedPtr msg) {
    double ecef_x, ecef_y, ecef_z;
    latLonToECEF(msg->latitude, msg->longitude, msg->altitude, ecef_x, ecef_y, ecef_z);

    // Compute minimum distance to the path
    double min_distance = computeMinDistanceToPath(ecef_x, ecef_y, ecef_z);

    // Publish the minimum distance
    std_msgs::msg::Float32 min_distance_msg;
    min_distance_msg.data = min_distance;
    pub_min_distance_->publish(min_distance_msg);
}

double PathFollowingNode::computeMinDistanceToPath(double robot_ecef_x, double robot_ecef_y, double robot_ecef_z) {
    double min_distance = std::numeric_limits<double>::infinity();

    //RCLCPP_INFO(this->get_logger(), "Robot ECEF Position: x=%.6f, y=%.6f, z=%.6f", robot_ecef_x, robot_ecef_y, robot_ecef_z);

    for (const auto& pose : robot_path.poses) {
        double path_x = pose.pose.position.x;
        double path_y = pose.pose.position.y;
        double path_z = pose.pose.position.z;
        double distance = std::sqrt(std::pow(robot_ecef_x - path_x, 2) + std::pow(robot_ecef_y - path_y, 2) + std::pow(robot_ecef_z - path_z, 2));

        //RCLCPP_INFO(this->get_logger(), "Path Point ECEF: x=%.6f, y=%.6f, z=%.6f, Distance=%.6f", path_x, path_y, path_z, distance);

        if (distance < min_distance) {
            min_distance = distance;
        }
    }

    //RCLCPP_INFO(this->get_logger(), "Minimum Distance: %.6f", min_distance);
    return min_distance;
}


void PathFollowingNode::latLonToECEF(double latitude, double longitude, double altitude, double& x, double& y, double& z) {
    // WGS84 ellipsoid constants
    const double a = 6378137.0;  // Semi-major axis
    const double f = 1.0 / 298.257223563;  // Flattening
    const double b = a * (1.0 - f);  // Semi-minor axis
    const double e2 = 1.0 - (b * b) / (a * a);  // Square of eccentricity

    double sinLat = sin(latitude * M_PI / 180.0);
    double cosLat = cos(latitude * M_PI / 180.0);
    double sinLon = sin(longitude * M_PI / 180.0);
    double cosLon = cos(longitude * M_PI / 180.0);

    double N = a / sqrt(1.0 - e2 * sinLat * sinLat);

    x = (N + altitude) * cosLat * cosLon;
    y = (N + altitude) * cosLat * sinLon;
    z = (N * (1.0 - e2) + altitude) * sinLat;
}

    void PathFollowingNode::joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg){
      dead_man = bool(msg->buttons[dead_man_button]);
      x_axis = msg->axes[x_axis_button];
      z_axis = msg->axes[z_axis_button];
      geometry_msgs::msg::Twist twist_msg;
      twist_msg.linear.x = x_axis;
      twist_msg.angular.z = z_axis;
if (pub_twist) {
//pub_twist->publish(twist_msg);
} else {
    RCLCPP_ERROR(this->get_logger(), "Publisher is not initialized");
}  
}


double PathFollowingNode::pointToLineDistance(
    const geometry_msgs::msg::Point& point,
    const geometry_msgs::msg::Point& line_start,
    const geometry_msgs::msg::Point& line_end) {
    double dx = line_end.x - line_start.x;
    double dy = line_end.y - line_start.y;
    double num = std::abs(dy * point.x - dx * point.y + line_end.x * line_start.y - line_end.y * line_start.x);
    double den = std::sqrt(dx * dx + dy * dy);
    return num / den;
}


rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn PathFollowingNode::on_configure(const rclcpp_lifecycle::State &) {
    pub_ = this->create_publisher<std_msgs::msg::String>("lifecycle_chatter", 10);
    pub_path = this->create_publisher<nav_msgs::msg::Path>("local_path", 10);
    pub_poi_path = this->create_publisher<nav_msgs::msg::Path>("poi_path", 10);
    pub_ecef_path = this->create_publisher<nav_msgs::msg::Path>("ecef_path", 10);
    pub_enu_path = this->create_publisher<nav_msgs::msg::Path>("enu_path", 10);

    //timer_path = this->create_wall_timer(20.0s, [this]() { this->wait_path(); });
    tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
    this->get_parameter("path_service_name", service_name);
    this->get_parameter("robot_frame", robot_frame);
    this->get_parameter("local_fixed_frame", fixed_local_frame);
    this->get_parameter("global_fixed_frame", fixed_global_frame);
    this->get_parameter("path_params.look_points_ahead", points_look_ahead);
    this->get_parameter("cmd_vel_topic_name", cmd_vel_topic);
    this->get_parameter("target_distance", target_distance);
    this->get_parameter("node_frequency", node_frequency);
    this->get_parameter("fsm.change_fsm_mode_srv_name", change_fsm_mode_srv_name);
    this->get_parameter("joystick_topic_name", joy_topic);
    this->get_parameter("gps_topic", gps_topic);

    sub_odom = this->create_subscription<nav_msgs::msg::Odometry> ("/fixposition/odometry_enu",10,
    std::bind(&PathFollowingNode::odom_callback, this,std::placeholders::_1));
    pub_twist = this->create_publisher<geometry_msgs::msg::Twist>(cmd_vel_topic, 10);
    sub_joy = this->create_subscription<sensor_msgs::msg::Joy> (joy_topic,10,
    std::bind(&PathFollowingNode::joy_callback, this,std::placeholders::_1));
    sub_fix = this->create_subscription<sensor_msgs::msg::NavSatFix> (gps_topic,10,
    std::bind(&PathFollowingNode::fix_callback, this,std::placeholders::_1));
    pub_dist_last_obj = this->create_publisher<std_msgs::msg::Float32>("dist_last_obj", 10);
    pub_min_distance_ = this->create_publisher<std_msgs::msg::Float32>("min_distance_to_path", 10);
    getRPathClient = this->create_client<path_manager_interfaces::srv::GetFixFramePath>(service_name);
    FSM_client = this->create_client<ctl_mission_interfaces::srv::ChangeMode>(change_fsm_mode_srv_name);

    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn PathFollowingNode::on_activate(const rclcpp_lifecycle::State & state) {
    LifecycleNode::on_activate(state);
    auto timer_period = std::chrono::duration<double>(1.0 / node_frequency);
    timer_transform = this->create_wall_timer(timer_period, [this]() { this->timer_transform_cb(); });
    callGetFixFramePathService();
    min_point_idx = 0;
    got_path = false;
    nav_msgs::msg::Path path_robot_frame = transform_path_to_robot_frame(robot_path, robot_frame);
    min_point_idx = min_id_all_path(path_robot_frame);
    timer_path.reset();

    RCLCPP_INFO(get_logger(), "on_activate() is called.");
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn PathFollowingNode::on_deactivate(const rclcpp_lifecycle::State & state) {
    LifecycleNode::on_deactivate(state);
    timer_transform.reset();
    RCLCPP_INFO(get_logger(), "on_deactivate() is called.");
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn PathFollowingNode::on_cleanup(const rclcpp_lifecycle::State &) {
    timer_transform.reset();
    pub_.reset();
    RCLCPP_INFO(get_logger(), "on_cleanup() is called.");
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn PathFollowingNode::on_shutdown(const rclcpp_lifecycle::State & state) {
    timer_transform.reset();
    pub_.reset();
    RCLCPP_INFO(get_logger(), "on_shutdown() is called from state %s.", state.label().c_str());
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}
void PathFollowingNode::first_not_negative(const nav_msgs::msg::Path& path_in) {
            double min_distance = std::numeric_limits<double>::max();
        for (int i = min_point_idx; i < path_in.poses.size(); i++) {
            if (path_in.poses[i].pose.position.x > 0.0) {
                min_point_idx = i;
                break;
            }
        }
}
void PathFollowingNode::distances_to_wp(const nav_msgs::msg::Path& path_in, const nav_msgs::msg::Odometry& robot_odom) {
    double min_distance = std::numeric_limits<double>::max();

    // Get the robot's current position from the odometry message
    double robot_x = robot_odom.pose.pose.position.x;
    double robot_y = robot_odom.pose.pose.position.y;

    int max_idx = min_point_idx + 10;
    if (max_idx > path_in.poses.size()) max_idx = path_in.poses.size();
    for (int i = min_point_idx; i < max_idx; i++) {
        double distance = sqrt(pow(path_in.poses[i].pose.position.x - robot_x, 2) + pow(path_in.poses[i].pose.position.y - robot_y, 2));

        if (distance < min_distance) {
            min_distance = distance;
            if (i > min_point_idx) min_point_idx = i;
        }
    }
}


int PathFollowingNode::min_id_all_path(const nav_msgs::msg::Path& path_in) {
        double min_distance = std::numeric_limits<double>::max();
        int min_idx = 0;
        for (int i = 0; i < path_in.poses.size(); i++) {
            double distance = sqrt(pow(path_in.poses[i].pose.position.x, 2)+pow(path_in.poses[i].pose.position.y, 2));

                
                if (distance < min_distance) {
                    min_distance = distance;
                    min_idx = i;
                }

        }
        return min_idx;

}


double PathFollowingNode::calculateDistance(const geometry_msgs::msg::PoseStamped& p1, const geometry_msgs::msg::PoseStamped& p2) {
    double dx = p2.pose.position.x - p1.pose.position.x;
    double dy = p2.pose.position.y - p1.pose.position.y;
    return sqrt(dx * dx + dy * dy);
}

nav_msgs::msg::Path PathFollowingNode::transform_path_to_robot_frame(const nav_msgs::msg::Path& path_in, 
                                                                    const std::string output_frame) {
    nav_msgs::msg::Path path_out;
    std::string input_frame = path_in.header.frame_id;
    path_out.header.frame_id = output_frame;
    path_out.header.stamp = this->get_clock()->now();

    // Define a maximum allowable age for transforms (e.g., 0.1 seconds)
    rclcpp::Duration max_transform_age = rclcpp::Duration::from_seconds(0.5);

    for (int i = 0; i < path_in.poses.size(); i++) {
        geometry_msgs::msg::PoseStamped pose_out;
        try {
            auto transform = tf_buffer_->lookupTransform(output_frame, input_frame, tf2::TimePointZero);
            
            // Check if the transform is too old
            rclcpp::Time transform_time = transform.header.stamp;
            rclcpp::Time current_time = this->get_clock()->now();
            if (((current_time - transform_time) > max_transform_age) && output_frame == robot_frame) {
                RCLCPP_WARN(this->get_logger(), "Transform from %s to %s is too old: %f seconds. Going back to Ready",
                            input_frame.c_str(), output_frame.c_str(), (current_time - transform_time).seconds());
            //sendChangeModeRequest(FSM_client, 1); // Go back to Ready

              
            }

            tf2::doTransform(path_in.poses[i], pose_out, transform);
            pose_out.header.frame_id = output_frame;
        } catch (const tf2::TransformException & ex) {
            RCLCPP_WARN(this->get_logger(), "Could not transform %s to %s: %s", output_frame.c_str(), input_frame.c_str(), ex.what());
            continue;
        }
        path_out.poses.push_back(pose_out);
    }
    return path_out;
}

void PathFollowingNode::odom_callback(const nav_msgs::msg::Odometry& msg) {
    odometry_msg = msg;
}


int PathFollowingNode::find_closest_pose_ahead_of_robot(const nav_msgs::msg::Path& path) {
    int closest_index = -1;
    double min_distance = std::numeric_limits<double>::max();

    for (size_t i = 0; i < path.poses.size(); ++i) {
        double x = path.poses[i].pose.position.x;
        double y = path.poses[i].pose.position.y;
        double distance = std::sqrt(x * x + y * y);

        if (distance < min_distance) {
            min_distance = distance;
            closest_index = static_cast<int>(i);
        }
    }
    return closest_index;
}



void PathFollowingNode::latLonToUTM(double latitude, double longitude, double& utmNorthing, double& utmEasting, int& zone, bool& isNorth) {
    GeographicLib::UTMUPS::Forward(latitude, longitude, zone, isNorth, utmEasting, utmNorthing);
}

