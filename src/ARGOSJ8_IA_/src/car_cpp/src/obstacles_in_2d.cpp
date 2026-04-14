// // // octomap_navigation_nodes.cpp
// // // Este nodo suscribe al tópico "octomap" (de tipo octomap_msgs::msg::Octomap),
// // // procesa el árbol OctoMap para proyectar la información 3D en el suelo, y
// // // publica "nodos de obstáculo" (como PoseArray y MarkerArray) que indican
// // // las áreas donde el vehículo no debe pasar, basándose en la diferencia
// // // de altura respecto al suelo en cada celda.

// // #include <rclcpp/rclcpp.hpp>
// // #include <geometry_msgs/msg/pose_array.hpp>
// // #include <geometry_msgs/msg/pose.hpp>
// // #include <geometry_msgs/msg/point.hpp>
// // #include <visualization_msgs/msg/marker_array.hpp>
// // #include <visualization_msgs/msg/marker.hpp>

// // // Incluimos los mensajes de OctoMap y la conversión al árbol
// // #include <octomap_msgs/msg/octomap.hpp>
// // #include <octomap_msgs/conversions.h>

// // // Incluir la librería OctoMap para acceder a la clase OcTree
// // #include <octomap/octomap.h>

// // #include <unordered_map>
// // #include <cmath>
// // #include <utility>

// // // Definición de una función hash para std::pair<int,int> para usarlo en unordered_map
// // struct PairHash {
// //   std::size_t operator()(const std::pair<int,int>& p) const {
// //     auto h1 = std::hash<int>{}(p.first);
// //     auto h2 = std::hash<int>{}(p.second);
// //     return h1 ^ (h2 << 1);
// //   }
// // };

// // class OctomapNavigationNodes : public rclcpp::Node {
// // public:
// //   OctomapNavigationNodes() : Node("octomap_navigation_nodes")
// //   {
// //     // Declarar parámetros configurables:
// //     // obstacle_height_threshold: diferencia mínima en z para considerar que en una celda hay obstáculo
// //     // cell_size: tamaño de la celda para agrupar los puntos (en metros)
// //     this->declare_parameter("obstacle_height_threshold", 1.0);
// //     this->declare_parameter("cell_size", 0.5);
// //     obstacle_height_threshold_ = this->get_parameter("obstacle_height_threshold").as_double();
// //     cell_size_ = this->get_parameter("cell_size").as_double();

// //     // Suscribirse al tópico del OctoMap
// //     octomap_sub_ = this->create_subscription<octomap_msgs::msg::Octomap>(
// //       "octomap_full", 10,
// //       std::bind(&OctomapNavigationNodes::octomapCallback, this, std::placeholders::_1)
// //     );

// //     // Publicador para los nodos de obstáculo (PoseArray)
// //     obstacle_nodes_pub_ = this->create_publisher<geometry_msgs::msg::PoseArray>("obstacle_navigation_nodes", 10);

// //     // Publicador para visualizar los nodos de obstáculo con markers
// //     marker_pub_ = this->create_publisher<visualization_msgs::msg::MarkerArray>("obstacle_nodes_markers", 10);

// //     RCLCPP_INFO(this->get_logger(), "Nodo OctomapNavigationNodes iniciado. Umbral obstáculo: %.2f, Tamaño de celda: %.2f",
// //                 obstacle_height_threshold_, cell_size_);
// //   }

// // private:
// //   void octomapCallback(const octomap_msgs::msg::Octomap::SharedPtr msg)
// //   {
// //     // Convertir el mensaje Octomap a un objeto OcTree
// //     // La función fullMsgToMap realiza la conversión y devuelve un AbstractOcTree*
// //     octomap::AbstractOcTree* abstract_tree = octomap_msgs::fullMsgToMap(*msg);
// //     if (!abstract_tree) {
// //       RCLCPP_ERROR(this->get_logger(), "Error al convertir el mensaje Octomap al árbol OcTree.");
// //       return;
// //     }
// //     // Hacemos un cast dinámico a OcTree (la implementación común de OctoMap)
// //     octomap::OcTree* tree = dynamic_cast<octomap::OcTree*>(abstract_tree);
// //     if (!tree) {
// //       RCLCPP_ERROR(this->get_logger(), "Error al convertir AbstractOcTree a OcTree.");
// //       delete abstract_tree;
// //       return;
// //     }

