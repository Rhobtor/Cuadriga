#ifndef PATH_RECORD_NODE_HPP_
#define PATH_RECORD_NODE_HPP_

#include <chrono>
#include <memory>
#include <string>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_lifecycle/lifecycle_node.hpp>
#include "sensor_msgs/msg/joy.hpp"
#include "sensor_msgs/msg/nav_sat_fix.hpp"
#include "nav_msgs/msg/path.hpp"
#include "geometry_msgs/msg/twist.hpp"

class PathRecordNode : public rclcpp_lifecycle::LifecycleNode {
public:
    explicit PathRecordNode(const std::string & node_name, bool intra_process_comms = false);

    // Lifecycle transition callback overrides
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_configure(const rclcpp_lifecycle::State &);
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_activate(const rclcpp_lifecycle::State &);
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State &);
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_cleanup(const rclcpp_lifecycle::State &);
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_shutdown(const rclcpp_lifecycle::State &);

private:
    void gps_callback(const sensor_msgs::msg::NavSatFix::SharedPtr msg);
    void joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg);
    void record_points_loop();
    int get_idx_min_distance_to_path(const nav_msgs::msg::Path& path);


    // Publishers and subscribers
    std::shared_ptr<rclcpp::Publisher<nav_msgs::msg::Path>> pub_ll_path;
    std::shared_ptr<rclcpp::Subscription<sensor_msgs::msg::NavSatFix>> sub_gps;
    std::shared_ptr<rclcpp::Subscription<sensor_msgs::msg::Joy>> sub_joy;
    std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<geometry_msgs::msg::Twist>> pub_twist;

    // Other member variables
    std::shared_ptr<rclcpp::TimerBase> timer_;
    nav_msgs::msg::Path ll_path;
    double lat, lon, altitude, distance_between_record_points;
    double utmNorthing, utmEasting, x_axis, z_axis, x_multiplier, z_multiplier;
    int zone;
    bool isNorth, dead_man;
    int dead_man_button, x_axis_button, z_axis_button;
    std::string gps_topic, joy_topic, cmd_vel_topic, ll_path_topic;
    int queue_size;
    // Helper methods if any
    void latLonToUTM(double latitude, double longitude, double& utmNorthing, double& utmEasting, int& zone, bool& isNorth);
};

#endif  // PATH_RECORD_NODE_HPP_
