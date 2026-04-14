#include "security_check/SecurityCheckNode.hpp"

SecurityCheckNode::SecurityCheckNode(const std::string & node_name, bool intra_process_comms)
: rclcpp_lifecycle::LifecycleNode(node_name, rclcpp::NodeOptions().use_intra_process_comms(intra_process_comms)),
  message_age_threshold_(1.0), th1(0.5), th2(1.0)
{
    this->declare_parameter("cmd_vel_topic_name", "cmd_vel_test");
    this->declare_parameter("secured_cmd_vel_topic_name", "secure_cmd_vel_test");
    this->declare_parameter("lidar_topic", "weighted_scan");
    this->declare_parameter("lidar_frame", "Velodyne_link");
    this->declare_parameter("robot_frame", "base_link");
    this->declare_parameter("dissable_service_name", "enable_security_check");
    this->declare_parameter("threshold_1", 1.0);
    this->declare_parameter("threshold_2", 3.0);
    this->declare_parameter("threshold_path", 1.5);
    this->declare_parameter("factor", 0.5);
    this->declare_parameter("bypass_password", "disable");
    this->declare_parameter("get_security_check_srv_name", "get_security_check");
    this->declare_parameter("password_check_srv_name", "security_password_check");
    this->declare_parameter("min_distance_to_path_topic_name", "min_distance_to_path");
    this->declare_parameter("fsm.change_fsm_mode_srv_name", "change_fsm_mode");
    this->declare_parameter("fsm.get_fsm_srv_name", "get_fsm_mode");

    th1 = 1.0;
    th2 = 3.0;
    threshold_path = 1.5;
    factor = 0.5;
    last_msg_time_ = this->get_clock()->now();
    obstacle_detected = false;
    last_lidar_msg_time_ = this->get_clock()->now();
    adjusted_msg = std::make_shared<geometry_msgs::msg::Twist>();
    current_mode_ = Mode::Ready; // Initialize current_mode_ to Ready
}


void SecurityCheckNode::cmdvelCallbak(const geometry_msgs::msg::Twist::SharedPtr msg)
{
    last_msg_time_ = this->get_clock()->now();
    received_cmd_vel = msg;
}

void SecurityCheckNode::perform_security_check() {
    auto now = this->get_clock()->now();
    check_futures();

    // Ensure the publisher is activated
    if (!pub_cmd_vel->is_activated()) {
        RCLCPP_WARN(this->get_logger(), "Publisher not activated, skipping publish");
        return;
    }

    // Check if the security check is bypassed
    if (security_bypass_enabled_) {
        if (received_cmd_vel != nullptr) {
            pub_cmd_vel->publish(*received_cmd_vel);
        } else {
            RCLCPP_WARN(this->get_logger(), "received_cmd_vel is null");
        }
        return;
    }

    // Check if the cmd_vel message is too old
    if ((now - last_msg_time_).seconds() > message_age_threshold_) {
        // Publish stop message if too old
        geometry_msgs::msg::Twist stop_msg;
        stop_msg.linear.x = 0.0;
        stop_msg.angular.z = 0.0;
        pub_cmd_vel->publish(stop_msg);
    } else if (security_enabled_) {
        // Publish adjusted message if security check is enabled
        pub_cmd_vel->publish(*adjusted_msg);
    } else {
        // Publish received_cmd_vel if security check is disabled
        if (received_cmd_vel != nullptr) {
            pub_cmd_vel->publish(*received_cmd_vel);
        } else {
            RCLCPP_WARN(this->get_logger(), "received_cmd_vel is null");
        }
    }
}

void SecurityCheckNode::lidarCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
{
    float min_weighted_distance = std::numeric_limits<float>::max();
    int num_ranges = msg->ranges.size();
    float min_weight = 0.2;  // Minimum weight for angles near ±π/2

    // Loop through each range and calculate the weighted distance
    for (int i = 0; i < num_ranges; ++i) {
        if (msg->ranges[i] < msg->range_min || msg->ranges[i] > msg->range_max) {
            continue; // Ignore invalid range values
        }

        // Calculate the angle of the lidar point
        float angle = msg->angle_min + i * msg->angle_increment;

        // Calculate the weight based on the angle
        float weight = (1 - min_weight) * std::cos(angle) + (1 + min_weight) / 2;

        // Calculate the weighted distance
        float weighted_distance = msg->ranges[i] / weight;

        // Update the minimum weighted distance
        if (weighted_distance < min_weighted_distance) {
            min_weighted_distance = weighted_distance;
        }
    }

    adjusted_msg = received_cmd_vel;

    if (min_weighted_distance <= th1) {
        // Stop the robot because an obstacle is too close
        adjusted_msg->linear.x = 0.0;
        adjusted_msg->angular.z = 0.0;
        RCLCPP_WARN(this->get_logger(), "Robot stopped for obstacle presence!!!!!!!!!!!!!!!!!");
        if (current_mode_ == Mode::PathFollowing && !security_bypass_enabled_) {
            sendChangeModeRequest(change_FSM_mode_srv_, 1);
        }
    } else {
        // Linearly scale the speed based on the weighted distance to the obstacle
        float speed_limit = factor * (min_weighted_distance - th1);
        if (speed_limit < 0.2) speed_limit = 0.2;

        // Check and limit linear speed
        if (fabs(speed_limit) < fabs(received_cmd_vel->linear.x)) {
            RCLCPP_WARN(this->get_logger(), "Limiting linear speed for obstacle presence!!!!!!!!!!!!!!!!!");
            adjusted_msg->linear.x = std::copysign(speed_limit, received_cmd_vel->linear.x); // Maintain direction of original speed
        }

        // Check and limit angular speed
        if (fabs(speed_limit) < fabs(received_cmd_vel->angular.z)) {
            RCLCPP_WARN(this->get_logger(), "Limiting angular speed for obstacle presence!!!!!!!!!!!!!!!!!");
            adjusted_msg->angular.z = std::copysign(speed_limit, received_cmd_vel->angular.z); // Maintain direction of original speed
        }
    }
}