// //     // Utilizaremos dos mapas (hash maps) para:
// //     // 1. Almacenar el nivel del suelo en cada celda (mínimo valor z).
// //     // 2. Marcar si en una celda se detecta al menos un punto que supere el umbral (obstáculo).
// //     std::unordered_map<std::pair<int,int>, double, PairHash> ground_levels;
// //     std::unordered_map<std::pair<int,int>, bool, PairHash> obstacle_cells;

// //     // Primera pasada: determinar el nivel del suelo en cada celda.
// //     // Para cada nodo hoja ocupado, se calcula la celda (x,y) a partir del tamaño de celda.
// //     for (octomap::OcTree::leaf_iterator it = tree->begin_leafs(), end = tree->end_leafs(); it != end; ++it) {
// //       // Consideramos nodos ocupados (umbral típico 0.5 para ocupación)
// //       if (it->getOccupancy() > 0.5) {
// //         double x = it.getX();
// //         double y = it.getY();
// //         double z = it.getZ();
// //         int cell_x = static_cast<int>(std::floor(x / cell_size_));
// //         int cell_y = static_cast<int>(std::floor(y / cell_size_));
// //         std::pair<int,int> cell(cell_x, cell_y);
// //         // Actualizamos el valor mínimo de z (nivel del suelo) para la celda
// //         if (ground_levels.find(cell) == ground_levels.end()) {
// //           ground_levels[cell] = z;
// //         } else {
// //           if (z < ground_levels[cell]) {
// //             ground_levels[cell] = z;
// //           }
// //         }
// //       }
// //     }

// //     // Segunda pasada: identificar en cada celda si existe un obstáculo.
// //     // Se recorre de nuevo el árbol y, para cada nodo ocupado, se compara su altura con el nivel del suelo
// //     for (octomap::OcTree::leaf_iterator it = tree->begin_leafs(), end = tree->end_leafs(); it != end; ++it) {
// //       if (it->getOccupancy() > 0.5) {
// //         double x = it.getX();
// //         double y = it.getY();
// //         double z = it.getZ();
// //         int cell_x = static_cast<int>(std::floor(x / cell_size_));
// //         int cell_y = static_cast<int>(std::floor(y / cell_size_));
// //         std::pair<int,int> cell(cell_x, cell_y);
// //         // Si se conoce el nivel del suelo para la celda, se verifica si la diferencia es mayor al umbral
// //         if (ground_levels.find(cell) != ground_levels.end()) {
// //           double ground_z = ground_levels[cell];
// //           if (z - ground_z > obstacle_height_threshold_) {
// //             obstacle_cells[cell] = true;
// //           }
// //         }
// //       }
// //     }

// //     // Crear un PoseArray para publicar los "nodos de obstáculo" proyectados sobre el suelo.
// //     geometry_msgs::msg::PoseArray obstacle_nodes;
// //     obstacle_nodes.header = msg->header;

// //     // Crear un MarkerArray para visualizar estos nodos (por ejemplo, como cubos rojos)
// //     visualization_msgs::msg::MarkerArray markers;
// //     int marker_id = 0;

// //     // Iterar sobre las celdas marcadas como obstáculo
// //     for (const auto &pair : obstacle_cells) {
// //       if (pair.second) {
// //         int cell_x = pair.first.first;
// //         int cell_y = pair.first.second;
// //         // Obtener el nivel del suelo en la celda
// //         double ground_z = ground_levels[pair.first];
// //         // Calcular el centro de la celda en coordenadas (x, y)
// //         double center_x = (cell_x + 0.5) * cell_size_;
// //         double center_y = (cell_y + 0.5) * cell_size_;
// //         double center_z = ground_z; // Proyectado sobre el suelo

// //         // Crear un pose para el nodo de obstáculo
// //         geometry_msgs::msg::Pose pose;
// //         pose.position.x = center_x;
// //         pose.position.y = center_y;
// //         pose.position.z = center_z;
// //         // Orientación identidad
// //         pose.orientation.w = 1.0;
// //         obstacle_nodes.poses.push_back(pose);

