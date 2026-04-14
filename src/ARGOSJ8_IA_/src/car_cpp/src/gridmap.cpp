// // gridmap_from_markers_node.cpp — rellena NaN como plano (pendiente 0 °, elevación media)
// // Convierte el MarkerArray de terrain_slope_node en un GridMap continuo.
// //
// // Capas publicadas:
// //   • elevation   – z del marcador con huecos rellenados por la media de z
// //   • slope_deg   – pendiente (°) con huecos rellenados a 0
// // Publica también un OccupancyGrid para debug (toda celda NaN originalmente se marca libre).
// //
// // Suscribe: /terrain_slope (visualization_msgs/MarkerArray)
// // Publica : /terrain_grid_from_markers (GridMap)  &  /terrain_occupancy_from_markers (OccupancyGrid)

// #include <rclcpp/rclcpp.hpp>
// #include <visualization_msgs/msg/marker_array.hpp>
// #include <nav_msgs/msg/occupancy_grid.hpp>
// #include <grid_map_ros/grid_map_ros.hpp>
// #include <grid_map_ros/GridMapRosConverter.hpp>
// #include <grid_map_msgs/msg/grid_map.hpp>
// #include <opencv2/imgproc.hpp>
// #include <limits>
// #include <cmath>
// #include <cstring>

// class GridMapFromMarkers : public rclcpp::Node
// {
// public:
//   GridMapFromMarkers() : Node("gridmap_from_markers")
//   {
//     declare_parameter("resolution", 1.0);
//     declare_parameter("margin_cells", 5);
//     declare_parameter("obstacle_slope_deg", 20.0);

//     res_     = get_parameter("resolution").as_double();
//     margin_  = get_parameter("margin_cells").as_int();
//     obs_thr_ = get_parameter("obstacle_slope_deg").as_double();

//     sub_markers_ = create_subscription<visualization_msgs::msg::MarkerArray>(
//       "/terrain_slope", 10,
//       std::bind(&GridMapFromMarkers::markersCB, this, std::placeholders::_1));

//     pub_grid_ = create_publisher<grid_map_msgs::msg::GridMap>("/terrain_grid_from_markers", 10);
//     pub_occ_  = create_publisher<nav_msgs::msg::OccupancyGrid>("/terrain_occupancy_from_markers", 10);
//   }

// private:
//   void markersCB(const visualization_msgs::msg::MarkerArray::SharedPtr msg)
//   {
//     if(msg->markers.empty()) return;

//     /* 1. Bounding box */
//     double ox=std::numeric_limits<double>::max(), oy=std::numeric_limits<double>::max();
//     double mx=std::numeric_limits<double>::lowest(), my=std::numeric_limits<double>::lowest();
//     for(const auto &m : msg->markers){
//       ox = std::min(ox, m.pose.position.x);
//       oy = std::min(oy, m.pose.position.y);
//       mx = std::max(mx, m.pose.position.x);
//       my = std::max(my, m.pose.position.y);
//     }
//     const double mg = margin_ * res_;
//     ox -= mg; oy -= mg; mx += mg; my += mg;
//     const int width  = static_cast<int>(std::ceil((mx - ox)/res_));
//     const int height = static_cast<int>(std::ceil((my - oy)/res_));

//     /* 2. Buffers */
//     cv::Mat elev (height,width,CV_32FC1,cv::Scalar(std::numeric_limits<float>::quiet_NaN()));
//     cv::Mat slope(height,width,CV_32FC1,cv::Scalar(std::numeric_limits<float>::quiet_NaN()));
//     cv::Mat occ  (height,width,CV_8SC1 ,cv::Scalar(-1));

//     auto w2m = [&](double wx, double wy, int &ix, int &iy)
//     {
//       ix = static_cast<int>(std::floor((wx - ox) / res_));
//       iy = static_cast<int>(std::floor((wy - oy) / res_));

