#include "laser_segmentation/laser_segmentation.hpp"

namespace laser_segmentation
{

LaserSegmentation::LaserSegmentation(const rclcpp::NodeOptions & options)
: rclcpp_lifecycle::LifecycleNode("laser_segmentation", "", options),
  tf_buffer_(std::make_shared<tf2_ros::Buffer>(this->get_clock())),
  tf_listener_(std::make_shared<tf2_ros::TransformListener>(*tf_buffer_))
{
  // Declare parameters with default values
  this->declare_parameter<std::string>("scan_topic", "sick_scan");
  this->declare_parameter<std::string>("seg_topic", "segments");
  this->declare_parameter<std::string>("pointcloud_topic", "pointcloud");
  this->declare_parameter<std::string>("weighted_scan_topic", "weighted_scan");
  this->declare_parameter<double>("side_vision_limit", 30.0);
  this->declare_parameter<double>("weight_factor", 0.6);
  this->declare_parameter<double>("angle_increment", M_PI / 180);  // 1 degree
  this->declare_parameter<double>("range_min", 0.1);
  this->declare_parameter<double>("range_max", 10.0);
  this->declare_parameter<int64_t>("buffer_size", 20);
  this->declare_parameter<int64_t>("detection_threshold",10);
  this->declare_parameter<double>("spatial_threshold", 0.1);  // Example threshold value
  this->declare_parameter<double>("inner_zone_angle", 30.0);
  this->declare_parameter<double>("middle_zone_angle", 60.0);
}

CallbackReturn LaserSegmentation::on_configure(const rclcpp_lifecycle::State &)
{
  // Handles storage and dynamic configuration of parameters.
  // Returns pointer to data current param settings.
  param_handler_ = std::make_unique<ParameterHandler>(shared_from_this(), this->get_logger());
  params_ = param_handler_->getParams();

  // Retrieve parameter values
  scan_topic_ = this->get_parameter("scan_topic").as_string();
  seg_topic_ = this->get_parameter("seg_topic").as_string();
  pointcloud_topic_ = this->get_parameter("pointcloud_topic").as_string();
  weighted_scan_topic_ = this->get_parameter("weighted_scan_topic").as_string();
  side_vision_limit_ = this->get_parameter("side_vision_limit").as_double();
  weight_factor_ = this->get_parameter("weight_factor").as_double();
  angle_increment_ = this->get_parameter("angle_increment").as_double();
  range_min_ = this->get_parameter("range_min").as_double();
  range_max_ = this->get_parameter("range_max").as_double();
  buffer_size_ = this->get_parameter("buffer_size").as_int();
  detection_threshold_ = this->get_parameter("detection_threshold").as_int();
  spatial_threshold = this->get_parameter("spatial_threshold").as_double();
  this->get_parameter("inner_zone_angle", inner_zone_angle_);
  this->get_parameter("middle_zone_angle", middle_zone_angle_);

  // Setting for segmentation algorithm
  if (params_->segmentation_type == "jump_distance") {
    segmentation_.reset(new JumpDistanceSegmentation);
  } else if (params_->segmentation_type == "jump_distance_merge") {
    segmentation_.reset(new JumpDistanceSegmentationMerge);
  } else {
    RCLCPP_FATAL(
      this->get_logger(), "Segmentation algorithm is invalid: %s]",
      params_->segmentation_type.c_str());
    return CallbackReturn::FAILURE;
  }

  // Publishers
  segment_pub_ = this->create_publisher<slg_msgs::msg::SegmentArray>(seg_topic_, 1);
  segment_viz_points_pub_ = this->create_publisher<visualization_msgs::msg::MarkerArray>(seg_topic_ + "/visualization", 10);
  pointcloud_pub_ = this->create_publisher<sensor_msgs::msg::PointCloud2>(pointcloud_topic_, 10);
  weighted_scan_pub_ = this->create_publisher<sensor_msgs::msg::LaserScan>(weighted_scan_topic_, 10);
  raw_weighted_scan_pub_ = this->create_publisher<sensor_msgs::msg::LaserScan>(weighted_scan_topic_ + "_raw", 10);

  // Subscribers
  auto default_qos = rclcpp::QoS(rclcpp::SystemDefaultsQoS());
  scan_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
    scan_topic_,
    default_qos,
    std::bind(&LaserSegmentation::scan_callback, this, std::placeholders::_1));