// //         // Crear un marker para visualizar el nodo de obstáculo
// //         visualization_msgs::msg::Marker marker;
// //         marker.header = msg->header;
// //         marker.ns = "obstacle_nodes";
// //         marker.id = marker_id++;
// //         marker.type = visualization_msgs::msg::Marker::CUBE;
// //         marker.action = visualization_msgs::msg::Marker::ADD;
// //         marker.pose = pose;
// //         marker.scale.x = cell_size_;
// //         marker.scale.y = cell_size_;
// //         marker.scale.z = 0.1; // Altura visual pequeña
// //         marker.color.r = 1.0;
// //         marker.color.g = 0.0;
// //         marker.color.b = 0.0;
// //         marker.color.a = 1.0;
// //         marker.lifetime = rclcpp::Duration(0, 0);
// //         markers.markers.push_back(marker);
// //       }
// //     }

// //     // Publicar los nodos de obstáculo y los markers de visualización
// //     obstacle_nodes_pub_->publish(obstacle_nodes);
// //     marker_pub_->publish(markers);

// //     // Liberar la memoria del árbol
// //     delete tree;
// //   }

// //   // Parámetros configurables
// //   double obstacle_height_threshold_; // Diferencia mínima de altura respecto al suelo para considerar un obstáculo
// //   double cell_size_;                 // Tamaño de la celda para agrupar puntos (en metros)

// //   // Suscripción al tópico Octomap
// //   rclcpp::Subscription<octomap_msgs::msg::Octomap>::SharedPtr octomap_sub_;

// //   // Publicadores para los nodos de obstáculo y visualización
// //   rclcpp::Publisher<geometry_msgs::msg::PoseArray>::SharedPtr obstacle_nodes_pub_;
// //   rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_;
// // };

// // int main(int argc, char ** argv)
// // {
// //   rclcpp::init(argc, argv);
// //   auto node = std::make_shared<OctomapNavigationNodes>();
// //   rclcpp::spin(node);
// //   rclcpp::shutdown();
// //   return 0;
// // }
// #include <rclcpp/rclcpp.hpp>
// #include <geometry_msgs/msg/pose_array.hpp>
// #include <geometry_msgs/msg/pose.hpp>
// #include <geometry_msgs/msg/point.hpp>
// #include <visualization_msgs/msg/marker_array.hpp>
// #include <visualization_msgs/msg/marker.hpp>
// #include <octomap/octomap.h>
// #include <octomap/OcTree.h>
// #include <octomap_msgs/msg/octomap.hpp>
// #include <octomap_msgs/conversions.h>
// #include <tf2_ros/buffer.h>
// #include <tf2_ros/transform_listener.h>
// #include <geometry_msgs/msg/transform_stamped.hpp>
// #include <unordered_map>
// #include <cmath>
// #include <utility>

// // Incluir nanoflann (asegúrate de tener instalado libnanoflann-dev)
// #include "nanoflann.hpp"

// // Definición de una función hash para std::pair<int,int>
// struct PairHash {
//   std::size_t operator()(const std::pair<int,int>& p) const {
//     auto h1 = std::hash<int>{}(p.first);
//     auto h2 = std::hash<int>{}(p.second);
//     return h1 ^ (h2 << 1);
//   }
// };

// // Función que recorre el OcTree y extrae los nodos libres.
// std::vector<octomap::point3d> extractFreeNodes(const octomap::OcTree &tree) {
//   std::vector<octomap::point3d> nodes;
//   for (octomap::OcTree::leaf_iterator it = tree.begin_leafs(), end = tree.end_leafs();
//        it != end; ++it) {
//     if (!tree.isNodeOccupied(*it)) {
//       nodes.push_back(it.getCoordinate());
//     }
//   }
//   return nodes;
// }

// // Estructura para usar con nanoflann (solo 2D: x,y)
// struct PointCloud2D {
//   std::vector<std::array<double, 2>> pts;
//   inline size_t kdtree_get_point_count() const { return pts.size(); }
//   inline double kdtree_get_pt(const size_t idx, const size_t dim) const { return pts[idx][dim]; }
//   template <class BBOX> bool kdtree_get_bbox(BBOX&) const { return false; }
// };

