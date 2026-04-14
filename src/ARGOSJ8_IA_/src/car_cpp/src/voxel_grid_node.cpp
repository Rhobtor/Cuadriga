#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <sensor_msgs/msg/point_field.hpp>

#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <tf2/LinearMath/Transform.h>
#include <tf2/LinearMath/Vector3.h>

#include <unordered_map>
#include <array>
#include <vector>
#include <string>
#include <cmath>
#include <cstring>
#include <limits>
#include <cstdint>

using std::placeholders::_1;

class VoxelGridNode : public rclcpp::Node
{
public:
  VoxelGridNode()
  : Node("voxel_grid_cpp_self_filter")
  {
    // ---------- IO ----------
    input_topic_ = this->declare_parameter<std::string>(
     "input", "/zed/zed_node/point_cloud/cloud_registered");
     //input_topic_ = this->declare_parameter<std::string>(
      // "input", "/camera/points");
    output_topic_ = this->declare_parameter<std::string>(
      "output", "/merged_points_filtered");

    // OJO: si lo dejas vacío, conserva frame de entrada (recomendado)
    output_frame_id_ = this->declare_parameter<std::string>(
      "output_frame_id", "");

    // ---------- Voxel ----------
    leaf_size_ = this->declare_parameter<double>("leaf_size", 0.10);
    min_points_per_voxel_ = this->declare_parameter<int>("min_points_per_voxel", 3);

    // ---------- Filtros geométricos globales ----------
    min_range_ = this->declare_parameter<double>("min_range", 0.5);
    max_range_ = this->declare_parameter<double>("max_range", 20.0);
    min_z_     = this->declare_parameter<double>("min_z", -5.0);
    max_z_     = this->declare_parameter<double>("max_z",  2.0);

    // ---------- TF ----------
    tf_timeout_sec_ = this->declare_parameter<double>("tf_timeout_sec", 0.05);

    // ---------- Caja principal de exclusión (robot/chasis general) ----------
    exclude_enabled_ = this->declare_parameter<bool>("exclude_box_enabled", true);
    exclude_frame_   = this->declare_parameter<std::string>("exclude_box_frame", "base_link");

    ex_cx_ = this->declare_parameter<double>("exclude_box_center_x", 0.35);
    ex_cy_ = this->declare_parameter<double>("exclude_box_center_y", 0.00);
    ex_cz_ = this->declare_parameter<double>("exclude_box_center_z", 0.35);
    ex_sx_ = this->declare_parameter<double>("exclude_box_size_x",   3.10);
    ex_sy_ = this->declare_parameter<double>("exclude_box_size_y",   1.80);
    ex_sz_ = this->declare_parameter<double>("exclude_box_size_z",   1.40);

    // ---------- Caja secundaria específica para capó ----------
    hood_enabled_ = this->declare_parameter<bool>("hood_box_enabled", true);

    // Ajusta estos valores a tu robot
    hx_  = this->declare_parameter<double>("hood_box_center_x", 1.00);
    hy_  = this->declare_parameter<double>("hood_box_center_y", 0.00);
    hz_  = this->declare_parameter<double>("hood_box_center_z", 0.20);
    hsx_ = this->declare_parameter<double>("hood_box_size_x",   1.40);
    hsy_ = this->declare_parameter<double>("hood_box_size_y",   1.20);
    hsz_ = this->declare_parameter<double>("hood_box_size_z",   1.10);

    // Si no hay TF, ¿pasar nube sin self-filter?
    pass_through_if_no_tf_ = this->declare_parameter<bool>("pass_through_if_no_tf", true);

    // Debug
    debug_log_ = this->declare_parameter<bool>("debug_log", true);

    if (leaf_size_ <= 0.0) {
      RCLCPP_WARN(this->get_logger(), "leaf_size <= 0. Se fuerza a 0.10");
      leaf_size_ = 0.10;
    }
    if (max_range_ <= min_range_) {
      RCLCPP_WARN(this->get_logger(), "max_range <= min_range. Ajustando max_range=min_range+0.1");
      max_range_ = min_range_ + 0.1;
    }

    if (!output_frame_id_.empty()) {
      RCLCPP_WARN(
        this->get_logger(),
        "output_frame_id='%s'. OJO: este nodo NO transforma puntos a ese frame, "
        "solo cambia el header.frame_id. Déjalo vacío salvo que sepas lo que haces.",
        output_frame_id_.c_str()
      );
    }

    pub_ = this->create_publisher<sensor_msgs::msg::PointCloud2>(output_topic_, rclcpp::SensorDataQoS());

    sub_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(
      input_topic_, rclcpp::SensorDataQoS(),
      std::bind(&VoxelGridNode::cloudCallback, this, _1));

    tf_buffer_   = std::make_shared<tf2_ros::Buffer>(this->get_clock());
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

    RCLCPP_INFO(
      this->get_logger(),
      "VoxelGridNode iniciado.\n"
      "  input: %s\n"
      "  output: %s\n"
      "  leaf_size: %.3f | min_pts_voxel: %d\n"
      "  range: [%.2f, %.2f] | z: [%.2f, %.2f]\n"
      "  exclude_box: %s (frame=%s)\n"
      "  hood_box: %s (frame=%s)\n"
      "  tf_timeout: %.3f s",
      input_topic_.c_str(), output_topic_.c_str(),
      leaf_size_, min_points_per_voxel_,
      min_range_, max_range_, min_z_, max_z_,
      exclude_enabled_ ? "ON" : "OFF", exclude_frame_.c_str(),
      hood_enabled_ ? "ON" : "OFF", exclude_frame_.c_str(),
      tf_timeout_sec_
    );
  }

private:
  struct KeyHasher {
    std::size_t operator()(const std::array<int,3>& k) const noexcept {
      // hash sencillo para índices de voxel
      const std::size_t h1 = static_cast<std::size_t>(k[0]) * 73856093u;
      const std::size_t h2 = static_cast<std::size_t>(k[1]) * 19349663u;
      const std::size_t h3 = static_cast<std::size_t>(k[2]) * 83492791u;
      return h1 ^ h2 ^ h3;
    }
  };