  RCLCPP_INFO(this->get_logger(), "Configured laser segmentation node");

  return CallbackReturn::SUCCESS;
}

CallbackReturn LaserSegmentation::on_activate(const rclcpp_lifecycle::State & state)
{
  LifecycleNode::on_activate(state);
  RCLCPP_INFO(this->get_logger(), "Activating the node...");

  segment_pub_->on_activate();
  segment_viz_points_pub_->on_activate();
  pointcloud_pub_->on_activate();
  weighted_scan_pub_->on_activate();
  raw_weighted_scan_pub_->on_activate();
  return CallbackReturn::SUCCESS;
}

CallbackReturn LaserSegmentation::on_deactivate(const rclcpp_lifecycle::State & state)
{
  LifecycleNode::on_deactivate(state);
  RCLCPP_INFO(this->get_logger(), "Deactivating the node...");

  segment_pub_->on_deactivate();
  segment_viz_points_pub_->on_deactivate();
  pointcloud_pub_->on_deactivate();
  weighted_scan_pub_->on_deactivate();
  raw_weighted_scan_pub_->on_deactivate();
  return CallbackReturn::SUCCESS;
}

CallbackReturn LaserSegmentation::on_cleanup(const rclcpp_lifecycle::State & /*state*/)
{
  RCLCPP_INFO(this->get_logger(), "Cleaning the node...");

  // Release the shared pointers
  segment_pub_.reset();
  segment_viz_points_pub_.reset();
  pointcloud_pub_.reset();
  weighted_scan_pub_.reset();
  raw_weighted_scan_pub_.reset();
  param_handler_.reset();

  return CallbackReturn::SUCCESS;
}

CallbackReturn LaserSegmentation::on_shutdown(const rclcpp_lifecycle::State & state)
{
  RCLCPP_INFO(this->get_logger(), "Shutdown the node from state %s.", state.label().c_str());

  // Release the shared pointers
  segment_pub_.reset();
  segment_viz_points_pub_.reset();
  pointcloud_pub_.reset();
  weighted_scan_pub_.reset();
  raw_weighted_scan_pub_.reset();

  return CallbackReturn::SUCCESS;
}