// // Downsampling mediante kd-tree usando nanoflann
// std::vector<octomap::point3d> downsampleNodesKdTree(const std::vector<octomap::point3d>& nodes,
//                                                     double node_distance_threshold) {
//   std::vector<octomap::point3d> downsampled;
//   if (nodes.empty()) return downsampled;
//   PointCloud2D cloud;
//   for (const auto &pt : nodes) {
//     cloud.pts.push_back({pt.x(), pt.y()});
//   }
//   typedef nanoflann::KDTreeSingleIndexAdaptor<
//       nanoflann::L2_Simple_Adaptor<double, PointCloud2D>,
//       PointCloud2D,
//       2
//       > kd_tree_t;
//   kd_tree_t index(2, cloud, nanoflann::KDTreeSingleIndexAdaptorParams(10));
//   index.buildIndex();

//   std::vector<bool> accepted(cloud.pts.size(), false);
//   const double search_radius = node_distance_threshold * node_distance_threshold;
//   for (size_t i = 0; i < cloud.pts.size(); ++i) {
//     if (accepted[i])
//       continue;
//     accepted[i] = true;
//     downsampled.push_back(octomap::point3d(cloud.pts[i][0], cloud.pts[i][1], 0.0));
//     std::vector<std::pair<unsigned int, double>> ret_matches;
//     nanoflann::SearchParams params;
//     const double query_pt[2] = { cloud.pts[i][0], cloud.pts[i][1] };
//     index.radiusSearch(&query_pt[0], search_radius, ret_matches, params);
//     for (size_t j = 0; j < ret_matches.size(); j++) {
//       accepted[ret_matches[j].first] = true;
//     }
//   }
//   return downsampled;
// }

// class OctomapNavigationNodes : public rclcpp::Node {
// public:
//   OctomapNavigationNodes() : Node("octomap_navigation_nodes")
//   {
//     // Declarar parámetros configurables.
//     this->declare_parameter("obstacle_height_threshold", 1.0);
//     this->declare_parameter("cell_size", 0.2);
//     obstacle_height_threshold_ = this->get_parameter("obstacle_height_threshold").as_double();
//     cell_size_ = this->get_parameter("cell_size").as_double();

//     // Suscribirse al tópico del OctoMap.
//     octomap_sub_ = this->create_subscription<octomap_msgs::msg::Octomap>(
//       "octomap_full", 10,
//       std::bind(&OctomapNavigationNodes::octomapCallback, this, std::placeholders::_1)
//     );

//     // Publicador para los nodos de obstáculo (PoseArray).
//     obstacle_nodes_pub_ = this->create_publisher<geometry_msgs::msg::PoseArray>("obstacle_navigation_nodes", 10);

//     // Publicador para visualizar los nodos de obstáculo con markers.
//     marker_pub_ = this->create_publisher<visualization_msgs::msg::MarkerArray>("obstacle_nodes_markers", 10);

//     // Inicializar el tf2 buffer y el listener.
//     tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock(), tf2::Duration(std::chrono::seconds(10)));
//     tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_, this);

//     RCLCPP_INFO(this->get_logger(), "Nodo OctomapNavigationNodes iniciado. Umbral obstáculo: %.2f, Tamaño de celda: %.2f",
//                 obstacle_height_threshold_, cell_size_);
//   }

// private:
//   void octomapCallback(const octomap_msgs::msg::Octomap::SharedPtr msg) {
    
//     // Obtener la transformación "base_link" -> "map".
//     geometry_msgs::msg::TransformStamped transformStamped;
//     try {
//       rclcpp::Time query_time(0, 0, this->get_clock()->get_clock_type());
//       transformStamped = tf_buffer_->lookupTransform("map", "base_link", query_time, rclcpp::Duration::from_seconds(0.1));
//     } catch (tf2::TransformException &ex) {
//       RCLCPP_WARN(this->get_logger(), "No se pudo obtener la transformación: %s", ex.what());
//       return;
//     }
//     double robot_x = transformStamped.transform.translation.x;
//     double robot_y = transformStamped.transform.translation.y;
//     double robot_z = transformStamped.transform.translation.z;

