#ifndef PATH_FOLLOWING_NODE_HPP_
#define PATH_FOLLOWING_NODE_HPP_

#include <chrono>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <cmath> 
#include "lifecycle_msgs/msg/transition.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/publisher.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"
#include "std_msgs/msg/string.hpp"
#include "tf2/exceptions.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "path_manager_interfaces/srv/get_fix_frame_path.hpp"
#include "nav_msgs/msg/path.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "sensor_msgs/msg/joy.hpp"
#include "sensor_msgs/msg/nav_sat_fix.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "ctl_mission_interfaces/srv/change_mode.hpp"
#include "std_msgs/msg/float32.hpp"
#include <GeographicLib/UTMUPS.hpp>

using namespace std::chrono_literals;

class PathFollowingNode : public rclcpp_lifecycle::LifecycleNode {
public:
    explicit PathFollowingNode(const std::string &node_name, bool intra_process_comms = false);

    // Lifecycle methods
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_configure(const rclcpp_lifecycle::State &);
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_activate(const rclcpp_lifecycle::State &state);
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State &state);
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_cleanup(const rclcpp_lifecycle::State &);
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_shutdown(const rclcpp_lifecycle::State &state);
    // Custom methods
    void timer_transform_cb();
    void wait_path();
    void callGetFixFramePathService();
    void checkFuture();
    void joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg);
    void fix_callback(const sensor_msgs::msg::NavSatFix::SharedPtr msg);
    double calculateDistance(const geometry_msgs::msg::PoseStamped& p1, const geometry_msgs::msg::PoseStamped& p2);

    // Utility functions
    nav_msgs::msg::Path transform_path_to_robot_frame(const nav_msgs::msg::Path& path_in, const std::string output_frame);
    int find_closest_pose_ahead_of_robot(const nav_msgs::msg::Path& path);
    void distances_to_wp(const nav_msgs::msg::Path& path_in, const nav_msgs::msg::Odometry& robot_odom);
    void sendChangeModeRequest(const rclcpp::Client<ctl_mission_interfaces::srv::ChangeMode>::SharedPtr& client,
    int transition_id);
    void checkChangeModeFuture();
    double pointToLineDistance( const geometry_msgs::msg::Point& point,
    const geometry_msgs::msg::Point& line_start,
    const geometry_msgs::msg::Point& line_end);
    int min_id_all_path(const nav_msgs::msg::Path& path_in);
    void first_not_negative(const nav_msgs::msg::Path& path_in) ;
    void odom_callback(const nav_msgs::msg::Odometry& path);
    void latLonToUTM(double latitude, double longitude, double& utmNorthing, double& utmEasting, int& zone, bool& isNorth);
    double computeMinDistanceToPath(double robot_ecef_x, double robot_ecef_y, double robot_ecef_z);
    void latLonToECEF(double latitude, double longitude, double altitude, double& x, double& y, double& z);


private:
    std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::String>> pub_;
    std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<nav_msgs::msg::Path>> pub_path;
    std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<nav_msgs::msg::Path>> pub_poi_path;
    std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<nav_msgs::msg::Path>> pub_ecef_path;
    std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<nav_msgs::msg::Path>> pub_enu_path;
    std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::Float32>> pub_min_distance_;  // Add this line


    std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<geometry_msgs::msg::Twist>> pub_twist;
    std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::Float32>> pub_dist_last_obj;
    std::shared_ptr<rclcpp::Subscription<sensor_msgs::msg::Joy>> sub_joy;
    std::shared_ptr<rclcpp::Subscription<sensor_msgs::msg::NavSatFix>> sub_fix;
    std::shared_ptr<rclcpp::Subscription<nav_msgs::msg::Odometry>> sub_odom;

    std::shared_ptr<rclcpp::Client<ctl_mission_interfaces::srv::ChangeMode>> FSM_client;

    std::shared_ptr<rclcpp::TimerBase> timer_path;
    std::shared_ptr<rclcpp::TimerBase> timer_transform;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    std::unique_ptr<tf2_ros::Buffer> tf_buffer_;  
    rclcpp::Client<path_manager_interfaces::srv::GetFixFramePath>::SharedPtr getRPathClient;
    rclcpp::Client<path_manager_interfaces::srv::GetFixFramePath>::SharedFuture service_future_;
    std::shared_ptr<rclcpp::TimerBase> future_check_timer_;
    nav_msgs::msg::Path robot_path;
    nav_msgs::msg::Odometry odometry_msg;
    std::shared_future<std::shared_ptr<ctl_mission_interfaces::srv::ChangeMode::Response>> service_future_fsm;

    rclcpp::TimerBase::SharedPtr future_check_timer_fsm;
    std::string robot_name, robot_frame, change_fsm_mode_srv_name, fixed_local_frame, 
                fixed_global_frame, service_name, cmd_vel_topic, joy_topic, gps_topic;
    bool got_path = false, fetch_path_ = false, path_finished = false;
    int min_point_idx = 0, points_look_ahead, points_to_local_planner;
    int dead_man_button = 5, x_axis_button = 1, z_axis_button = 2;
    double x_axis, z_axis, target_distance, current_height, node_frequency;
    bool dead_man;

};

#endif // PATH_FOLLOWING_NODE_HPP_