void LaserSegmentation::scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr scan_msg)
{
  std::lock_guard<std::mutex> param_lock(param_handler_->getMutex());

  // Note: Only perform laserscan segmentation if there's any subscriber
  if (segment_pub_->get_subscription_count() == 0 &&
    segment_viz_points_pub_->get_subscription_count() == 0 &&
    pointcloud_pub_->get_subscription_count() == 0 &&
    weighted_scan_pub_->get_subscription_count() == 0 &&
    raw_weighted_scan_pub_->get_subscription_count() == 0)
  {
    return;
  }
  RCLCPP_INFO_ONCE(
    this->get_logger(), "Subscribed to laser scan topic: [%s]", scan_topic_.c_str());

  // Read the laser scan
  std::vector<slg::Point2D> point_list;
  double phi = scan_msg->angle_min;
  double angle_resolution = scan_msg->angle_increment;
  for (const auto r : scan_msg->ranges) {
    if (r >= scan_msg->range_min && r <= scan_msg->range_max) {
      point_list.push_back(slg::Point2D::from_polar_coords(r, phi));
    } else {
      point_list.push_back(slg::Point2D::quiet_NaN());
    }
    phi += angle_resolution;
  }

  // Segment the points
  std::vector<slg::Segment2D> segment_list;
  segmentation_->initialize_segmentation(
    params_->distance_threshold, angle_resolution, params_->noise_reduction,
    params_->method_threshold);
  segmentation_->perform_segmentation(point_list, segment_list);

  // Filter segments
  auto segment_filtered_list = filter_segments(segment_list);

  // Identification of segments and set angular distance
  for (std::vector<slg::Segment2D>::size_type s = 0; s < segment_filtered_list.size(); s++) {
    double angle = std::min(
      scan_msg->angle_max - segment_filtered_list[s].max_angle(),
      segment_filtered_list[s].min_angle() - scan_msg->angle_min);
    segment_filtered_list[s].set_id(s);
    segment_filtered_list[s].set_angular_distance_to_closest_boundary(angle);
  }

  // Publish the segment array
  slg_msgs::msg::SegmentArray segment_array_msg;
  segment_array_msg.header = scan_msg->header;
  for (const auto & segment : segment_filtered_list) {
    segment_array_msg.segments.push_back(segment);
  }
  segment_pub_->publish(segment_array_msg);

  // Convert SegmentArray to PointCloud2
  sensor_msgs::msg::PointCloud2 pointcloud_msg;
  sensor_msgs::msg::PointCloud2 lidar_frame_pointcloud_msg;
  convert_segments_to_pointcloud(segment_array_msg, pointcloud_msg, lidar_frame_pointcloud_msg);

  // Ensure pointcloud fields are set correctly before processing
  if (pointcloud_msg.fields.empty()) {
    //RCLCPP_ERROR(this->get_logger(), "PointCloud2 fields are empty, cannot process pointcloud.");
    return;
  }

  // Add the latest detections to the buffer
  pcl::PointCloud<pcl::PointXYZ> pcl_cloud;
  pcl::fromROSMsg(pointcloud_msg, pcl_cloud);
  std::vector<geometry_msgs::msg::Point> current_detections;
  for (const auto& pcl_point : pcl_cloud.points) {
    geometry_msgs::msg::Point point;
    point.x = pcl_point.x;
    point.y = pcl_point.y;
    point.z = pcl_point.z;
    current_detections.push_back(point);
  }

  // Check for consistent detections
  auto consistent_detections = check_consistent_detection(current_detections);

  sensor_msgs::msg::PointCloud2 consistent_detections_pointcloud;
  // Publish the filtered point cloud using the consistent detections
  consistent_detections_pointcloud = publish_filtered_pointcloud(pcl_cloud, consistent_detections);

  // Convert PointCloud2 to weighted LaserScan
  sensor_msgs::msg::LaserScan laserscan_msg;
  convert_pointcloud_to_weighted_laserscan(consistent_detections_pointcloud, laserscan_msg, scan_msg->header);
  weighted_scan_pub_->publish(laserscan_msg);

  // Publish the raw weighted LaserScan
  raw_weighted_scan_pub_->publish(laserscan_msg);

  // Publish visualization markers
  segment_viz_points_pub_->publish(
    create_segment_viz_points(scan_msg->header, segment_filtered_list));
}


void LaserSegmentation::convert_segments_to_pointcloud(
  const slg_msgs::msg::SegmentArray & segment_array, sensor_msgs::msg::PointCloud2 & pointcloud_msg, sensor_msgs::msg::PointCloud2 & lidar_frame_pointcloud_msg)
{
  pcl::PointCloud<pcl::PointXYZ> cloud;


  for (const auto & segment : segment_array.segments) {
    for (const auto & point : segment.points) {
      // Ensure points are valid before adding them
      if (std::isfinite(point.x) && std::isfinite(point.y) && std::isfinite(point.z)) {
        cloud.push_back(pcl::PointXYZ(point.x, point.y, 0.0));
      } else {
        //RCLCPP_WARN(this->get_logger(), "Invalid point encountered: (%f, %f, %f)", point.x, point.y, point.z);
      }
    }
  }

  // Ensure that the pointcloud is not empty before converting
  if (cloud.empty()) {
    //RCLCPP_WARN(this->get_logger(), "Empty pointcloud generated from segments");
    return;
  }

  pcl::toROSMsg(cloud, lidar_frame_pointcloud_msg);
  lidar_frame_pointcloud_msg.header = segment_array.header;

  // Transform the pointcloud to the fixed frame FP_ENU0
  pcl::PointCloud<pcl::PointXYZ> transformed_cloud;
  try {
    geometry_msgs::msg::TransformStamped transform_stamped = tf_buffer_->lookupTransform("FP_ENU0", lidar_frame_pointcloud_msg.header.frame_id, tf2::TimePointZero);
    pcl_ros::transformPointCloud(cloud, transformed_cloud, transform_stamped);
    pcl::toROSMsg(transformed_cloud, pointcloud_msg);
    pointcloud_msg.header.frame_id = "FP_ENU0";
  } catch (tf2::TransformException & ex) {
    //RCLCPP_WARN(this->get_logger(), "Could not transform pointcloud to FP_ENU0: %s", ex.what());
    pcl::toROSMsg(cloud, pointcloud_msg);  // Publish original pointcloud if transformation fails
    pointcloud_msg.header = segment_array.header;
  }

  //RCLCPP_DEBUG(this->get_logger(), "PointCloud conversion complete");
}