//     // Convertir el mensaje Octomap a OcTree.
//     octomap::AbstractOcTree* abstract_tree = octomap_msgs::fullMsgToMap(*msg);
//     if (!abstract_tree) {
//       RCLCPP_ERROR(this->get_logger(), "Error al convertir el mensaje Octomap a OcTree");
//       return;
//     }
//     octomap::OcTree* tree_ptr = dynamic_cast<octomap::OcTree*>(abstract_tree);
//     if (!tree_ptr) {
//       RCLCPP_ERROR(this->get_logger(), "El árbol obtenido no es de tipo OcTree");
//       delete abstract_tree;
//       return;
//     }
//     std::shared_ptr<octomap::OcTree> tree(tree_ptr);

//     // Primera pasada: determinar el nivel del suelo en cada celda (usando nodos ocupados)
//     std::unordered_map<std::pair<int,int>, double, PairHash> ground_levels;
//     for (octomap::OcTree::leaf_iterator it = tree->begin_leafs(), end = tree->end_leafs();
//          it != end; ++it) {
//       if (it->getOccupancy() > 0.5) {
//         double x = it.getX();
//         double y = it.getY();
//         double z = it.getZ();
//         int cell_x = static_cast<int>(std::floor(x / cell_size_));
//         int cell_y = static_cast<int>(std::floor(y / cell_size_));
//         std::pair<int,int> cell(cell_x, cell_y);
//         if (ground_levels.find(cell) == ground_levels.end()) {
//           ground_levels[cell] = z;
//         } else if (z < ground_levels[cell]) {
//           ground_levels[cell] = z;
//         }
//       }
//     }

//     // Segunda pasada: identificar celdas con obstáculo si la diferencia en z es mayor al umbral.
//     std::unordered_map<std::pair<int,int>, bool, PairHash> obstacle_cells;
//     for (octomap::OcTree::leaf_iterator it = tree->begin_leafs(), end = tree->end_leafs();
//          it != end; ++it) {
//       if (it->getOccupancy() > 0.5) {
//         double x = it.getX();
//         double y = it.getY();
//         double z = it.getZ();
//         int cell_x = static_cast<int>(std::floor(x / cell_size_));
//         int cell_y = static_cast<int>(std::floor(y / cell_size_));
//         std::pair<int,int> cell(cell_x, cell_y);
//         if (ground_levels.find(cell) != ground_levels.end()) {
//           double ground_z = ground_levels[cell];
//           if (z - ground_z > obstacle_height_threshold_) {
//             obstacle_cells[cell] = true;
//           }
//         }
//       }
//     }

//     // Filtrado por vecinos: conservar solo las celdas que tengan al menos 1 vecino marcado.
//     std::unordered_map<std::pair<int,int>, bool, PairHash> final_cells;
//     for (const auto &p : obstacle_cells) {
//       if (p.second) {
//         int cx = p.first.first;
//         int cy = p.first.second;
//         int neighbor_count = 0;
//         for (int dx = -1; dx <= 1; ++dx) {
//           for (int dy = -1; dy <= 1; ++dy) {
//             if (dx == 0 && dy == 0) continue;
//             std::pair<int,int> neighbor(cx+dx, cy+dy);
//             if (obstacle_cells.find(neighbor) != obstacle_cells.end() && obstacle_cells[neighbor]) {
//               neighbor_count++;
//             }
//           }
//         }
//         if (neighbor_count >= 2) {
//           final_cells[p.first] = true;
//         }
//       }
//     }

//     // Crear un PoseArray para los nodos de obstáculo proyectados sobre el suelo.
//     geometry_msgs::msg::PoseArray obstacle_nodes;
//     obstacle_nodes.header = msg->header;
//     obstacle_nodes.header.frame_id = "map";

//     // Crear un MarkerArray para visualizar los obstáculos.
//     visualization_msgs::msg::MarkerArray markers;
//     int marker_id = 0;
//     for (const auto &cell : final_cells) {
//       int cell_x = cell.first.first;
//       int cell_y = cell.first.second;
//       double ground_z = ground_levels[cell.first];
//       double center_x = (cell_x + 0.5) * cell_size_;
//       double center_y = (cell_y + 0.5) * cell_size_;
//       double center_z = ground_z; // Proyección sobre el suelo

//       geometry_msgs::msg::Pose pose;
//       pose.position.x = center_x;
//       pose.position.y = center_y;
//       pose.position.z = center_z;
//       pose.orientation.w = 1.0;
//       obstacle_nodes.poses.push_back(pose);