  struct KeyEqual {
    bool operator()(const std::array<int,3>& a, const std::array<int,3>& b) const noexcept {
      return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
    }
  };

  struct Accum {
    float sx{0.f}, sy{0.f}, sz{0.f};
    int count{0};
  };

  using VoxelMap = std::unordered_map<std::array<int,3>, Accum, KeyHasher, KeyEqual>;

  inline bool insideBox(
    const tf2::Vector3& p,
    double cx, double cy, double cz,
    double sx, double sy, double sz) const
  {
    return std::fabs(p.x() - cx) <= (sx * 0.5) &&
           std::fabs(p.y() - cy) <= (sy * 0.5) &&
           std::fabs(p.z() - cz) <= (sz * 0.5);
  }

  void cloudCallback(const sensor_msgs::msg::PointCloud2::SharedPtr msg)
  {
    const std::size_t npts = static_cast<std::size_t>(msg->width) * static_cast<std::size_t>(msg->height);
    if (npts == 0) return;

    if (msg->is_bigendian) {
      RCLCPP_ERROR_THROTTLE(
        this->get_logger(), *this->get_clock(), 2000,
        "PointCloud big-endian no soportada.");
      return;
    }

    if (msg->point_step == 0 || msg->data.empty()) {
      RCLCPP_WARN_THROTTLE(
        this->get_logger(), *this->get_clock(), 2000,
        "PointCloud vacía o point_step=0.");
      return;
    }

    const std::size_t expected_size = static_cast<std::size_t>(msg->point_step) * npts;
    if (msg->data.size() < expected_size) {
      RCLCPP_WARN_THROTTLE(
        this->get_logger(), *this->get_clock(), 2000,
        "Tamaño de data inválido: data=%zu, esperado=%zu",
        msg->data.size(), expected_size);
      return;
    }

    // Buscar x,y,z
    const sensor_msgs::msg::PointField *fx = nullptr, *fy = nullptr, *fz = nullptr;
    for (const auto &f : msg->fields) {
      if (f.name == "x") fx = &f;
      else if (f.name == "y") fy = &f;
      else if (f.name == "z") fz = &f;
    }

    if (!fx || !fy || !fz) {
      RCLCPP_ERROR_THROTTLE(
        this->get_logger(), *this->get_clock(), 2000,
        "La nube no tiene campos x/y/z.");
      return;
    }

    if (fx->datatype != sensor_msgs::msg::PointField::FLOAT32 ||
        fy->datatype != sensor_msgs::msg::PointField::FLOAT32 ||
        fz->datatype != sensor_msgs::msg::PointField::FLOAT32) {
      RCLCPP_ERROR_THROTTLE(
        this->get_logger(), *this->get_clock(), 2000,
        "x/y/z deben ser FLOAT32.");
      return;
    }

    if ((fx->offset + 4) > msg->point_step ||
        (fy->offset + 4) > msg->point_step ||
        (fz->offset + 4) > msg->point_step) {
      RCLCPP_ERROR_THROTTLE(
        this->get_logger(), *this->get_clock(), 2000,
        "Offsets x/y/z inválidos para point_step=%u", msg->point_step);
      return;
    }

    // Preparar TF cloud_frame -> exclude_frame
    tf2::Transform cloud_to_box_tf;
    bool use_box_tf = false;
    bool want_boxes = (exclude_enabled_ || hood_enabled_);

    if (want_boxes) {
      if (exclude_frame_ == msg->header.frame_id) {
        cloud_to_box_tf.setIdentity();
        use_box_tf = true;
      } else {
        try {
          auto ts = tf_buffer_->lookupTransform(
            exclude_frame_,                  // target
            msg->header.frame_id,            // source
            rclcpp::Time(msg->header.stamp), // tiempo de la nube
            rclcpp::Duration::from_seconds(tf_timeout_sec_));

          tf2::fromMsg(ts.transform, cloud_to_box_tf);
          use_box_tf = true;
        } catch (const tf2::TransformException &ex) {
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
      // Modo estricto: si no hay TF, no publicamos nube
      return;
    }

    const float min_r2 = static_cast<float>(min_range_ * min_range_);
    const float max_r2 = static_cast<float>(max_range_ * max_range_);
    const float min_zf = static_cast<float>(min_z_);
    const float max_zf = static_cast<float>(max_z_);
    const float inv_leaf = 1.0f / static_cast<float>(leaf_size_);

    const uint8_t* data = msg->data.data();
    const std::size_t step = static_cast<std::size_t>(msg->point_step);

    VoxelMap voxels;
    voxels.reserve(npts / 3 + 1);

    std::size_t nan_rejected = 0;
    std::size_t range_rejected = 0;
    std::size_t z_rejected = 0;
    std::size_t box_rejected = 0;

    for (std::size_t i = 0; i < npts; ++i) {
      const uint8_t* base = data + i * step;

      float x, y, z;
      std::memcpy(&x, base + fx->offset, sizeof(float));
      std::memcpy(&y, base + fy->offset, sizeof(float));
      std::memcpy(&z, base + fz->offset, sizeof(float));

      if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z)) {
        ++nan_rejected;
        continue;
      }

      const float r2 = x*x + y*y + z*z;
      if (r2 < min_r2 || r2 > max_r2) {
        ++range_rejected;
        continue;
      }

      if (z < min_zf || z > max_zf) {
        ++z_rejected;
        continue;
      }

      if (want_boxes && use_box_tf) {
        const tf2::Vector3 p_box = cloud_to_box_tf * tf2::Vector3(x, y, z);

        const bool in_main = exclude_enabled_ &&
          insideBox(p_box, ex_cx_, ex_cy_, ex_cz_, ex_sx_, ex_sy_, ex_sz_);

        const bool in_hood = hood_enabled_ &&
          insideBox(p_box, hx_, hy_, hz_, hsx_, hsy_, hsz_);

        if (in_main || in_hood) {
          ++box_rejected;
          continue;
        }
      }

      const int ix = static_cast<int>(std::floor(x * inv_leaf));
      const int iy = static_cast<int>(std::floor(y * inv_leaf));
      const int iz = static_cast<int>(std::floor(z * inv_leaf));

      const std::array<int,3> key{{ix, iy, iz}};
      auto &acc = voxels[key];
      acc.sx += x;
      acc.sy += y;
      acc.sz += z;
      acc.count += 1;
    }

    if (voxels.empty()) return;

    std::vector<float> out_xyz;
    out_xyz.reserve(voxels.size() * 3);

    std::size_t density_rejected = 0;
    for (const auto &kv : voxels) {
      const Accum &a = kv.second;
      if (a.count < min_points_per_voxel_) {
        ++density_rejected;
        continue;
      }
      const float inv = 1.0f / static_cast<float>(a.count);
      out_xyz.push_back(a.sx * inv);
      out_xyz.push_back(a.sy * inv);
      out_xyz.push_back(a.sz * inv);
    }

    if (out_xyz.empty()) return;

    sensor_msgs::msg::PointCloud2 out;
    out.header.stamp = msg->header.stamp;
    out.header.frame_id = output_frame_id_.empty() ? msg->header.frame_id : output_frame_id_;

    out.height = 1;
    out.width = static_cast<uint32_t>(out_xyz.size() / 3);

    out.is_bigendian = false;
    out.is_dense = true;

    out.point_step = 12;  // x,y,z float32
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
        "in=%zu | vox=%zu | out=%u | rej[nan=%zu,range=%zu,z=%zu,box=%zu,density=%zu] | in_frame=%s | out_frame=%s",
        npts, voxels.size(), out.width,
        nan_rejected, range_rejected, z_rejected, box_rejected, density_rejected,
        msg->header.frame_id.c_str(), out.header.frame_id.c_str());
    }
  }

private:
  // Params IO
  std::string input_topic_;
  std::string output_topic_;
  std::string output_frame_id_;

  // Voxel
  double leaf_size_;
  int min_points_per_voxel_;

  // Global filters
  double min_range_, max_range_;
  double min_z_, max_z_;

  // TF
  double tf_timeout_sec_;

  // Main exclusion box
  bool exclude_enabled_;
  std::string exclude_frame_;
  double ex_cx_, ex_cy_, ex_cz_;
  double ex_sx_, ex_sy_, ex_sz_;

  // Hood exclusion box
  bool hood_enabled_;
  double hx_, hy_, hz_;
  double hsx_, hsy_, hsz_;

  bool pass_through_if_no_tf_;
  bool debug_log_;

  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr sub_;
  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pub_;

  std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
};

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<VoxelGridNode>());
  rclcpp::shutdown();
  return 0;
}