//       // // OpenCV tiene el origen (0,0) arriba-izquierda; ROS lo tiene abajo-izquierda.
//       //iy = width - 1 - iy;      // ← corrección indispensable
//       // ix = std::clamp(ix, 0, width - 1);
//     };

//     /* 3. Volcar marcadores */
//     for(const auto &m : msg->markers){
//       int ix,iy; w2m(m.pose.position.x, m.pose.position.y, ix, iy);
//       if(ix<0||iy<0||ix>=width||iy>=height) continue;
//       elev.at<float>(iy,ix)  = m.pose.position.z;
//       float deg              = static_cast<float>(m.color.r) * 45.0f;
//       slope.at<float>(iy,ix) = deg;
//       occ.at<int8_t>(iy,ix)  = (deg > obs_thr_ ? 100 : 0);
//     }

//     /* 4. Calcular media de elevaciones válidas */
//     cv::Scalar meanElev = cv::mean(elev, elev == elev); // NaN!=NaN
//     float elev_fill_val = std::isnan(meanElev[0]) ? 0.0f : static_cast<float>(meanElev[0]);

//     /* 5. Rellenar NaN como plano */
//     cv::patchNaNs(elev , elev_fill_val);
//     cv::patchNaNs(slope, 0.0f);        // pendiente 0° donde antes no había datos

//     /* 6. Cualquier celda todavía desconocida en occ se marca como libre */
//     occ.setTo(0, occ == -1);

//     /* 7. Construir GridMap */
//     grid_map::GridMap gm({"elevation","slope_deg"});
//     gm.setFrameId("map");
//     gm.setGeometry(grid_map::Length(width*res_,height*res_), res_,
//                    grid_map::Position(ox+width*res_/2.0, oy+height*res_/2.0));

//     auto copy=[&](const cv::Mat &src,const std::string &layer){
//       grid_map::Matrix &dst=gm[layer]; dst.setConstant(std::numeric_limits<float>::quiet_NaN());
//       const bool rowMatch=(dst.rows()==height);
//       for(int y=0;y<height;++y){ for(int x=0;x<width;++x){ float v;
//         if(src.type()==CV_8SC1) v=static_cast<float>(src.at<int8_t>(y,x)); else v=src.at<float>(y,x);
//         if(rowMatch) dst(y,x)=v; else dst(x,y)=v; }} };
//     copy(elev ,"elevation");
//     copy(slope,"slope_deg");

//     pub_grid_->publish(*grid_map::GridMapRosConverter::toMessage(gm));

//     /* 8. Occupancy debug */
//     nav_msgs::msg::OccupancyGrid og; og.header.stamp=now(); og.header.frame_id="map";
//     og.info.resolution=res_; og.info.width=width; og.info.height=height;
//     og.info.origin.position.x=ox; og.info.origin.position.y=oy; og.info.origin.orientation.w=1.0;
//     og.data.resize(width*height); std::memcpy(og.data.data(), occ.data, width*height);
//     pub_occ_->publish(og);
//   }

//   /* --- params & comms --- */
//   double res_{}; int margin_{}; double obs_thr_{};
//   rclcpp::Subscription<visualization_msgs::msg::MarkerArray>::SharedPtr sub_markers_;
//   rclcpp::Publisher<grid_map_msgs::msg::GridMap>::SharedPtr pub_grid_;
//   rclcpp::Publisher<nav_msgs::msg::OccupancyGrid>::SharedPtr pub_occ_;
// };

// int main(int argc,char**argv){ rclcpp::init(argc,argv); rclcpp::spin(std::make_shared<GridMapFromMarkers>()); rclcpp::shutdown(); }

// terrain_grid_map_node.cpp
// ROS 2 node that subscribes to a terrain_slope MarkerArray and generates
// a GridMap layer ("elevation") with the ground height of each grid cell.
//
// Author: ChatGPT 2025-05-21

#include <rclcpp/rclcpp.hpp>
#include <visualization_msgs/msg/marker_array.hpp>

#include <grid_map_ros/grid_map_ros.hpp>          // GridMap + converter
#include <grid_map_msgs/msg/grid_map.hpp>