void LaserSegmentation::convert_pointcloud_to_weighted_laserscan(
  const sensor_msgs::msg::PointCloud2 & lidar_frame_pointcloud_msg, sensor_msgs::msg::LaserScan & laserscan_msg, const std_msgs::msg::Header & header)
{
  pcl::PointCloud<pcl::PointXYZ> cloud;
  pcl::fromROSMsg(lidar_frame_pointcloud_msg, cloud);

  double angle_min = -M_PI / 2;
  double angle_max = M_PI / 2;

  int num_readings = static_cast<int>((angle_max - angle_min) / angle_increment_);
  std::vector<float> ranges(num_readings, std::numeric_limits<float>::infinity());

  double inner_zone_rad = inner_zone_angle_ * M_PI / 180.0;
  double middle_zone_rad = middle_zone_angle_ * M_PI / 180.0;

  double zone1_min = -M_PI / 2;
  double zone1_max = -middle_zone_rad;
  double zone2_min = -middle_zone_rad;
  double zone2_max = -inner_zone_rad;
  double zone3_min = -inner_zone_rad;
  double zone3_max = inner_zone_rad;
  double zone4_min = inner_zone_rad;
  double zone4_max = middle_zone_rad;
  double zone5_min = middle_zone_rad;
  double zone5_max = M_PI / 2;

  for (const auto & point : cloud.points) {
    float distance = std::sqrt(point.x * point.x + point.y * point.y);
    float angle = std::atan2(point.y, point.x);

    if (angle < angle_min || angle > angle_max || distance < range_min_ || distance > range_max_) {
      continue;
    }

    int index = static_cast<int>((angle - angle_min) / angle_increment_);
    if (index < 0 || index >= num_readings) {
      //RCLCPP_WARN(this->get_logger(), "Index out of bounds: %d", index);
      continue;
    }

    if ((zone1_min <= angle && angle < zone1_max) || (zone5_min <= angle && angle <= zone5_max)) {
      // Ignore the beam
      continue;
    } else if ((zone2_min <= angle && angle < zone2_max) || (zone4_min <= angle && angle < zone4_max)) {
      // Apply symmetrical weighting
      float weight = 1 - weight_factor_ * std::cos(std::abs(angle) - inner_zone_rad);
      if (weight <= 0) {
        weight = std::numeric_limits<float>::min();
      }
      float weighted_distance = distance / weight;

      if (std::isinf(weighted_distance) || std::isnan(weighted_distance)) {
        //RCLCPP_WARN(this->get_logger(), "Invalid weighted distance: %f", weighted_distance);
        continue;
      }

      if (weighted_distance < ranges[index]) {
        ranges[index] = weighted_distance;
      }
    } else if (zone3_min <= angle && angle < zone3_max) {
      // No weighting
      if (distance < ranges[index]) {
        ranges[index] = distance;
      }
    }
  }

  // Create LaserScan message
  laserscan_msg.header = header;
  laserscan_msg.angle_min = angle_min;
  laserscan_msg.angle_max = angle_max;
  laserscan_msg.angle_increment = angle_increment_;
  laserscan_msg.time_increment = 0.0;
  laserscan_msg.scan_time = 0.0;
  laserscan_msg.range_min = range_min_;
  laserscan_msg.range_max = range_max_;
  laserscan_msg.ranges = ranges;
}



