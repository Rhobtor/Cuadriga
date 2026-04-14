#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <sensor_msgs/msg/point_field.hpp>

#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <tf2/LinearMath/Transform.h>
#include <tf2/LinearMath/Vector3.h>

#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

using std::placeholders::_1;

class VelodyneNoiseFilterNode : public rclcpp::Node
{
public:
  VelodyneNoiseFilterNode()
  : Node("velodyne_noise_filter")
  {
    input_topic_ = this->declare_parameter<std::string>("input", "/ARGJ801/Velodyne/scan_cloud");
    output_topic_ = this->declare_parameter<std::string>("output", "/ARGJ801/Velodyne/scan_cloud_filtered");
    output_frame_id_ = this->declare_parameter<std::string>("output_frame_id", "");

    leaf_size_ = this->declare_parameter<double>("leaf_size", 0.15);
    min_points_per_voxel_ = this->declare_parameter<int>("min_points_per_voxel", 1);

    support_leaf_size_ = this->declare_parameter<double>("support_leaf_size", 0.35);
    support_radius_voxels_ = this->declare_parameter<int>("support_radius_voxels", 1);
    min_support_points_ = this->declare_parameter<int>("min_support_points", 3);

    min_range_ = this->declare_parameter<double>("min_range", 1.0);
    max_range_ = this->declare_parameter<double>("max_range", 60.0);
    min_z_ = this->declare_parameter<double>("min_z", -3.0);
    max_z_ = this->declare_parameter<double>("max_z", 3.0);

    reject_origin_enabled_ = this->declare_parameter<bool>("reject_origin_enabled", true);
    reject_origin_epsilon_ = this->declare_parameter<double>("reject_origin_epsilon", 0.05);

    tf_timeout_sec_ = this->declare_parameter<double>("tf_timeout_sec", 0.05);

    exclude_enabled_ = this->declare_parameter<bool>("exclude_box_enabled", true);
    exclude_frame_ = this->declare_parameter<std::string>("exclude_box_frame", "base_link");
    ex_cx_ = this->declare_parameter<double>("exclude_box_center_x", 0.35);
    ex_cy_ = this->declare_parameter<double>("exclude_box_center_y", 0.00);
    ex_cz_ = this->declare_parameter<double>("exclude_box_center_z", 0.35);
    ex_sx_ = this->declare_parameter<double>("exclude_box_size_x", 3.10);
    ex_sy_ = this->declare_parameter<double>("exclude_box_size_y", 1.80);
    ex_sz_ = this->declare_parameter<double>("exclude_box_size_z", 1.40);

    hood_enabled_ = this->declare_parameter<bool>("hood_box_enabled", true);
    hx_ = this->declare_parameter<double>("hood_box_center_x", 1.00);
    hy_ = this->declare_parameter<double>("hood_box_center_y", 0.00);
    hz_ = this->declare_parameter<double>("hood_box_center_z", 0.20);
    hsx_ = this->declare_parameter<double>("hood_box_size_x", 1.40);
    hsy_ = this->declare_parameter<double>("hood_box_size_y", 1.20);
    hsz_ = this->declare_parameter<double>("hood_box_size_z", 1.10);

    pass_through_if_no_tf_ = this->declare_parameter<bool>("pass_through_if_no_tf", true);
    debug_log_ = this->declare_parameter<bool>("debug_log", true);

    if (leaf_size_ <= 0.0) {
      RCLCPP_WARN(this->get_logger(), "leaf_size <= 0. Se fuerza a 0.15");
      leaf_size_ = 0.15;
    }
    if (support_leaf_size_ <= 0.0) {
      RCLCPP_WARN(this->get_logger(), "support_leaf_size <= 0. Se fuerza a 0.35");
      support_leaf_size_ = 0.35;
    }
    if (support_radius_voxels_ < 0) {
      RCLCPP_WARN(this->get_logger(), "support_radius_voxels < 0. Se fuerza a 0");
      support_radius_voxels_ = 0;
    }
    if (min_support_points_ < 1) {
      RCLCPP_WARN(this->get_logger(), "min_support_points < 1. Se fuerza a 1");
      min_support_points_ = 1;
    }
    if (min_points_per_voxel_ < 1) {
      RCLCPP_WARN(this->get_logger(), "min_points_per_voxel < 1. Se fuerza a 1");
      min_points_per_voxel_ = 1;
    }
    if (max_range_ <= min_range_) {
      RCLCPP_WARN(this->get_logger(), "max_range <= min_range. Ajustando max_range=min_range+1.0");
      max_range_ = min_range_ + 1.0;
    }
    if (reject_origin_epsilon_ < 0.0) {
      RCLCPP_WARN(this->get_logger(), "reject_origin_epsilon < 0. Se fuerza a 0.05");
      reject_origin_epsilon_ = 0.05;
    }

    pub_ = this->create_publisher<sensor_msgs::msg::PointCloud2>(output_topic_, rclcpp::SensorDataQoS());
    sub_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(
      input_topic_, rclcpp::SensorDataQoS(), std::bind(&VelodyneNoiseFilterNode::cloudCallback, this, _1));

    tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

    RCLCPP_INFO(
      this->get_logger(),
      "VelodyneNoiseFilterNode iniciado.\n"
      "  input: %s\n"
      "  output: %s\n"
      "  leaf_size: %.3f | min_pts_voxel: %d\n"
      "  support_leaf: %.3f | support_radius: %d | min_support_points: %d\n"
      "  range: [%.2f, %.2f] | z: [%.2f, %.2f]\n"
      "  reject_origin: %s (eps=%.3f)\n"
      "  exclude_box: %s (frame=%s)\n"
      "  hood_box: %s (frame=%s)",
      input_topic_.c_str(), output_topic_.c_str(),
      leaf_size_, min_points_per_voxel_,
      support_leaf_size_, support_radius_voxels_, min_support_points_,
      min_range_, max_range_, min_z_, max_z_,
      reject_origin_enabled_ ? "ON" : "OFF", reject_origin_epsilon_,
      exclude_enabled_ ? "ON" : "OFF", exclude_frame_.c_str(),
      hood_enabled_ ? "ON" : "OFF", exclude_frame_.c_str());
  }

private:
  struct KeyHasher {
    std::size_t operator()(const std::array<int, 3> & key) const noexcept
    {
      const std::size_t h1 = static_cast<std::size_t>(key[0]) * 73856093u;
      const std::size_t h2 = static_cast<std::size_t>(key[1]) * 19349663u;
      const std::size_t h3 = static_cast<std::size_t>(key[2]) * 83492791u;
      return h1 ^ h2 ^ h3;
    }
  };

