/**
 * @file path_record_node.cpp
 * @brief Lifecycle state node that records a driven trajectory into a simple lat/lon path.
 *
 * ## Role in the J8 mission FSM
 * This node is activated by the mission/orchestrator when the operator wants to
 * *drive and record* a path (typically to later reuse it for path following).
 *
 * While **active**:
 * - Reads current GPS fix (lat/lon/alt).
 * - Reads joystick input and publishes Twist (so the vehicle can be driven).
 * - Periodically appends a new pose to `ll_path` when the driven distance exceeds
 *   `distance_between_record_points`.
 *
 * On **deactivate** it publishes the accumulated `nav_msgs/Path` on `ll_path_topic`.
 *
 * ## ROS contract (topics / params)
 * ### Subscriptions
 * - `gps_topic` (default: `/fixposition/navsatfix`) [sensor_msgs/msg/NavSatFix]
 * - `joystick_topic_name` (default: `joy`) [sensor_msgs/msg/Joy] (only created when active)
 *
 * ### Publications
 * - `cmd_vel_topic_name` (default: `cmd_vel_test`) [geometry_msgs/msg/Twist]
 * - `ll_path_topic` (default: `ll_path`) [nav_msgs/msg/Path]
 *
 * ### Parameters
 * - `distance_between_record_points` (m): minimum spacing between successive recorded points.
 * - `dead_man_button`, `x_axis_button`, `z_axis_button`: joystick mapping.
 * - `x_multiplier`, `z_multiplier`: scaling factors applied to joystick axes.
 * - `queue_size`: QoS depth for pubs/subs.
 *
 * ## Data model notes / gotchas
 * - The recorded `nav_msgs/Path` stores latitude in `pose.position.x` and longitude in
 *   `pose.position.y` (and altitude in `pose.position.z`). This is not a standard frame.
 *   Downstream consumers should treat it as a "lat/lon list" rather than a metric map.
 * - `latLonToUTM()` is used only to compute inter-point distance in meters.
 */