//       visualization_msgs::msg::Marker marker;
//       marker.header = msg->header;
//       marker.ns = "obstacle_nodes";
//       marker.id = marker_id++;
//       marker.type = visualization_msgs::msg::Marker::CUBE;
//       marker.action = visualization_msgs::msg::Marker::ADD;
//       marker.pose = pose;
//       marker.scale.x = cell_size_;
//       marker.scale.y = cell_size_;
//       marker.scale.z = 0.1;
//       marker.color.r = 1.0;
//       marker.color.g = 0.0;
//       marker.color.b = 0.0;
//       marker.color.a = 1.0;
//       marker.lifetime = rclcpp::Duration(0, 0);
//       markers.markers.push_back(marker);
//     }

//     // Publicar en los tópicos indicados.
//     obstacle_nodes_pub_->publish(obstacle_nodes);
//     marker_pub_->publish(markers);
//   }

//   rclcpp::Publisher<geometry_msgs::msg::PoseArray>::SharedPtr obstacle_nodes_pub_;
//   rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_;
//   rclcpp::Subscription<octomap_msgs::msg::Octomap>::SharedPtr octomap_sub_;
//   std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
//   std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
//   double min_distance_;
//   double max_distance_;
//   double cell_size_;
//   double node_distance_threshold_;
//   double obstacle_height_threshold_;
//   std::unordered_map<std::pair<int,int>, bool, PairHash> prev_cell_map_;
// };

// int main(int argc, char ** argv)
// {
//   rclcpp::init(argc, argv);
//   auto node = std::make_shared<OctomapNavigationNodes>();
//   rclcpp::spin(node);
//   rclcpp::shutdown();
//   return 0;
// }



// octomap_navigation_nodes.cpp
//
// Nodo que extrae obstáculos de un OctoMap pero **ignora** aquellas celdas que
// el tópico "terrain_slope" marca como terreno transitable.
//
// ROS 2 Humble • C++17
// -----------------------------------------------------------------------------

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_array.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <visualization_msgs/msg/marker.hpp>

#include <octomap/octomap.h>
#include <octomap/OcTree.h>
#include <octomap_msgs/msg/octomap.hpp>
#include <octomap_msgs/conversions.h>

#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/msg/transform_stamped.hpp>

#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <cmath>
#include <utility>

// (Opcional) Si sigues usando nanoflann y k-d-tree para downsampling
#include "nanoflann.hpp"

// -----------------------------------------------------------------------------
// Utilidades
// -----------------------------------------------------------------------------
struct PairHash
{
  std::size_t operator()(const std::pair<int,int>& p) const
  {
    auto h1 = std::hash<int>{}(p.first);
    auto h2 = std::hash<int>{}(p.second);
    return h1 ^ (h2 << 1);
  }
};

// -----------------------------------------------------------------------------
// Nodo principal
// -----------------------------------------------------------------------------
class OctomapNavigationNodes : public rclcpp::Node
{
public:
  OctomapNavigationNodes()
  : Node("octomap_navigation_nodes")
  {
    // ---------------------- Parámetros configurables -------------------------
    this->declare_parameter("obstacle_height_threshold", 1.5); // [m]
    this->declare_parameter("cell_size",                 0.20); // [m]
    obstacle_height_threshold_ = this->get_parameter("obstacle_height_threshold").as_double();
    cell_size_                 = this->get_parameter("cell_size").as_double();

    // --------------------------- Suscripciones -------------------------------
    octomap_sub_ = this->create_subscription<octomap_msgs::msg::Octomap>(
      "octomap_full", 10,
      std::bind(&OctomapNavigationNodes::octomapCallback, this, std::placeholders::_1));

    // ❶  Nueva suscripción al terreno con pendiente transitable
    slope_sub_ = this->create_subscription<visualization_msgs::msg::MarkerArray>(
      "terrain_slope", 10,
      std::bind(&OctomapNavigationNodes::slopeCallback, this, std::placeholders::_1));

    // --------------------------- Publicadores --------------------------------
    obstacle_nodes_pub_ =
      this->create_publisher<geometry_msgs::msg::PoseArray>("obstacle_navigation_nodes", 10);

    marker_pub_ =
      this->create_publisher<visualization_msgs::msg::MarkerArray>("obstacle_nodes_markers", 10);

    // -------------------------- TF listener ----------------------------------
    tf_buffer_   = std::make_shared<tf2_ros::Buffer>(this->get_clock(),
                                                     tf2::Duration(std::chrono::seconds(10)));
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_, this);