  struct KeyEqual {
    bool operator()(const std::array<int, 3> & lhs, const std::array<int, 3> & rhs) const noexcept
    {
      return lhs[0] == rhs[0] && lhs[1] == rhs[1] && lhs[2] == rhs[2];
    }
  };

  struct Accum {
    float sx{0.0f};
    float sy{0.0f};
    float sz{0.0f};
    int count{0};
  };

  using CountMap = std::unordered_map<std::array<int, 3>, int, KeyHasher, KeyEqual>;
  using VoxelMap = std::unordered_map<std::array<int, 3>, Accum, KeyHasher, KeyEqual>;

  static float readFloat(const uint8_t * base, std::size_t offset)
  {
    float value;
    std::memcpy(&value, base + offset, sizeof(float));
    return value;
  }

  inline std::array<int, 3> makeKey(float x, float y, float z, float inv_leaf) const
  {
    return {{
      static_cast<int>(std::floor(x * inv_leaf)),
      static_cast<int>(std::floor(y * inv_leaf)),
      static_cast<int>(std::floor(z * inv_leaf))
    }};
  }

  inline bool insideBox(
    const tf2::Vector3 & point,
    double cx, double cy, double cz,
    double sx, double sy, double sz) const
  {
    return std::fabs(point.x() - cx) <= (sx * 0.5) &&
           std::fabs(point.y() - cy) <= (sy * 0.5) &&
           std::fabs(point.z() - cz) <= (sz * 0.5);
  }

  int supportCountForKey(const CountMap & support_counts, const std::array<int, 3> & key) const
  {
    int support = 0;
    for (int dx = -support_radius_voxels_; dx <= support_radius_voxels_; ++dx) {
      for (int dy = -support_radius_voxels_; dy <= support_radius_voxels_; ++dy) {
        for (int dz = -support_radius_voxels_; dz <= support_radius_voxels_; ++dz) {
          const std::array<int, 3> neighbor{{key[0] + dx, key[1] + dy, key[2] + dz}};
          const auto it = support_counts.find(neighbor);
          if (it != support_counts.end()) {
            support += it->second;
          }
        }
      }
    }
    return support;
  }