void SecurityCheckNode::passwordServiceCallback(
    const std::shared_ptr<security_check_interfaces::srv::PasswordCheck::Request> request,
    std::shared_ptr<security_check_interfaces::srv::PasswordCheck::Response> response)
{
    std::string bypass_password;
    this->get_parameter("bypass_password", bypass_password);

    if (request->password == bypass_password) {
        security_bypass_enabled_ = true;
        response->success = true;
        response->message = "Security check bypassed successfully.";
        RCLCPP_INFO(this->get_logger(), "Security check bypassed with correct password.");
    }
    else if (request->password == "enable")
    {
        security_bypass_enabled_ = false;
        response->success = true;
        response->message = "Security check enabled successfully.";
        RCLCPP_INFO(this->get_logger(), "Security check enabled with correct password.");    
    
    } else {
        response->success = false;
        response->message = "Incorrect password for security check bypass.";
        RCLCPP_WARN(this->get_logger(), "Incorrect password for security check bypass.");
    }
}

void SecurityCheckNode::getSecurity(
        const std::shared_ptr<security_check_interfaces::srv::GetSecurityCheckStatus::Request> request,
        std::shared_ptr<security_check_interfaces::srv::GetSecurityCheckStatus::Response> response)
    {
       response->status = security_enabled_;

    }
