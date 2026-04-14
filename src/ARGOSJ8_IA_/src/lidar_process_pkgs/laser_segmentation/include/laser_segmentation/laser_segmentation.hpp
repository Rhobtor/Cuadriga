#ifndef LASER_SEGMENTATION__LASER_SEGMENTATION_HPP_
#define LASER_SEGMENTATION__LASER_SEGMENTATION_HPP_

// C++
#include <memory>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <limits>

// ROS
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"
#include "rcl_interfaces/msg/set_parameters_result.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"
#include "visualization_msgs/msg/marker_array.hpp"

// TF2
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2_eigen/tf2_eigen.hpp>

// PCL
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl_ros/transforms.hpp>
#include <sensor_msgs/point_cloud2_iterator.hpp>

// SIMPLE LASER GEOMETRY
#include "slg_msgs/segment2D.hpp"
#include "slg_msgs/msg/segment_array.hpp"

// LASER SEGMENTATION
#include "laser_segmentation/parula.hpp"
#include "laser_segmentation/parameter_handler.hpp"
#include "laser_segmentation/segmentation/segmentation.hpp"
#include "laser_segmentation/segmentation/jump_distance.hpp"
#include "laser_segmentation/segmentation/jump_distance_merge.hpp"

using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

namespace laser_segmentation
{

class LaserSegmentation : public rclcpp_lifecycle::LifecycleNode
{
public:
  explicit LaserSegmentation(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());
  ~LaserSegmentation() = default;

  CallbackReturn on_configure(const rclcpp_lifecycle::State & state) override;
  CallbackReturn on_activate(const rclcpp_lifecycle::State & state) override;
  CallbackReturn on_deactivate(const rclcpp_lifecycle::State & state) override;
  CallbackReturn on_cleanup(const rclcpp_lifecycle::State & state) override;
  CallbackReturn on_shutdown(const rclcpp_lifecycle::State & state) override;

protected:
  void scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr scan);
  std::vector<slg::Segment2D> filter_segments(const std::vector<slg::Segment2D> & segments);
  visualization_msgs::msg::MarkerArray create_segment_viz_points(
    std_msgs::msg::Header header,
    std::vector<slg::Segment2D> segment_list);
  std_msgs::msg::ColorRGBA get_parula_color(unsigned int index, unsigned int max);
  std_msgs::msg::ColorRGBA get_palette_color(unsigned int index);

  void convert_segments_to_pointcloud(
    const slg_msgs::msg::SegmentArray & segment_array, sensor_msgs::msg::PointCloud2 & pointcloud_msg, sensor_msgs::msg::PointCloud2 & lidar_frame_pointcloud_msg);

  void convert_pointcloud_to_weighted_laserscan(
    const sensor_msgs::msg::PointCloud2 & lidar_frame_pointcloud_msg, sensor_msgs::msg::LaserScan & laserscan_msg, const std_msgs::msg::Header & header);
  std::vector<geometry_msgs::msg::Point> check_consistent_detection(const std::vector<geometry_msgs::msg::Point>& current_detections);
  sensor_msgs::msg::PointCloud2 publish_filtered_pointcloud(const pcl::PointCloud<pcl::PointXYZ>& pcl_cloud, 
                                  const std::vector<geometry_msgs::msg::Point>& consistent_detections);

  // Node parameters
  std::string scan_topic_;
  std::string seg_topic_;
  std::string pointcloud_topic_;
  std::string weighted_scan_topic_;
  double side_vision_limit_;
  double weight_factor_;
  double angle_increment_;
  double range_min_;
  double range_max_;
  double spatial_threshold;
  double inner_zone_angle_;  // Angle defining the inner zone boundary
  double middle_zone_angle_; // Angle defining the middle zone boundary
  int64_t buffer_size_;
  int64_t detection_threshold_;

  // Publishers
  rclcpp_lifecycle::LifecyclePublisher<slg_msgs::msg::SegmentArray>::SharedPtr segment_pub_;
  rclcpp_lifecycle::LifecyclePublisher<visualization_msgs::msg::MarkerArray>::SharedPtr segment_viz_points_pub_;
  rclcpp_lifecycle::LifecyclePublisher<sensor_msgs::msg::PointCloud2>::SharedPtr pointcloud_pub_;
  rclcpp_lifecycle::LifecyclePublisher<sensor_msgs::msg::LaserScan>::SharedPtr weighted_scan_pub_;
  rclcpp_lifecycle::LifecyclePublisher<sensor_msgs::msg::LaserScan>::SharedPtr raw_weighted_scan_pub_;

  // Subscribers
  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_sub_;

  // TF2 buffer and listener
  std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;

  // Other member variables
  std::unique_ptr<ParameterHandler> param_handler_;
  Parameters * params_;
  std::shared_ptr<Segmentation> segmentation_;
  std::deque<std::vector<float>> scan_buffer_;
  std::deque<std::vector<geometry_msgs::msg::Point>> detection_history_;
};

}  // namespace laser_segmentation

#endif  // LASER_SEGMENTATION__LASER_SEGMENTATION_HPP_