  void cloudCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg)
  {
    const std::size_t npts = static_cast<std::size_t>(msg->width) * static_cast<std::size_t>(msg->height);
    if (npts == 0) {
      return;
    }

    if (msg->is_bigendian) {
      RCLCPP_ERROR_THROTTLE(this->get_logger(), *this->get_clock(), 2000, "PointCloud big-endian no soportada.");
      return;
    }

    if (msg->point_step == 0 || msg->data.empty()) {
      RCLCPP_WARN_THROTTLE(this->get_logger(), *this->get_clock(), 2000, "PointCloud vacia o point_step=0.");
      return;
    }

    const std::size_t expected_size = static_cast<std::size_t>(msg->point_step) * npts;
    if (msg->data.size() < expected_size) {
      RCLCPP_WARN_THROTTLE(
        this->get_logger(), *this->get_clock(), 2000,
        "Tamano de data invalido: data=%zu, esperado=%zu", msg->data.size(), expected_size);
      return;
    }

    const sensor_msgs::msg::PointField * fx = nullptr;
    const sensor_msgs::msg::PointField * fy = nullptr;
    const sensor_msgs::msg::PointField * fz = nullptr;

    for (const auto & field : msg->fields) {
      if (field.name == "x") {
        fx = &field;
      } else if (field.name == "y") {
        fy = &field;
      } else if (field.name == "z") {
        fz = &field;
      }
    }

    if (!fx || !fy || !fz) {
      RCLCPP_ERROR_THROTTLE(this->get_logger(), *this->get_clock(), 2000, "La nube no tiene campos x/y/z.");
      return;
    }

    if (fx->datatype != sensor_msgs::msg::PointField::FLOAT32 ||
        fy->datatype != sensor_msgs::msg::PointField::FLOAT32 ||
        fz->datatype != sensor_msgs::msg::PointField::FLOAT32)
    {
      RCLCPP_ERROR_THROTTLE(this->get_logger(), *this->get_clock(), 2000, "x/y/z deben ser FLOAT32.");
      return;
    }

    if ((fx->offset + 4) > msg->point_step ||
        (fy->offset + 4) > msg->point_step ||
        (fz->offset + 4) > msg->point_step)
    {
      RCLCPP_ERROR_THROTTLE(
        this->get_logger(), *this->get_clock(), 2000,
        "Offsets x/y/z invalidos para point_step=%u", msg->point_step);
      return;
    }

    tf2::Transform cloud_to_box_tf;
    bool use_box_tf = false;
    const bool want_boxes = exclude_enabled_ || hood_enabled_;

    if (want_boxes) {
      if (exclude_frame_ == msg->header.frame_id) {
        cloud_to_box_tf.setIdentity();
        use_box_tf = true;
      } else {
        try {
          const auto transform = tf_buffer_->lookupTransform(
            exclude_frame_,
            msg->header.frame_id,
            rclcpp::Time(msg->header.stamp),
            rclcpp::Duration::from_seconds(tf_timeout_sec_));
          tf2::fromMsg(transform.transform, cloud_to_box_tf);
          use_box_tf = true;
        } catch (const tf2::TransformException & ex) {
          RCLCPP_WARN_THROTTLE(
            this->get_logger(), *this->get_clock(), 3000,
            "No TF %s <- %s @t=%.3f. %s",
            exclude_frame_.c_str(),
            msg->header.frame_id.c_str(),
            rclcpp::Time(msg->header.stamp).seconds(),
            ex.what());
          use_box_tf = false;
        }
      }
    }

    if (want_boxes && !use_box_tf && !pass_through_if_no_tf_) {
      return;
    }

    const float min_r2 = static_cast<float>(min_range_ * min_range_);
    const float max_r2 = static_cast<float>(max_range_ * max_range_);
    const float min_zf = static_cast<float>(min_z_);
    const float max_zf = static_cast<float>(max_z_);
    const float reject_origin_eps = static_cast<float>(reject_origin_epsilon_);
    const float inv_support_leaf = 1.0f / static_cast<float>(support_leaf_size_);
    const float inv_leaf = 1.0f / static_cast<float>(leaf_size_);

    const uint8_t * data = msg->data.data();
    const std::size_t step = static_cast<std::size_t>(msg->point_step);

    std::vector<std::array<float, 3>> candidates;
    candidates.reserve(npts);

    CountMap support_counts;
    support_counts.reserve(npts / 2 + 1);

    std::size_t nan_rejected = 0;
    std::size_t origin_rejected = 0;
    std::size_t range_rejected = 0;
    std::size_t z_rejected = 0;
    std::size_t box_rejected = 0;

    for (std::size_t i = 0; i < npts; ++i) {
      const uint8_t * base = data + i * step;
      const float x = readFloat(base, fx->offset);
      const float y = readFloat(base, fy->offset);
      const float z = readFloat(base, fz->offset);

      if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z)) {
        ++nan_rejected;
        continue;
      }

      if (reject_origin_enabled_ &&
          std::fabs(x) <= reject_origin_eps &&
          std::fabs(y) <= reject_origin_eps &&
          std::fabs(z) <= reject_origin_eps)
      {
        ++origin_rejected;
        continue;
      }

      const float r2 = x * x + y * y + z * z;
      if (r2 < min_r2 || r2 > max_r2) {
        ++range_rejected;
        continue;
      }

      if (z < min_zf || z > max_zf) {
        ++z_rejected;
        continue;
      }

      if (want_boxes && use_box_tf) {
        const tf2::Vector3 point_in_box = cloud_to_box_tf * tf2::Vector3(x, y, z);
        const bool in_main_box = exclude_enabled_ &&
          insideBox(point_in_box, ex_cx_, ex_cy_, ex_cz_, ex_sx_, ex_sy_, ex_sz_);
        const bool in_hood_box = hood_enabled_ &&
          insideBox(point_in_box, hx_, hy_, hz_, hsx_, hsy_, hsz_);
        if (in_main_box || in_hood_box) {
          ++box_rejected;
          continue;
        }
      }

      candidates.push_back({{x, y, z}});
      ++support_counts[makeKey(x, y, z, inv_support_leaf)];
    }

    if (candidates.empty()) {
      return;
    }

    VoxelMap out_voxels;
    out_voxels.reserve(candidates.size() / 2 + 1);

    std::size_t support_rejected = 0;
    for (const auto & point : candidates) {
      const auto support_key = makeKey(point[0], point[1], point[2], inv_support_leaf);
      if (supportCountForKey(support_counts, support_key) < min_support_points_) {
        ++support_rejected;
        continue;
      }

      auto & accum = out_voxels[makeKey(point[0], point[1], point[2], inv_leaf)];
      accum.sx += point[0];
      accum.sy += point[1];
      accum.sz += point[2];
      accum.count += 1;
    }

    if (out_voxels.empty()) {
      return;
    }

    std::vector<float> out_xyz;
    out_xyz.reserve(out_voxels.size() * 3);

    std::size_t density_rejected = 0;
    for (const auto & entry : out_voxels) {
      const Accum & accum = entry.second;
      if (accum.count < min_points_per_voxel_) {
        ++density_rejected;
        continue;
      }

      const float inv_count = 1.0f / static_cast<float>(accum.count);
      out_xyz.push_back(accum.sx * inv_count);
      out_xyz.push_back(accum.sy * inv_count);
      out_xyz.push_back(accum.sz * inv_count);
    }

    if (out_xyz.empty()) {
      return;
    }

    sensor_msgs::msg::PointCloud2 out;
    out.header.stamp = msg->header.stamp;
    out.header.frame_id = output_frame_id_.empty() ? msg->header.frame_id : output_frame_id_;
    out.height = 1;
    out.width = static_cast<uint32_t>(out_xyz.size() / 3);
    out.is_bigendian = false;
    out.is_dense = true;
    out.point_step = 12;
    out.row_step = out.point_step * out.width;

    out.fields.resize(3);
    out.fields[0].name = "x";
    out.fields[0].offset = 0;
    out.fields[0].datatype = sensor_msgs::msg::PointField::FLOAT32;
    out.fields[0].count = 1;
    out.fields[1].name = "y";
    out.fields[1].offset = 4;
    out.fields[1].datatype = sensor_msgs::msg::PointField::FLOAT32;
    out.fields[1].count = 1;
    out.fields[2].name = "z";
    out.fields[2].offset = 8;
    out.fields[2].datatype = sensor_msgs::msg::PointField::FLOAT32;
    out.fields[2].count = 1;

    out.data.resize(static_cast<std::size_t>(out.row_step) * out.height);
    std::memcpy(out.data.data(), out_xyz.data(), out_xyz.size() * sizeof(float));

    pub_->publish(out);

    if (debug_log_) {
      RCLCPP_INFO_THROTTLE(
        this->get_logger(), *this->get_clock(), 2000,
        "in=%zu | candidates=%zu | support_cells=%zu | out_voxels=%zu | out=%u | rej[nan=%zu,origin=%zu,range=%zu,z=%zu,box=%zu,support=%zu,density=%zu]",
        npts, candidates.size(), support_counts.size(), out_voxels.size(), out.width,
        nan_rejected, origin_rejected, range_rejected, z_rejected, box_rejected, support_rejected, density_rejected);
    }
  }

  std::string input_topic_;
  std::string output_topic_;
  std::string output_frame_id_;

  double leaf_size_;
  int min_points_per_voxel_;

  double support_leaf_size_;
  int support_radius_voxels_;
  int min_support_points_;

  double min_range_;
  double max_range_;
  double min_z_;
  double max_z_;

  bool reject_origin_enabled_;
  double reject_origin_epsilon_;

  double tf_timeout_sec_;

  bool exclude_enabled_;
  std::string exclude_frame_;
  double ex_cx_;
  double ex_cy_;
  double ex_cz_;
  double ex_sx_;
  double ex_sy_;
  double ex_sz_;

  bool hood_enabled_;
  double hx_;
  double hy_;
  double hz_;
  double hsx_;
  double hsy_;
  double hsz_;

  bool pass_through_if_no_tf_;
  bool debug_log_;

  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr sub_;
  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pub_;

  std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<VelodyneNoiseFilterNode>());
  rclcpp::shutdown();
  return 0;
}