void SecurityCheckNode::checkChangeModeFuture() {
    auto wait_time = std::chrono::seconds(0);
    if (service_future_fsm.valid() && service_future_fsm.wait_for(wait_time) == std::future_status::ready) {
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


void SecurityCheckNode::sendChangeModeRequest(
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
    auto timer_period = std::chrono::milliseconds(100); // Explicitly creating a duration of 100 milliseconds
    future_check_timer_fsm = this->create_wall_timer(
        timer_period, std::bind(&SecurityCheckNode::checkChangeModeFuture, this));
}

void SecurityCheckNode::handle_security_service(const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
                                                std::shared_ptr<std_srvs::srv::SetBool::Response> response) {
  security_enabled_ = request->data;
  if (current_mode_ == Mode::PathFollowing) security_enabled_ = true;
  response->success = true;
  response->message = "Security check " + std::string(security_enabled_ ? "enabled" : "disabled");
}

    void SecurityCheckNode::fetch_fsm_mode() {
        if (!get_mode_client_->wait_for_service(std::chrono::seconds(1))) {
            RCLCPP_ERROR(this->get_logger(), "Service get_fsm_mode not available.");
            return;
        }
        auto request = std::make_shared<ctl_mission_interfaces::srv::GetMode::Request>();
        auto future_result = get_mode_client_->async_send_request(request);
        future_results_.push_back(future_result);
    }

    void SecurityCheckNode::check_futures() {
        for (auto it = future_results_.begin(); it != future_results_.end();) {
            auto& future = *it;
            if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                auto response = future.get();
                current_mode_ = static_cast<Mode>(response->mode);
                if (current_mode_ == Mode::PathFollowing) security_enabled_ = true;
                //RCLCPP_INFO(this->get_logger(), "Current FSM mode updated to %d", static_cast<int>(current_mode_));
                it = future_results_.erase(it);
            } else {
                ++it;
            }
        }
    }


void SecurityCheckNode::minDistanceCallback(const std_msgs::msg::Float32::SharedPtr msg) {
    float min_distance = msg->data;

    //RCLCPP_INFO(this->get_logger(), "Received minimum distance: %.6f", min_distance);
    //RCLCPP_INFO(this->get_logger(), "Threshold: %.6f", threshold_path);

    if (min_distance > threshold_path) {
        // Stop the robot because the distance is below the threshold
        adjusted_msg->linear.x = 0.0f;
        adjusted_msg->angular.z = 0.0f;
        RCLCPP_WARN(this->get_logger(), "Robot stopped due to minimum distance threshold! Min distance: %.6f", min_distance);

        if (current_mode_ == Mode::PathFollowing) {
            sendChangeModeRequest(change_FSM_mode_srv_, 1);
        }
    } 
}



rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
SecurityCheckNode::on_configure(const rclcpp_lifecycle::State &)
{
    RCLCPP_INFO(get_logger(), "on_configure() is called.");
    this->get_parameter("cmd_vel_topic_name", input_cmd_vel_topic);
    this->get_parameter("secured_cmd_vel_topic_name", output_cmd_vel_topic);
    this->get_parameter("lidar_topic", lidar_topic);
    this->get_parameter("threshold_1", th1);
    this->get_parameter("threshold_2", th2);
    this->get_parameter("factor", factor);
    this->get_parameter("dissable_service_name", dissable_srv_name);
    this->get_parameter("bypass_password", bypass_password);
    this->get_parameter("threshold_path", threshold_path);
    this->get_parameter("fsm.change_fsm_mode_srv_name", change_fsm_mode_srv_name);
    this->get_parameter("fsm.get_fsm_srv_name", get_fsm_srv_name);
    this->get_parameter("get_security_check_srv_name", get_security_check_srv_name);
    this->get_parameter("password_check_srv_name", password_check_srv_name);
    this->get_parameter("min_distance_to_path_topic_name", min_distance_to_path_topic_name);

    pub_cmd_vel = this->create_publisher<geometry_msgs::msg::Twist>(output_cmd_vel_topic, 10);
    security_bypass_enabled_ = true;
    auto check_timer_period = std::chrono::milliseconds(20); // Explicitly creating a duration of 100 milliseconds

    security_check_timer_ = this->create_wall_timer(
        check_timer_period, std::bind(&SecurityCheckNode::perform_security_check, this));
    change_FSM_mode_srv_ = this->create_client<ctl_mission_interfaces::srv::ChangeMode>(change_fsm_mode_srv_name);
    get_mode_client_ = this->create_client<ctl_mission_interfaces::srv::GetMode>(get_fsm_srv_name);

    sub_cmd_vel = this->create_subscription<geometry_msgs::msg::Twist>(
    input_cmd_vel_topic, 10, std::bind(&SecurityCheckNode::cmdvelCallbak, this, std::placeholders::_1));
    tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
    // Initialize the service server
    security_service_ = this->create_service<std_srvs::srv::SetBool>(dissable_srv_name, 
    std::bind(&SecurityCheckNode::handle_security_service, this, std::placeholders::_1, std::placeholders::_2));
    mode_timer_ = this->create_wall_timer(std::chrono::seconds(1), std::bind(&SecurityCheckNode::fetch_fsm_mode, this));
    getSecurityChecksrv = this->create_service<security_check_interfaces::srv::GetSecurityCheckStatus>
    (get_security_check_srv_name,std::bind(&SecurityCheckNode::getSecurity, this, std::placeholders::_1, std::placeholders::_2));
    // Password service
    password_service_ = this->create_service<security_check_interfaces::srv::PasswordCheck>(
        password_check_srv_name, std::bind(&SecurityCheckNode::passwordServiceCallback, this, std::placeholders::_1, std::placeholders::_2));
    // Initially enable security check
    sub_min_distance_ = this->create_subscription<std_msgs::msg::Float32>(
    min_distance_to_path_topic_name, 10, std::bind(&SecurityCheckNode::minDistanceCallback, this, std::placeholders::_1));
    security_enabled_ = true;
     sub_lidar = this->create_subscription<sensor_msgs::msg::LaserScan>(
            lidar_topic, 10, std::bind(&SecurityCheckNode::lidarCallback, this, std::placeholders::_1));
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
SecurityCheckNode::on_activate(const rclcpp_lifecycle::State &)
{
    RCLCPP_INFO(get_logger(), "on_activate() is called.");
    pub_cmd_vel->on_activate();
   
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
SecurityCheckNode::on_deactivate(const rclcpp_lifecycle::State &)
{
    RCLCPP_INFO(get_logger(), "on_deactivate() is called.");
    pub_cmd_vel->on_deactivate();
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
SecurityCheckNode::on_cleanup(const rclcpp_lifecycle::State &)
{
    RCLCPP_INFO(get_logger(), "on_cleanup() is called.");
    pub_cmd_vel.reset();
    sub_cmd_vel.reset();
    lidar_subs_.clear();
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
SecurityCheckNode::on_shutdown(const rclcpp_lifecycle::State & state)
{
    RCLCPP_INFO(get_logger(), "on_shutdown() is called from state %s.", state.label().c_str());
    pub_cmd_vel.reset();
    sub_cmd_vel.reset();
    lidar_subs_.clear();
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}