#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/msg/transform_stamped.hpp>

#include <memory>
#include <limits>
#include <cmath>

class TerrainGridMapNode : public rclcpp::Node
{
public:
  TerrainGridMapNode() : Node("terrain_grid_map_node"), tf_buffer_(this->get_clock())
  {
    declare_parameter("cell_size",        1.0);   // [m]
    declare_parameter("grid_length_x",   40.0);   // [m]
    declare_parameter("grid_length_y",   40.0);   // [m]
    declare_parameter("layer_name", "elevation");

    cell_size_     = get_parameter("cell_size").as_double();
    length_x_      = get_parameter("grid_length_x").as_double();
    length_y_      = get_parameter("grid_length_y").as_double();
    layer_name_    = get_parameter("layer_name").as_string();

    marker_sub_ = create_subscription<visualization_msgs::msg::MarkerArray>(
        "terrain_slope", rclcpp::SensorDataQoS(),
        std::bind(&TerrainGridMapNode::markerCB, this, std::placeholders::_1));

    grid_pub_ = create_publisher<grid_map_msgs::msg::GridMap>("terrain_grid", 10);

    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(tf_buffer_, this);

    RCLCPP_INFO(get_logger(),
                "TerrainGridMapNode listo - celda=%.2fm, tamaño=%.1fx%.1fm.",
                cell_size_, length_x_, length_y_);
  }

private:
  /* -------------------------------------------------------------------- */
  void markerCB(const visualization_msgs::msg::MarkerArray::SharedPtr msg)
  {
    /* 1) Centro del mapa = posición actual del robot ------------------- */
    geometry_msgs::msg::TransformStamped tf_map_base;
    try {
      tf_map_base = tf_buffer_.lookupTransform("map", "base_link", rclcpp::Time(0));
    } catch (const tf2::TransformException &e) {
      RCLCPP_WARN(get_logger(), "TF lookup falló: %s", e.what());
      return;
    }
    const double cx = tf_map_base.transform.translation.x;
    const double cy = tf_map_base.transform.translation.y;

    /* 2) Crear un GridMap con una sola capa “elevation” ----------------- */
    grid_map::GridMap map({layer_name_});
    map.setFrameId("map");
    map.setGeometry(grid_map::Length(length_x_, length_y_), cell_size_,
                    grid_map::Position(cx, cy));               // centro en robot
    map[layer_name_].setConstant(std::numeric_limits<float>::quiet_NaN());

    /* 3) Insertar cada marcador (x,y,z) en la celda correspondiente ----- */
    for (const auto &mk : msg->markers) {
      const double px = mk.pose.position.x;
      const double py = mk.pose.position.y;
      const double pz = mk.pose.position.z;

      grid_map::Position pt(px, py);
      if (!map.isInside(pt)) continue;

      grid_map::Index idx;
      map.getIndex(pt, idx);

      float &cell = map.at(layer_name_, idx);
      if (std::isnan(cell) || pz < cell) {          // nos quedamos con la cota más baja
        cell = static_cast<float>(pz);
      }
    }

    /* 4) Publicar ------------------------------------------------------- */
    auto msg_out = grid_map::GridMapRosConverter::toMessage(map);
    grid_pub_->publish(*msg_out);
    RCLCPP_DEBUG(get_logger(), "Publicado GridMap con %zu celdas pobladas.",
                 map[layer_name_].array().count());
  }

  /* ------------------------------ Vars ---------------------------------- */
  rclcpp::Subscription<visualization_msgs::msg::MarkerArray>::SharedPtr marker_sub_;
  rclcpp::Publisher<grid_map_msgs::msg::GridMap>::SharedPtr            grid_pub_;

  tf2_ros::Buffer                                                      tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener>                          tf_listener_;

  double       cell_size_;
  double       length_x_, length_y_;
  std::string  layer_name_;
};

/* ----------------------------------------------------------------------- */
int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TerrainGridMapNode>());
  rclcpp::shutdown();
  return 0;
}