    RCLCPP_INFO(get_logger(),
                "OctomapNavigationNodes listo  –  umbral_z = %.2f m, cell = %.2f m.",
                obstacle_height_threshold_, cell_size_);
  }

private:
  // ======================================================================
  // CALLBACK: Slope  ------------------------------------------------------
  // ======================================================================
  void slopeCallback(const visualization_msgs::msg::MarkerArray::SharedPtr msg)
  {
    std::lock_guard<std::mutex> lock(slope_mutex_);
    slope_cells_.clear();

    for (const auto &m : msg->markers)
    {
      // Solo ADD – ignoramos DELETE / modificaciones
      if (m.action != visualization_msgs::msg::Marker::ADD)
        continue;

      auto insert_point = [this](double x, double y,
                                 std::unordered_set<std::pair<int,int>,PairHash> &set)
      {
        int cx = static_cast<int>(std::floor(x / cell_size_));
        int cy = static_cast<int>(std::floor(y / cell_size_));
        set.insert({cx, cy});
      };

      // 1. Marker con pose (por ejemplo, CUBE o SPHERE):
      insert_point(m.pose.position.x, m.pose.position.y, slope_cells_);

      // 2. Si es LINE_STRIP / LIST u otro con puntos, añadimos cada punto
      for (const auto  &p : m.points)
        insert_point(p.x, p.y, slope_cells_);
    }
  }

  // ======================================================================
  // CALLBACK: OctoMap  ----------------------------------------------------
  // ======================================================================
  void octomapCallback(const octomap_msgs::msg::Octomap::SharedPtr msg)
  {
    // -----------------------------------------------------------------
    // TF: base_link -> map  (no esencial para el filtrado en sí, pero
    // lo mantenemos porque ya estaba en tu código original)
    // -----------------------------------------------------------------
    geometry_msgs::msg::TransformStamped transformStamped;
    try {
      rclcpp::Time query_time(0, 0, this->get_clock()->get_clock_type());
      transformStamped = tf_buffer_->lookupTransform("map", "base_link", query_time, rclcpp::Duration::from_seconds(0.1));
    } catch (tf2::TransformException &ex) {
      RCLCPP_WARN(this->get_logger(), "No se pudo obtener la transformación: %s", ex.what());
      return;
    }

    // -----------------------------------------------------------------
    // Convertir Octomap a OcTree
    // -----------------------------------------------------------------
    auto *abstract_tree = octomap_msgs::fullMsgToMap(*msg);
    if (!abstract_tree)
    {
      RCLCPP_ERROR(get_logger(), "No se pudo convertir Octomap -> OcTree");
      return;
    }

    auto *raw_tree = dynamic_cast<octomap::OcTree*>(abstract_tree);
    if (!raw_tree)
    {
      RCLCPP_ERROR(get_logger(), "AbstractOcTree no es OcTree");
      delete abstract_tree;
      return;
    }
    std::shared_ptr<octomap::OcTree> tree(raw_tree);   // RAII

    // -----------------------------------------------------------------
    // Primera pasada: nivel mínimo (suelo) por celda 2 D
    // -----------------------------------------------------------------
    std::unordered_map<std::pair<int,int>, double, PairHash> ground_z;
    for (auto it = tree->begin_leafs(), end = tree->end_leafs(); it != end; ++it)
    {
      if (!tree->isNodeOccupied(*it)) continue;

      int cx = static_cast<int>(std::floor(it.getX() / cell_size_));
      int cy = static_cast<int>(std::floor(it.getY() / cell_size_));
      auto key = std::make_pair(cx, cy);

      const double z = it.getZ();
      if (ground_z.find(key) == ground_z.end() || z < ground_z[key])
        ground_z[key] = z;
    }

    // -----------------------------------------------------------------
    // Segunda pasada: marcar celdas con obstáculo según Δz > umbral,
    //                 pero ignorando celdas presentes en slope_cells_
    // -----------------------------------------------------------------
    std::unordered_map<std::pair<int,int>, bool, PairHash> obstacle_cells;

    {
      std::lock_guard<std::mutex> lock(slope_mutex_);

      for (auto it = tree->begin_leafs(), end = tree->end_leafs(); it != end; ++it)
      {
        if (!tree->isNodeOccupied(*it)) continue;

        int cx = static_cast<int>(std::floor(it.getX() / cell_size_));
        int cy = static_cast<int>(std::floor(it.getY() / cell_size_));
        auto key = std::make_pair(cx, cy);

        // ❷ Descartar si la celda está marcada como transitable
        if (slope_cells_.count(key)) continue;

        auto it_ground = ground_z.find(key);
        if (it_ground == ground_z.end()) continue;

        if ( (it.getZ() - it_ground->second) > obstacle_height_threshold_)
          obstacle_cells[key] = true;
      }
    }

    // -----------------------------------------------------------------
    // Filtro de vecindad (≥ 2 vecinos)
    // -----------------------------------------------------------------
    std::unordered_map<std::pair<int,int>, bool, PairHash> final_cells;
    for (const auto &kv : obstacle_cells)
    {
      if (!kv.second) continue;

      const int cx = kv.first.first;
      const int cy = kv.first.second;

      int neighbors = 0;
      for (int dx=-1; dx<=1; ++dx)
        for (int dy=-1; dy<=1; ++dy)
        {
          if (dx==0 && dy==0) continue;
          if (obstacle_cells.find({cx+dx, cy+dy}) != obstacle_cells.end())
            ++neighbors;
        }

      if (neighbors >= 2)
        final_cells[kv.first] = true;
    }

    // -----------------------------------------------------------------
    // Publicar resultados
    // -----------------------------------------------------------------
    geometry_msgs::msg::PoseArray pa;
    pa.header = msg->header;      // frame = "map" (ya viene en msg->header)
    visualization_msgs::msg::MarkerArray ma;

    int id = 0;
    for (const auto &kv : final_cells)
    {
      const int cx = kv.first.first;
      const int cy = kv.first.second;

      const double cz = ground_z[kv.first];     // suelo z
      const double px = (cx + 0.5)*cell_size_;
      const double py = (cy + 0.5)*cell_size_;

      geometry_msgs::msg::Pose p;
      p.position.x = px;
      p.position.y = py;
      p.position.z = cz;
      p.orientation.w = 1.0;
      pa.poses.emplace_back(p);

      visualization_msgs::msg::Marker m;
      m.header   = msg->header;
      m.ns       = "obstacle_nodes";
      m.id       = id++;
      m.type     = visualization_msgs::msg::Marker::CUBE;
      m.action   = visualization_msgs::msg::Marker::ADD;
      m.pose     = p;
      m.scale.x  = cell_size_;
      m.scale.y  = cell_size_;
      m.scale.z  = 0.10;
      m.color.r  = 1.0f;
      m.color.g  = 0.0f;
      m.color.b  = 0.0f;
      m.color.a  = 1.0f;
      m.lifetime = rclcpp::Duration(0,0);
      ma.markers.emplace_back(m);
    }

    obstacle_nodes_pub_->publish(pa);
    marker_pub_->publish(ma);
  }

  // -------------------------------------------------------------------
  // Miembros
  // -------------------------------------------------------------------
  // Publicadores
  rclcpp::Publisher<geometry_msgs::msg::PoseArray>::SharedPtr     obstacle_nodes_pub_;
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_;

  // Suscripciones
  rclcpp::Subscription<octomap_msgs::msg::Octomap>::SharedPtr            octomap_sub_;
  rclcpp::Subscription<visualization_msgs::msg::MarkerArray>::SharedPtr  slope_sub_;

  // TF
  std::shared_ptr<tf2_ros::Buffer>          tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;

  // Parámetros
  double obstacle_height_threshold_{1.0};
  double cell_size_{0.20};

  // Terreno transitable recibido
  std::unordered_set<std::pair<int,int>, PairHash> slope_cells_;
  std::mutex                                       slope_mutex_;
};

// -----------------------------------------------------------------------------
// main
// -----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<OctomapNavigationNodes>());
  rclcpp::shutdown();
  return 0;
}
