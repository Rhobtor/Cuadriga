/**
 *  @file
 *  @brief Convert Data classes to ROS2 msgs
 *
 * \verbatim
 *  ___    ___
 *  \  \  /  /
 *   \  \/  /   Fixposition AG
 *   /  /\  \   All right reserved.
 *  /__/  \__\
 * \endverbatim
 *
 */

/* ROS */
#if __has_include(<tf2_eigen/tf2_eigen.hpp>)
#include <tf2_eigen/tf2_eigen.hpp>
#else
// This header was deprecated as of ROS2 Humble, but is still required in order to support Foxy.
#include <tf2_eigen/tf2_eigen.h>
#endif

/* PACKAGE */
#include <fixposition_driver_ros2/data_to_ros2.hpp>

/* ROS */
#include <rclcpp/rclcpp.hpp>

namespace fixposition {

static inline builtin_interfaces::msg::Time StampToRosTime(const times::GpsTime& stamp) {
    // Fixposition can output tow/wno=0 during bringup/no-fix. Publishing stamp=0
    // leads to TF_OLD_DATA and broken message filters; fallback to current time.
    if (stamp.tow == 0.0 && stamp.wno == 0) {
        return rclcpp::Clock(RCL_SYSTEM_TIME).now();
    }
    return GpsTimeToMsgTime(stamp);
}

void ImuDataToMsg(const ImuData& data, sensor_msgs::msg::Imu& msg) {
    msg.header.stamp = StampToRosTime(data.stamp);
    msg.header.frame_id = data.frame_id;

    tf2::toMsg(data.linear_acceleration, msg.linear_acceleration);
    tf2::toMsg(data.angular_velocity, msg.angular_velocity);
}

void NavSatStatusDataToMsg(const NavSatStatusData& data, sensor_msgs::msg::NavSatStatus& msg) {
    msg.status = data.status;
    msg.service = data.service;
}

void NavSatFixDataToMsg(const NavSatFixData& data, sensor_msgs::msg::NavSatFix& msg) {
    msg.header.stamp = StampToRosTime(data.stamp);
    msg.header.frame_id = data.frame_id;
    NavSatStatusDataToMsg(data.status, msg.status);
    msg.latitude = data.latitude;
    msg.longitude = data.longitude;
    msg.altitude = data.altitude;

    Eigen::Map<Eigen::Matrix<double, 3, 3>> cov_map =
        Eigen::Map<Eigen::Matrix<double, 3, 3>>(msg.position_covariance.data());
    cov_map = data.cov;

    msg.position_covariance_type = data.position_covariance_type;
}

void PoseWithCovDataToMsg(const PoseWithCovData& data, geometry_msgs::msg::PoseWithCovariance& msg) {
    msg.pose.position = tf2::toMsg(data.position);
    msg.pose.orientation = tf2::toMsg(data.orientation);

    Eigen::Map<Eigen::Matrix<double, 6, 6>> cov_map = Eigen::Map<Eigen::Matrix<double, 6, 6>>(msg.covariance.data());
    cov_map = data.cov;
}

void TwistWithCovDataToMsg(const fixposition::TwistWithCovData& data, geometry_msgs::msg::TwistWithCovariance& msg) {
    tf2::toMsg(data.linear, msg.twist.linear);
    tf2::toMsg(data.angular, msg.twist.angular);

    Eigen::Map<Eigen::Matrix<double, 6, 6>> cov_map = Eigen::Map<Eigen::Matrix<double, 6, 6>>(msg.covariance.data());
    cov_map = data.cov;
}

void OdometryDataToMsg(const fixposition::OdometryData& data, nav_msgs::msg::Odometry& msg) {
    msg.header.stamp = StampToRosTime(data.stamp);
    msg.header.frame_id = data.frame_id;
    msg.child_frame_id = data.child_frame_id;

    PoseWithCovDataToMsg(data.pose, msg.pose);
    TwistWithCovDataToMsg(data.twist, msg.twist);
}

void VrtkDataToMsg(const VrtkData& data, fixposition_driver_ros2::msg::VRTK& msg) {
    msg.header.stamp = StampToRosTime(data.stamp);
    msg.header.frame_id = data.frame_id;
    msg.pose_frame = data.pose_frame;
    msg.kin_frame = data.kin_frame;

    PoseWithCovDataToMsg(data.pose, msg.pose);
    TwistWithCovDataToMsg(data.velocity, msg.velocity);
    tf2::toMsg(data.acceleration, msg.acceleration);
    msg.fusion_status = data.fusion_status;
    msg.imu_bias_status = data.imu_bias_status;
    msg.gnss1_status = data.gnss1_status;
    msg.gnss2_status = data.gnss2_status;
    msg.wheelspeed_status = data.wheelspeed_status;
    msg.version = data.version;
}

void TfDataToMsg(const TfData& data, geometry_msgs::msg::TransformStamped& msg) {
    msg.header.frame_id = data.frame_id;
    msg.child_frame_id = data.child_frame_id;

    msg.header.stamp = StampToRosTime(data.stamp);

    msg.transform.rotation = tf2::toMsg(data.rotation);
    tf2::toMsg(data.translation, msg.transform.translation);
}

}  // namespace fixposition