std::vector<slg::Segment2D> LaserSegmentation::filter_segments(
  const std::vector<slg::Segment2D> & segments)
{
  std::vector<slg::Segment2D> filtered_segments;
  filtered_segments.reserve(segments.size());

  double squared_min_segment_width = params_->min_segment_width * params_->min_segment_width;
  double squared_max_segment_width = params_->max_segment_width * params_->max_segment_width;

  for (const auto & segment : segments) {
    // By number of points
    if (segment.size() < params_->min_points_segment ||
      segment.size() > params_->max_points_segment)
    {
      continue;
    }

    // By distance to sensor
    if (segment.centroid().length() < params_->min_avg_distance_from_sensor ||
      segment.centroid().length() > params_->max_avg_distance_from_sensor)
    {
      continue;
    }

    // By width
    if (segment.width_squared() < squared_min_segment_width ||
      segment.width_squared() > squared_max_segment_width)
    {
      continue;
    }

    filtered_segments.push_back(segment);
  }
  return filtered_segments;
}

visualization_msgs::msg::MarkerArray LaserSegmentation::create_segment_viz_points(
  std_msgs::msg::Header header,
  std::vector<slg::Segment2D> segment_list)
{
  // Create the visualization message
  visualization_msgs::msg::MarkerArray viz_array;

  // Create a deletion marker to clear the previous points
  visualization_msgs::msg::Marker deletion_marker;
  deletion_marker.header = header;
  deletion_marker.action = visualization_msgs::msg::Marker::DELETEALL;
  viz_array.markers.push_back(deletion_marker);

  // Create a marker point
  visualization_msgs::msg::Marker viz_points;
  viz_points.header = header;
  viz_points.lifetime = rclcpp::Duration(0, 10);
  viz_points.ns = "segments";
  viz_points.type = visualization_msgs::msg::Marker::POINTS;
  viz_points.action = visualization_msgs::msg::Marker::ADD;
  viz_points.scale.x = viz_points.scale.y = 0.02;

  // Create a marker centroid
  visualization_msgs::msg::Marker viz_centroids;
  viz_centroids.header = header;
  viz_centroids.lifetime = rclcpp::Duration(0, 10);
  viz_centroids.ns = "centroids";
  viz_centroids.type = visualization_msgs::msg::Marker::CUBE;
  viz_centroids.action = visualization_msgs::msg::Marker::ADD;
  viz_centroids.scale.x = viz_centroids.scale.y = viz_centroids.scale.z = 0.05;

  // Create a marker id text
  visualization_msgs::msg::Marker viz_text;
  viz_text.header = header;
  viz_text.lifetime = rclcpp::Duration(0, 10);
  viz_text.ns = "id";
  viz_text.type = visualization_msgs::msg::Marker::TEXT_VIEW_FACING;
  viz_text.action = visualization_msgs::msg::Marker::ADD;
  viz_text.scale.z = 0.25;
  viz_text.color.r = 1.0;
  viz_text.color.g = 1.0;
  viz_text.color.b = 1.0;
  viz_text.color.a = 1.0;

  // Show the segments and the id
  for (std::vector<slg::Segment2D>::size_type i = 0; i < segment_list.size(); i++) {
    viz_points.id = i;
    viz_text.id = i;
    viz_centroids.id = i;

    // Change the color of the segment
    viz_points.color = get_palette_color(i);
    viz_centroids.color = get_palette_color(i);

    // Iterate over the points of the segment
    slg::Segment2D current_segment = segment_list[i];
    for (const auto & point : current_segment.get_points()) {
      viz_points.points.push_back(point);
    }

    // Get position of the text
    viz_text.text = std::to_string(current_segment.get_id());
    viz_text.pose.position = current_segment.centroid();
    viz_text.pose.position.z = 0.10;

    // Place centroid under text
    viz_centroids.pose.position = current_segment.centroid();
    viz_centroids.pose.position.z = 0.0;

    // Push to arrays
    viz_array.markers.push_back(viz_points);
    viz_array.markers.push_back(viz_centroids);
    viz_array.markers.push_back(viz_text);

    // Clear markers
    viz_points.points.clear();
  }

  return viz_array;
}



std_msgs::msg::ColorRGBA LaserSegmentation::get_parula_color(unsigned int index, unsigned int max)
{
  std_msgs::msg::ColorRGBA color;
  int div = round(256 / max);
  color.r = parula[index * div][0];
  color.g = parula[index * div][1];
  color.b = parula[index * div][2];
  color.a = 1.0;
  return color;
}

