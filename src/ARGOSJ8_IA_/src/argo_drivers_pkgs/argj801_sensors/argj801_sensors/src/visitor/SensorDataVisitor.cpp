#include "visitor/SensorDataVisitor.hpp"

using namespace Visitor;

SensorDataVisitor::SensorDataVisitor()
{

}

void SensorDataVisitor::visitLidar(std::shared_ptr<SensorData::LidarData> lidarData)
{
  sensor_msgs::msg::LaserScan laserScan;

  laserScan.header.stamp = rclcpp::Time(static_cast<int64_t>(lidarData->getTime()* 1e9));
  laserScan.header.frame_id = lidarData->getFrameId();
  laserScan.angle_min = lidarData->getAngleMin();
  laserScan.angle_max = lidarData->getAngleMax();
  laserScan.angle_increment = lidarData->getAngleIncrement();
  laserScan.time_increment = lidarData->getTimeIncrement();
  laserScan.scan_time = lidarData->getScanTime();
  laserScan.range_min = lidarData->getRangeMin();
  laserScan.range_max = lidarData->getRangeMax();
  laserScan.ranges = lidarData->getRanges();

  if(lidarData->getPublisher()->get_subscription_count() > 0)
    lidarData->getPublisher()->publish(laserScan);
}

void SensorDataVisitor::visitMotorTelemetry(std::shared_ptr<SensorData::MotorTelemetry> motorTelemetry)
{
  argj801_sensors_msgs::msg::MotorTelemetry motorTelemetryMsg;

  motorTelemetryMsg.header.stamp = rclcpp::Time(static_cast<int64_t>(motorTelemetry->getTime()* 1e9));
  motorTelemetryMsg.header.frame_id = motorTelemetry->getFrameId();
  motorTelemetryMsg.rpm = motorTelemetry->getRpm();
  motorTelemetryMsg.endocer = motorTelemetry->getEncoder();

  if(motorTelemetry->getPublisher()->get_subscription_count() > 0)
    motorTelemetry->getPublisher()->publish(motorTelemetryMsg);
}

void SensorDataVisitor::visitOdometer(std::shared_ptr<SensorData::OdometerData> odometer)
{
  argj801_sensors_msgs::msg::Odometer odometerMsg;

  odometerMsg.header.stamp = rclcpp::Time(static_cast<int64_t>(odometer->getTime()* 1e9));
  odometerMsg.header.frame_id = odometer->getFrameId();
  odometerMsg.odometer = odometer->getOdometer();

  if(odometer->getPublisher()->get_subscription_count() > 0)
    odometer->getPublisher()->publish(odometerMsg);
}

void SensorDataVisitor::visitTwist(std::shared_ptr<SensorData::Twist> twist)
{
  geometry_msgs::msg::TwistStamped twistMsg;

  twistMsg.header.stamp = rclcpp::Time(static_cast<int64_t>(twist->getTime() * 1e9));
  twistMsg.header.frame_id = twist->getFrameId();
  twistMsg.twist.linear.x = twist->getLinearX();
  twistMsg.twist.linear.y = twist->getLinearY();
  twistMsg.twist.linear.z = twist->getLinearZ();
  twistMsg.twist.angular.x = twist->getAngularX();
  twistMsg.twist.angular.y = twist->getAngularY();
  twistMsg.twist.angular.z = twist->getAngularZ();

  if(twist->getPublisher()->get_subscription_count() > 0)
    twist->getPublisher()->publish(twistMsg);
}

void SensorDataVisitor::visitImage(std::shared_ptr<SensorData::CompressedImage> image)
{
  sensor_msgs::msg::CompressedImage imageMsg;

  imageMsg.header.stamp = rclcpp::Time(static_cast<int64_t>(image->getTime() * 1e9));
  imageMsg.header.frame_id = image->getFrameId();
  imageMsg.format = image->getFormat();
  imageMsg.data = image->getData();

  if(image->getPublisher()->get_subscription_count() > 0)
    image->getPublisher()->publish(imageMsg);
}