#include "ctl_mission/PathRecordNode.hpp"
#include <cmath>
#include <iostream>
#include <functional>
#include <GeographicLib/Geocentric.hpp>
#include <GeographicLib/UTMUPS.hpp>
#include <chrono>
using namespace std::chrono_literals;
PathRecordNode::PathRecordNode(const std::string &node_name, bool intra_process_comms)
    : rclcpp_lifecycle::LifecycleNode(node_name, rclcpp::NodeOptions().use_intra_process_comms(intra_process_comms)) {
    this->declare_parameter("distance_between_record_points", 1.0);
    this->declare_parameter("gps_topic", "/fixposition/navsatfix");
    this->declare_parameter("joystick_topic_name", "joy");
    this->declare_parameter("cmd_vel_topic_name", "cmd_vel_test");
    this->declare_parameter("ll_path_topic", "ll_path");
    this->declare_parameter("queue_size", 10);
    this->declare_parameter("dead_man_button", 5);
    this->declare_parameter("x_axis_button", 1);
    this->declare_parameter("z_axis_button", 3);
    this->declare_parameter("x_multiplier", 1.0);
    this->declare_parameter("z_multiplier", 1.0);

}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn 
PathRecordNode::on_configure(const rclcpp_lifecycle::State &) {
    this->get_parameter("distance_between_record_points", distance_between_record_points);
    this->get_parameter("gps_topic", gps_topic);
    this->get_parameter("cmd_vel_topic_name", cmd_vel_topic);
    this->get_parameter("ll_path_topic", ll_path_topic);
    this->get_parameter("queue_size", queue_size);
    this->get_parameter("dead_man_button", dead_man_button);
    this->get_parameter("x_axis_button", x_axis_button);
    this->get_parameter("z_axis_button", z_axis_button);
    this->get_parameter("joystick_topic_name", joy_topic);
    this->get_parameter("x_multiplier", x_multiplier);
    this->get_parameter("z_multiplier", z_multiplier);
    sub_gps = this->create_subscription<sensor_msgs::msg::NavSatFix>(
        gps_topic, queue_size, std::bind(&PathRecordNode::gps_callback, this, std::placeholders::_1));



    pub_ll_path = this->create_publisher<nav_msgs::msg::Path>(ll_path_topic, queue_size);
    pub_twist = this->create_publisher<geometry_msgs::msg::Twist>(cmd_vel_topic, queue_size);

    RCLCPP_INFO(get_logger(), "on_configure() is called.");
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
PathRecordNode::on_activate(const rclcpp_lifecycle::State & state) {
    LifecycleNode::on_activate(state);

    timer_ = this->create_wall_timer(
        100ms, std::bind(&PathRecordNode::record_points_loop, this));

    sub_joy = this->create_subscription<sensor_msgs::msg::Joy>(
        joy_topic, queue_size, std::bind(&PathRecordNode::joy_callback, this, std::placeholders::_1));
    ll_path.poses.clear();
    RCLCPP_INFO(get_logger(), "on_activate() is called.");
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
PathRecordNode::on_deactivate(const rclcpp_lifecycle::State & state) {
    RCLCPP_INFO(get_logger(), "on_deactivate() is called.");
    LifecycleNode::on_deactivate(state);
    ll_path.header.frame_id = 'll';
    pub_ll_path->publish(ll_path);
    timer_.reset();    sub_joy.reset();
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
PathRecordNode::on_cleanup(const rclcpp_lifecycle::State & state) {
    RCLCPP_INFO(get_logger(), "on_cleanup() is called.");
    // Add cleanup logic here.
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
PathRecordNode::on_shutdown(const rclcpp_lifecycle::State & state) {
    RCLCPP_INFO(get_logger(), "on_shutdown() is called.");
    // Add shutdown logic here.
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}
void PathRecordNode::gps_callback(const sensor_msgs::msg::NavSatFix::SharedPtr msg) {
    lat = msg->latitude;
    lon = msg->longitude;
    altitude = msg->altitude;
}

void PathRecordNode::joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg) {
    dead_man = static_cast<bool>(msg->buttons[dead_man_button]);
    //if (!dead_man) return; // If dead_man switch is not pressed, do nothing
    x_axis = x_multiplier* msg->axes[x_axis_button];
    z_axis = z_multiplier * msg->axes[z_axis_button];

    geometry_msgs::msg::Twist twist_msg;
    twist_msg.linear.x = x_axis;
    twist_msg.angular.z = z_axis;
    pub_twist->publish(twist_msg);
}

void PathRecordNode::record_points_loop() {
    // Example logic for recording points

    if (ll_path.poses.empty()) {
        geometry_msgs::msg::PoseStamped pose_to_add;
        pose_to_add.pose.position.x = lat;
        pose_to_add.pose.position.y = lon;
        pose_to_add.pose.position.z = altitude;
        ll_path.poses.push_back(pose_to_add);
    } else {
        double last_lat = ll_path.poses.back().pose.position.x;
        double last_lon = ll_path.poses.back().pose.position.y;

        latLonToUTM(lat, lon, utmNorthing, utmEasting, zone, isNorth);
        double lastutmNorthing, lastutmEasting;
        latLonToUTM(last_lat, last_lon, lastutmNorthing, lastutmEasting, zone, isNorth);

        float distance = sqrt(pow(utmEasting - lastutmEasting, 2) + pow(utmNorthing - lastutmNorthing, 2));

        if (distance > distance_between_record_points) {
            geometry_msgs::msg::PoseStamped pose_to_add;
            pose_to_add.pose.position.x = lat;
            pose_to_add.pose.position.y = lon;
            pose_to_add.pose.position.z = altitude;

            ll_path.poses.push_back(pose_to_add);
        }
    }
}

void PathRecordNode::latLonToUTM(double latitude, double longitude, double& utmNorthing, double& utmEasting, int& zone, bool& isNorth) {
    GeographicLib::UTMUPS::Forward(latitude, longitude, zone, isNorth, utmEasting, utmNorthing);
}