std_msgs::msg::ColorRGBA LaserSegmentation::get_palette_color(unsigned int index)
{
  std_msgs::msg::ColorRGBA color;
  switch (index % 8) {
    case 0: color.r = 255; color.g = 051; color.b = 051; break;
    case 2: color.r = 255; color.g = 153; color.b = 051; break;
    case 4: color.r = 255; color.g = 255; color.b = 051; break;
    case 6: color.r = 153; color.g = 051; color.b = 051; break;
    case 1: color.r = 051; color.g = 255; color.b = 051; break;
    case 3: color.r = 051; color.g = 255; color.b = 153; break;
    case 5: color.r = 051; color.g = 153; color.b = 255; break;
    case 7: color.r = 255; color.g = 051; color.b = 255; break;
  }
  color.a = 1.0;
  return color;
}

std::vector<geometry_msgs::msg::Point> LaserSegmentation::check_consistent_detection(const std::vector<geometry_msgs::msg::Point>& current_detections)
{


  std::vector<geometry_msgs::msg::Point> consistent_detections;

  for (const auto& current_detection : current_detections) {
    int count = 0;
    for (const auto& scan : detection_history_) {
      for (const auto& detection : scan) {
        double distance = std::sqrt(
          std::pow(current_detection.x - detection.x, 2) +
          std::pow(current_detection.y - detection.y, 2) +
          std::pow(current_detection.z - detection.z, 2));
        


        if (distance < spatial_threshold) {
          count++;
          break;
        }
      }
    }


 
    if (count >= static_cast<int>(detection_threshold_)) {
      consistent_detections.push_back(current_detection);
    }
   }

   /*If too few consistent detections are found, fallback to using the latest scan directly
  if (consistent_detections.size() < current_detections.size() / 2) {
    RCLCPP_INFO(this->get_logger(), "Fallback to latest scan detections");
    consistent_detections = current_detections;
  }*/


  // Add the latest detections to the buffer after the consistency check
  detection_history_.push_back(current_detections);

  // Check if the detection history buffer is full
  if (detection_history_.size() >= static_cast<std::size_t>(buffer_size_)) {
    detection_history_.pop_front();
  }

  return consistent_detections;
}


sensor_msgs::msg::PointCloud2 LaserSegmentation::publish_filtered_pointcloud(const pcl::PointCloud<pcl::PointXYZ>& pcl_cloud, const std::vector<geometry_msgs::msg::Point>& consistent_detections)
{
  // Filter the fixed frame point cloud using the consistent detections
  pcl::PointCloud<pcl::PointXYZ> consistent_cloud;
  for (const auto& pcl_point : pcl_cloud.points) {
    for (const auto& consistent_detection : consistent_detections) {
      if (std::sqrt(std::pow(pcl_point.x - consistent_detection.x, 2) +
                    std::pow(pcl_point.y - consistent_detection.y, 2) +
                    std::pow(pcl_point.z - consistent_detection.z, 2)) < spatial_threshold) {
        consistent_cloud.push_back(pcl_point);
        break;
      }
    }
  }
   sensor_msgs::msg::PointCloud2 pointcloud_msg;

  if (consistent_cloud.empty()) {
    //RCLCPP_WARN(this->get_logger(), "Empty consistent point cloud, not publishing.");
    return pointcloud_msg;
  }

  // Transform the consistent cloud back to the LiDAR frame
  pcl::PointCloud<pcl::PointXYZ> lidar_frame_cloud;
  try {
    geometry_msgs::msg::TransformStamped transform_stamped = tf_buffer_->lookupTransform("sick_link", "FP_ENU0", tf2::TimePointZero);
    pcl_ros::transformPointCloud(consistent_cloud, lidar_frame_cloud, transform_stamped);
  } catch (tf2::TransformException& ex) {
    //RCLCPP_WARN(this->get_logger(), "Transform to LiDAR frame failed: %s", ex.what());

    return pointcloud_msg;
  }

  // Publish the transformed point cloud
  pcl::toROSMsg(lidar_frame_cloud, pointcloud_msg);
  pointcloud_msg.header.frame_id = "sick_link";
  pointcloud_pub_->publish(pointcloud_msg);
  return pointcloud_msg;

}



}  // namespace laser_segmentation

#include "rclcpp_components/register_node_macro.hpp"

RCLCPP_COMPONENTS_REGISTER_NODE(laser_segmentation::LaserSegmentation)

