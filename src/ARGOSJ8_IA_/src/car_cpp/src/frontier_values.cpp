// #include <rclcpp/rclcpp.hpp>
// #include <nav_msgs/msg/occupancy_grid.hpp>
// #include <visualization_msgs/msg/marker.hpp>
// #include <geometry_msgs/msg/point.hpp>
// #include <geometry_msgs/msg/pose_array.hpp>
// #include <geometry_msgs/msg/pose.hpp>
// #include <std_msgs/msg/float64.hpp>
// #include <std_msgs/msg/float64_multi_array.hpp>
// #include <vector>
// #include <cmath>
// #include <algorithm>
// #include <utility>

// using std::placeholders::_1;

// // Parámetros ajustables
// constexpr double SAFE_DISTANCE_THRESHOLD = 3;      // Distancia mínima permitida entre centroide y obstáculo (m)
// constexpr double CLUSTER_DISTANCE_THRESHOLD = 2;     // Distancia máxima para agrupar puntos en un mismo cluster

// // Función para calcular la distancia euclidiana entre dos puntos
// double euclideanDistance(const geometry_msgs::msg::Point &p1, const geometry_msgs::msg::Point &p2) {
//   return std::hypot(p2.x - p1.x, p2.y - p1.y);
// }

// class FrontierBoundaryNode : public rclcpp::Node {
// public:
//   FrontierBoundaryNode() : Node("frontier_boundary_node")
//   {
//     // Suscribirse al mapa proyectado (OccupancyGrid)
//     occupancy_sub_ = this->create_subscription<nav_msgs::msg::OccupancyGrid>(
//       "/occupancy", 10,
//       std::bind(&FrontierBoundaryNode::occupancyCallback, this, _1)
//     );
//     // Suscribirse a los nodos ocupados (obstáculos)
//     occupied_nodes_sub_ = this->create_subscription<geometry_msgs::msg::PoseArray>(
//       "/occupied_rejected_nodes", 10,
//       std::bind(&FrontierBoundaryNode::occupiedNodesCallback, this, _1)
//     );
//     // Publicadores originales
//     marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>("frontier_marker", 10);
//     frontier_points_pub_ = this->create_publisher<geometry_msgs::msg::PoseArray>("frontier_points", 10);
//     frontier_entropy_pub_ = this->create_publisher<std_msgs::msg::Float64MultiArray>("frontier_entropies", 10);
//     total_entropy_pub_ = this->create_publisher<std_msgs::msg::Float64>("total_entropy", 10);
//     // Publicadores para safe frontier
//     safe_frontier_pub_ = this->create_publisher<geometry_msgs::msg::PoseArray>("safe_frontier_points", 10);
//     safe_frontier_entropy_pub_ = this->create_publisher<std_msgs::msg::Float64MultiArray>("safe_frontier_entropy", 10);
//     safe_marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>("safe_frontier_marker", 10);
//     total_safe_entropy_pub_ = this->create_publisher<std_msgs::msg::Float64>("total_safe_entropy", 10);

//     RCLCPP_INFO(this->get_logger(), "Nodo de frontera basado en projected_map iniciado.");
//   }

// private:
//   // Suscriptores
//   rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr occupancy_sub_;
//   rclcpp::Subscription<geometry_msgs::msg::PoseArray>::SharedPtr occupied_nodes_sub_;
//   // Publicadores originales
//   rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_pub_;
//   rclcpp::Publisher<geometry_msgs::msg::PoseArray>::SharedPtr frontier_points_pub_;
//   rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr frontier_entropy_pub_;
//   rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr total_entropy_pub_;
//   // Publicadores para safe frontier
//   rclcpp::Publisher<geometry_msgs::msg::PoseArray>::SharedPtr safe_frontier_pub_;
//   rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr safe_frontier_entropy_pub_;
//   rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr safe_marker_pub_;
//   rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr total_safe_entropy_pub_;

//   // Variables para almacenar datos
//   nav_msgs::msg::OccupancyGrid::SharedPtr occupancy_grid_;
//   geometry_msgs::msg::PoseArray::SharedPtr occupied_nodes_;

//   // Función para calcular la entropía de una celda
//   double computeCellEntropy(int8_t cell_value) {
//     double p;
//     if (cell_value == -1) {
//       p = 0.5;
//     } else {
//       p = static_cast<double>(cell_value) / 100.0;
//     }
//     if (p <= 0.0 || p >= 1.0)
//       return 0.0;
//     return -(p * std::log(p) + (1 - p) * std::log(1 - p));
//   }

//   void occupancyCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg) {
//     occupancy_grid_ = msg;
//     processOccupancyGrid(msg);
//   }

//   void occupiedNodesCallback(const geometry_msgs::msg::PoseArray::SharedPtr msg) {
//     occupied_nodes_ = msg;
//   }

//   void processOccupancyGrid(const nav_msgs::msg::OccupancyGrid::SharedPtr msg) {
//     int width = msg->info.width;
//     int height = msg->info.height;
//     double resolution = msg->info.resolution;
//     double origin_x = msg->info.origin.position.x;
//     double origin_y = msg->info.origin.position.y;

//     std::vector<geometry_msgs::msg::Point> frontier_points;
//     std::vector<double> frontier_local_entropies;

//     // Calcular la entropía total del mapa
//     double total_entropy_sum = 0.0;
//     for (size_t i = 0; i < msg->data.size(); ++i) {
//       total_entropy_sum += computeCellEntropy(msg->data[i]);
//     }
//     double total_entropy = total_entropy_sum / msg->data.size();

//     // Recorrer la grilla para detectar celdas frontera
//     for (int y = 0; y < height; ++y) {
//       for (int x = 0; x < width; ++x) {
//         int index = y * width + x;
//         int8_t cell_value = msg->data[index];
//         if (cell_value == 0) {  // celda libre
//           bool is_frontier = false;
//           for (int dy = -1; dy <= 1 && !is_frontier; ++dy) {
//             for (int dx = -1; dx <= 1 && !is_frontier; ++dx) {
//               if (dx == 0 && dy == 0)
//                 continue;
//               int nx = x + dx;
//               int ny = y + dy;
//               if (nx < 0 || nx >= width || ny < 0 || ny >= height)
//                 continue;
//               int n_index = ny * width + nx;
//               if (msg->data[n_index] == -1) {
//                 is_frontier = true;
//               }
//             }
//           }
//           if (is_frontier) {
//             geometry_msgs::msg::Point pt;
//             pt.x = origin_x + (x + 0.5) * resolution;
//             pt.y = origin_y + (y + 0.5) * resolution;
//             pt.z = 0.0;
//             frontier_points.push_back(pt);

//             double local_entropy_sum = 0.0;
//             int count = 0;
//             for (int dy = -1; dy <= 1; ++dy) {
//               for (int dx = -1; dx <= 1; ++dx) {
//                 int nx = x + dx;
//                 int ny = y + dy;
//                 if (nx < 0 || nx >= width || ny < 0 || ny >= height)
//                   continue;
//                 int n_index = ny * width + nx;
//                 local_entropy_sum += computeCellEntropy(msg->data[n_index]);
//                 ++count;
//               }
//             }
//             double local_entropy = (count > 0) ? local_entropy_sum / count : 0.0;
//             frontier_local_entropies.push_back(local_entropy);
//           }
//         }
//       }
//     }

//     // Publicar los puntos frontera originales (PoseArray)
//     geometry_msgs::msg::PoseArray frontier_poses;
//     frontier_poses.header = msg->header;
//     for (const auto &pt : frontier_points) {
//       geometry_msgs::msg::Pose pose;
//       pose.position = pt;
//       pose.orientation.w = 1.0;
//       frontier_poses.poses.push_back(pose);
//     }
//     frontier_points_pub_->publish(frontier_poses);

//     // Publicar marcador para visualizar los puntos frontera originales
//     visualization_msgs::msg::Marker marker;
//     marker.header.frame_id = msg->header.frame_id;
//     marker.header.stamp = this->now();
//     marker.ns = "frontier_boundary";
//     marker.id = 0;
//     marker.type = visualization_msgs::msg::Marker::SPHERE_LIST;
//     marker.action = visualization_msgs::msg::Marker::ADD;
//     marker.scale.x = resolution;
//     marker.scale.y = resolution;
//     marker.scale.z = resolution;
//     marker.color.r = 1.0;
//     marker.color.g = 0.0;
//     marker.color.b = 0.0;
//     marker.color.a = 1.0;
//     marker.lifetime = rclcpp::Duration(0, 0);
//     marker.points = frontier_points;
//     marker_pub_->publish(marker);

//     // Publicar entropías locales y total
//     std_msgs::msg::Float64MultiArray entropy_array_msg;
//     entropy_array_msg.data = frontier_local_entropies;
//     frontier_entropy_pub_->publish(entropy_array_msg);

//     std_msgs::msg::Float64 total_entropy_msg;
//     total_entropy_msg.data = total_entropy;
//     total_entropy_pub_->publish(total_entropy_msg);
//     RCLCPP_INFO(this->get_logger(), "Entropía total del mapa: %.3f", total_entropy);

//     // Combinar posición y entropía en un solo vector
//     std::vector<std::pair<geometry_msgs::msg::Point, double>> frontier_points_with_entropy;
//     if (frontier_points.size() == frontier_local_entropies.size()) {
//       for (size_t i = 0; i < frontier_points.size(); ++i) {
//         frontier_points_with_entropy.push_back(std::make_pair(frontier_points[i], frontier_local_entropies[i]));
//       }
//     }
//     // Clustering y publicación de safe frontier points y sus entropías
//     clusterAndPublishFrontiers(frontier_points_with_entropy);
//   }

//   // Función de clustering que trabaja con pares (punto, entropía)
//   void clusterAndPublishFrontiers(const std::vector<std::pair<geometry_msgs::msg::Point, double>> &frontier_points_with_entropy) {
//     // Agrupar en clusters simples
//     std::vector<std::vector<std::pair<geometry_msgs::msg::Point, double>>> clusters;
//     for (const auto &pt_pair : frontier_points_with_entropy) {
//       const auto &pt = pt_pair.first;
//       bool added = false;
//       for (auto &cluster : clusters) {
//         geometry_msgs::msg::Point centroid = computeCentroidFromPairs(cluster);
//         if (euclideanDistance(pt, centroid) < CLUSTER_DISTANCE_THRESHOLD) {
//           cluster.push_back(pt_pair);
//           added = true;
//           break;
//         }
//       }
//       if (!added) {
//         clusters.push_back({pt_pair});
//       }
//     }

//     // Calcular centroide y entropía promedio de cada cluster
//     std::vector<geometry_msgs::msg::Point> safe_centroids;
//     std::vector<double> safe_entropies;
//     for (const auto &cluster : clusters) {
//       geometry_msgs::msg::Point centroid = computeCentroidFromPairs(cluster);
//       double sum_entropy = 0.0;
//       for (const auto &pair : cluster) {
//         sum_entropy += pair.second;
//       }
//       double avg_entropy = sum_entropy / cluster.size();
//       // Filtrar el centroide si está muy cerca de algún obstáculo
//       bool safe = true;
//       if (occupied_nodes_ != nullptr) {
//         for (const auto &obs_pose : occupied_nodes_->poses) {
//           if (euclideanDistance(centroid, obs_pose.position) < SAFE_DISTANCE_THRESHOLD) {
//             safe = false;
//             break;
//           }
//         }
//       }
//       if (safe) {
//         safe_centroids.push_back(centroid);
//         safe_entropies.push_back(avg_entropy);
//       }
//     }

//     // Publicar safe frontier points como PoseArray
//     geometry_msgs::msg::PoseArray safe_frontier_poses;
//     safe_frontier_poses.header.stamp = this->now();
//     safe_frontier_poses.header.frame_id = "map";
//     for (const auto &pt : safe_centroids) {
//       geometry_msgs::msg::Pose pose;
//       pose.position = pt;
//       pose.orientation.w = 1.0;
//       safe_frontier_poses.poses.push_back(pose);
//     }
//     safe_frontier_pub_->publish(safe_frontier_poses);

//     // Publicar marcador para safe frontier points (usando un publicador separado)
//     visualization_msgs::msg::Marker safe_marker;
//     safe_marker.header.frame_id = "map";
//     safe_marker.header.stamp = this->now();
//     safe_marker.ns = "safe_frontier";
//     safe_marker.id = 1;
//     safe_marker.type = visualization_msgs::msg::Marker::SPHERE_LIST;
//     safe_marker.action = visualization_msgs::msg::Marker::ADD;
//     safe_marker.scale.x = 0.2;
//     safe_marker.scale.y = 0.2;
//     safe_marker.scale.z = 0.2;
//     safe_marker.color.r = 0.0;
//     safe_marker.color.g = 1.0;
//     safe_marker.color.b = 0.0;
//     safe_marker.color.a = 1.0;
//     for (const auto &pt : safe_centroids) {
//       safe_marker.points.push_back(pt);
//     }
//     safe_marker_pub_->publish(safe_marker);

//     // Publicar las entropías de los safe frontier points
//     std_msgs::msg::Float64MultiArray safe_entropy_array;
//     safe_entropy_array.data = safe_entropies;
//     safe_frontier_entropy_pub_->publish(safe_entropy_array);

//     // Calcular y publicar la entropía total de los safe frontier points (promedio)
//     double total_safe_entropy = 0.0;
//     if (!safe_entropies.empty()) {
//       for (double e : safe_entropies) {
//         total_safe_entropy += e;
//       }
//       total_safe_entropy /= safe_entropies.size();
//     }
//     std_msgs::msg::Float64 total_safe_entropy_msg;
//     total_safe_entropy_msg.data = total_safe_entropy;
//     total_safe_entropy_pub_->publish(total_safe_entropy_msg);

//     RCLCPP_INFO(this->get_logger(), "Se publicaron %zu centroides seguros de %zu puntos frontera.",
//                 safe_centroids.size(), frontier_points_with_entropy.size());
//   }

//   // Función para calcular el centroide de un vector de pares (punto, entropía)
//   geometry_msgs::msg::Point computeCentroidFromPairs(const std::vector<std::pair<geometry_msgs::msg::Point, double>> &pairs) {
//     geometry_msgs::msg::Point centroid;
//     double sum_x = 0.0, sum_y = 0.0;
//     for (const auto &pair : pairs) {
//       sum_x += pair.first.x;
//       sum_y += pair.first.y;
//     }
//     centroid.x = sum_x / pairs.size();
//     centroid.y = sum_y / pairs.size();
//     centroid.z = 0.0;
//     return centroid;
//   }

//   // Función para calcular el centroide de un vector de puntos
//   geometry_msgs::msg::Point computeCentroid(const std::vector<geometry_msgs::msg::Point> &points) {
//     geometry_msgs::msg::Point centroid;
//     double sum_x = 0.0, sum_y = 0.0;
//     for (const auto &pt : points) {
//       sum_x += pt.x;
//       sum_y += pt.y;
//     }
//     centroid.x = sum_x / points.size();
//     centroid.y = sum_y / points.size();
//     centroid.z = 0.0;
//     return centroid;
//   }
// };

// int main(int argc, char ** argv)
// {
//   rclcpp::init(argc, argv);
//   auto node = std::make_shared<FrontierBoundaryNode>();
//   rclcpp::spin(node);
//   rclcpp::shutdown();
//   return 0;
// }


// #include <rclcpp/rclcpp.hpp>
// #include <nav_msgs/msg/occupancy_grid.hpp>
// #include <visualization_msgs/msg/marker.hpp>
// #include <geometry_msgs/msg/point.hpp>
// #include <geometry_msgs/msg/pose_array.hpp>
// #include <geometry_msgs/msg/pose.hpp>
// #include <std_msgs/msg/float64.hpp>
// #include <std_msgs/msg/float64_multi_array.hpp>
// #include <vector>
// #include <cmath>
// #include <algorithm>
// #include <utility>

// using std::placeholders::_1;

// class FrontierBoundaryNode : public rclcpp::Node {
// public:
//   FrontierBoundaryNode() : Node("frontier_boundary_node")
//   {
//     // --- Parámetros ---
//     free_value_           = this->declare_parameter<int>("free_value", 0);
//     unknown_value_        = this->declare_parameter<int>("unknown_value", -1);
//     occupied_threshold_   = this->declare_parameter<int>("occupied_threshold", 65);  // >= es ocupado
//     safe_distance_m_      = this->declare_parameter<double>("safe_distance_m", 3.0);
//     cluster_distance_m_   = this->declare_parameter<double>("cluster_distance_m", 2.0);

//     occupancy_sub_ = this->create_subscription<nav_msgs::msg::OccupancyGrid>(
//       "/occupancy", 10, std::bind(&FrontierBoundaryNode::occupancyCallback, this, _1));

//     // Publicadores
//     marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>("frontier_marker", 10);
//     frontier_points_pub_ = this->create_publisher<geometry_msgs::msg::PoseArray>("frontier_points", 10);
//     frontier_entropy_pub_ = this->create_publisher<std_msgs::msg::Float64MultiArray>("frontier_entropies", 10);
//     total_entropy_pub_ = this->create_publisher<std_msgs::msg::Float64>("total_entropy", 10);

//     safe_frontier_pub_ = this->create_publisher<geometry_msgs::msg::PoseArray>("safe_frontier_points", 10);
//     safe_frontier_entropy_pub_ = this->create_publisher<std_msgs::msg::Float64MultiArray>("safe_frontier_entropy", 10);
//     safe_marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>("safe_frontier_marker", 10);
//     total_safe_entropy_pub_ = this->create_publisher<std_msgs::msg::Float64>("total_safe_entropy", 10);

//     RCLCPP_INFO(this->get_logger(), "Nodo de frontera basado solo en OccupancyGrid iniciado.");
//   }

// private:
//   // Suscriptor
//   rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr occupancy_sub_;

//   // Publicadores
//   rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_pub_;
//   rclcpp::Publisher<geometry_msgs::msg::PoseArray>::SharedPtr frontier_points_pub_;
//   rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr frontier_entropy_pub_;
//   rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr total_entropy_pub_;

//   rclcpp::Publisher<geometry_msgs::msg::PoseArray>::SharedPtr safe_frontier_pub_;
//   rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr safe_frontier_entropy_pub_;
//   rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr safe_marker_pub_;
//   rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr total_safe_entropy_pub_;

//   // Estado
//   nav_msgs::msg::OccupancyGrid::SharedPtr occupancy_grid_;

//   // Parámetros
//   int free_value_;
//   int unknown_value_;
//   int occupied_threshold_;
//   double safe_distance_m_;
//   double cluster_distance_m_;

//   // --- Utilidades ---
//   double computeCellEntropy(int8_t cell_value) const {
//     double p;
//     if (cell_value == unknown_value_) {
//       p = 0.5;
//     } else {
//       // cell_value esperado en [0..100]
//       p = std::clamp(static_cast<double>(cell_value) / 100.0, 0.0, 1.0);
//     }
//     if (p <= 0.0 || p >= 1.0) return 0.0;
//     return -(p * std::log(p) + (1 - p) * std::log(1 - p));
//   }

//   inline bool inBounds(int x, int y, int w, int h) const {
//     return (x >= 0 && x < w && y >= 0 && y < h);
//   }

//   bool isFrontierCell(const nav_msgs::msg::OccupancyGrid &g, int x, int y) const {
//     int w = g.info.width;
//     int h = g.info.height;
//     int idx = y * w + x;
//     if (!inBounds(x,y,w,h)) return false;

//     // Libre
//     if (g.data[idx] != free_value_) return false;

//     // Vecino desconocido 8-conexo
//     for (int dy = -1; dy <= 1; ++dy) {
//       for (int dx = -1; dx <= 1; ++dx) {
//         if (dx == 0 && dy == 0) continue;
//         int nx = x + dx, ny = y + dy;
//         if (!inBounds(nx, ny, w, h)) continue;
//         if (g.data[ny * w + nx] == unknown_value_) return true;
//       }
//     }
//     return false;
//   }

//   // ¿Hay alguna celda ocupada a menos de 'radius_m' del punto mundo?
//   bool occupiedWithinRadiusM(const nav_msgs::msg::OccupancyGrid &g,
//                              const geometry_msgs::msg::Point &pt_world,
//                              double radius_m) const
//   {
//     const double res = g.info.resolution;
//     const double ox = g.info.origin.position.x;
//     const double oy = g.info.origin.position.y;

//     int w = g.info.width;
//     int h = g.info.height;

//     // Centro del punto en índices de celda
//     int cx = static_cast<int>(std::floor((pt_world.x - ox) / res));
//     int cy = static_cast<int>(std::floor((pt_world.y - oy) / res));
//     if (!inBounds(cx, cy, w, h)) return false; // fuera del mapa → no vetamos

//     int r_cells = std::max(1, static_cast<int>(std::ceil(radius_m / res)));

//     int x0 = std::max(0, cx - r_cells);
//     int x1 = std::min(w - 1, cx + r_cells);
//     int y0 = std::max(0, cy - r_cells);
//     int y1 = std::min(h - 1, cy + r_cells);

//     double r2 = radius_m * radius_m;

//     for (int y = y0; y <= y1; ++y) {
//       for (int x = x0; x <= x1; ++x) {
//         int idx = y * w + x;
//         int8_t v = g.data[idx];
//         if (v >= occupied_threshold_) {
//           // coordenada mundo del centro de la celda
//           double wx = ox + (x + 0.5) * res;
//           double wy = oy + (y + 0.5) * res;
//           double dx = wx - pt_world.x;
//           double dy = wy - pt_world.y;
//           if (dx*dx + dy*dy <= r2) return true;
//         }
//       }
//     }
//     return false;
//   }

//   double localEntropy3x3(const nav_msgs::msg::OccupancyGrid &g, int x, int y) const {
//     int w = g.info.width;
//     int h = g.info.height;
//     double sum = 0.0;
//     int cnt = 0;
//     for (int dy = -1; dy <= 1; ++dy) {
//       for (int dx = -1; dx <= 1; ++dx) {
//         int nx = x + dx, ny = y + dy;
//         if (!inBounds(nx, ny, w, h)) continue;
//         sum += computeCellEntropy(g.data[ny * w + nx]);
//         ++cnt;
//       }
//     }
//     return (cnt > 0) ? (sum / cnt) : 0.0;
//   }

//   geometry_msgs::msg::Point cellToWorld(const nav_msgs::msg::OccupancyGrid &g, int x, int y) const {
//     geometry_msgs::msg::Point p;
//     p.x = g.info.origin.position.x + (x + 0.5) * g.info.resolution;
//     p.y = g.info.origin.position.y + (y + 0.5) * g.info.resolution;
//     p.z = 0.0;
//     return p;
//   }

//   static double euclideanDistance(const geometry_msgs::msg::Point &a, const geometry_msgs::msg::Point &b) {
//     return std::hypot(b.x - a.x, b.y - a.y);
//   }

//   geometry_msgs::msg::Point centroidOf(const std::vector<std::pair<geometry_msgs::msg::Point,double>> &pairs) const {
//     geometry_msgs::msg::Point c;
//     double sx=0.0, sy=0.0;
//     for (auto &pr : pairs) { sx += pr.first.x; sy += pr.first.y; }
//     c.x = sx / pairs.size();
//     c.y = sy / pairs.size();
//     c.z = 0.0;
//     return c;
//   }

//   // --- Pipeline ---
//   void occupancyCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg) {
//     occupancy_grid_ = msg;
//     processOccupancyGrid(*msg);
//   }

//   void processOccupancyGrid(const nav_msgs::msg::OccupancyGrid &g) {
//     const int w = g.info.width;
//     const int h = g.info.height;

//     std::vector<geometry_msgs::msg::Point> frontier_points;
//     std::vector<double> frontier_local_entropies;

//     // Entropía total del mapa
//     double total_entropy_sum = 0.0;
//     for (size_t i = 0; i < g.data.size(); ++i) total_entropy_sum += computeCellEntropy(g.data[i]);
//     double total_entropy = (g.data.empty() ? 0.0 : total_entropy_sum / g.data.size());

//     // Detección de fronteras + entropía local
//     for (int y = 0; y < h; ++y) {
//       for (int x = 0; x < w; ++x) {
//         if (!isFrontierCell(g, x, y)) continue;
//         auto pt = cellToWorld(g, x, y);
//         double le = localEntropy3x3(g, x, y);
//         frontier_points.push_back(pt);
//         frontier_local_entropies.push_back(le);
//       }
//     }

//     // Publicación de fronteras "raw"
//     geometry_msgs::msg::PoseArray frontier_poses;
//     frontier_poses.header = g.header;
//     for (const auto &pt : frontier_points) {
//       geometry_msgs::msg::Pose p; p.position = pt; p.orientation.w = 1.0;
//       frontier_poses.poses.push_back(p);
//     }
//     frontier_points_pub_->publish(frontier_poses);

//     visualization_msgs::msg::Marker mk;
//     mk.header.frame_id = g.header.frame_id;
//     mk.header.stamp = this->now();
//     mk.ns = "frontier_boundary";
//     mk.id = 0;
//     mk.type = visualization_msgs::msg::Marker::SPHERE_LIST;
//     mk.action = visualization_msgs::msg::Marker::ADD;
//     mk.scale.x = g.info.resolution;
//     mk.scale.y = g.info.resolution;
//     mk.scale.z = g.info.resolution;
//     mk.color.r = 1.0; mk.color.g = 0.0; mk.color.b = 0.0; mk.color.a = 1.0;
//     mk.lifetime = rclcpp::Duration(0,0);
//     mk.points = frontier_points;
//     marker_pub_->publish(mk);

//     std_msgs::msg::Float64MultiArray ent_arr; ent_arr.data = frontier_local_entropies;
//     frontier_entropy_pub_->publish(ent_arr);

//     std_msgs::msg::Float64 total_ent_msg; total_ent_msg.data = total_entropy;
//     total_entropy_pub_->publish(total_ent_msg);
//     RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 2000, "Entropía total del mapa: %.3f", total_entropy);

//     // Pares (punto, entropía) para clusterizar
//     std::vector<std::pair<geometry_msgs::msg::Point,double>> pairs;
//     pairs.reserve(frontier_points.size());
//     for (size_t i=0; i<frontier_points.size(); ++i) pairs.emplace_back(frontier_points[i], frontier_local_entropies[i]);

//     clusterAndPublishFrontiers(g, pairs);
//   }

//   void clusterAndPublishFrontiers(const nav_msgs::msg::OccupancyGrid &g,
//                                   const std::vector<std::pair<geometry_msgs::msg::Point,double>> &frontier_pairs)
//   {
//     // Clustering sencillo por distancia al centroide incremental
//     std::vector<std::vector<std::pair<geometry_msgs::msg::Point,double>>> clusters;
//     for (const auto &pr : frontier_pairs) {
//       const auto &pt = pr.first;
//       bool placed = false;
//       for (auto &cl : clusters) {
//         auto c = centroidOf(cl);
//         if (euclideanDistance(pt, c) < cluster_distance_m_) { cl.push_back(pr); placed = true; break; }
//       }
//       if (!placed) clusters.push_back({pr});
//     }

//     // Centroides + entropía promedio y filtrado por obstáculo cercano (solo grid)
//     std::vector<geometry_msgs::msg::Point> safe_centroids;
//     std::vector<double> safe_entropies;

//     for (const auto &cl : clusters) {
//       auto c = centroidOf(cl);
//       double sumE = 0.0; for (auto &pr : cl) sumE += pr.second;
//       double avgE = sumE / cl.size();

//       // Filtro de seguridad: descartar si hay ocupados cerca
//       if (occupiedWithinRadiusM(g, c, safe_distance_m_)) {
//         continue;
//       }

//       // (Opcional) Filtro anti “boquilla estrecha”:
//       // Si quieres endurecerlo más, añade un segundo anillo pequeño que exija
//       // que la fracción de libres en un radio r_free sea alta. De momento lo omitimos.

//       safe_centroids.push_back(c);
//       safe_entropies.push_back(avgE);
//     }

//     // Publicación SAFE
//     geometry_msgs::msg::PoseArray safe_pa;
//     safe_pa.header.frame_id = g.header.frame_id;
//     safe_pa.header.stamp = this->now();
//     for (const auto &pt : safe_centroids) { geometry_msgs::msg::Pose p; p.position=pt; p.orientation.w=1.0; safe_pa.poses.push_back(p); }
//     safe_frontier_pub_->publish(safe_pa);

//     visualization_msgs::msg::Marker smk;
//     smk.header.frame_id = g.header.frame_id;
//     smk.header.stamp = this->now();
//     smk.ns = "safe_frontier";
//     smk.id = 1;
//     smk.type = visualization_msgs::msg::Marker::SPHERE_LIST;
//     smk.action = visualization_msgs::msg::Marker::ADD;
//     smk.scale.x = 0.2; smk.scale.y = 0.2; smk.scale.z = 0.2;
//     smk.color.r = 0.0; smk.color.g = 1.0; smk.color.b = 0.0; smk.color.a = 1.0;
//     smk.points = safe_centroids;
//     safe_marker_pub_->publish(smk);

//     std_msgs::msg::Float64MultiArray safe_ent_arr; safe_ent_arr.data = safe_entropies;
//     safe_frontier_entropy_pub_->publish(safe_ent_arr);

//     double t_safe = 0.0; if (!safe_entropies.empty()) { for (double e : safe_entropies) t_safe += e; t_safe /= safe_entropies.size(); }
//     std_msgs::msg::Float64 t_safe_msg; t_safe_msg.data = t_safe;
//     total_safe_entropy_pub_->publish(t_safe_msg);

//     RCLCPP_INFO(this->get_logger(), "SAFE centroides publicados: %zu (de %zu puntos frontera).",
//                 safe_centroids.size(), frontier_pairs.size());
//   }
// };

// int main(int argc, char ** argv)
// {
//   rclcpp::init(argc, argv);
//   auto node = std::make_shared<FrontierBoundaryNode>();
//   rclcpp::spin(node);
//   rclcpp::shutdown();
//   return 0;
// }


#include <rclcpp/rclcpp.hpp>
#include <nav_msgs/msg/occupancy_grid.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <geometry_msgs/msg/pose_array.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <std_msgs/msg/float64.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>
#include <vector>
#include <cmath>
#include <algorithm>
#include <utility>
#include <unordered_map>

using std::placeholders::_1;

class FrontierBoundaryNode : public rclcpp::Node {
public:
  FrontierBoundaryNode() : Node("frontier_boundary_node")
  {
    // --- Parámetros ---
    free_value_             = this->declare_parameter<int>("free_value", 0);
    unknown_value_          = this->declare_parameter<int>("unknown_value", -1);
    occupied_threshold_     = this->declare_parameter<int>("occupied_threshold", 65); // >= es ocupado
    safe_distance_m_        = this->declare_parameter<double>("safe_distance_m", 3.0);
    cluster_distance_m_     = this->declare_parameter<double>("cluster_distance_m", 2.0);

    // NUEVOS
    treat_oob_as_unknown_   = this->declare_parameter<bool>("treat_oob_as_unknown", true);
    min_unknown_neighbors_  = this->declare_parameter<int>("min_unknown_neighbors", 2);
    voxel_size_m_           = this->declare_parameter<double>("voxel_size_m", 0.8);
    max_safe_frontiers_     = this->declare_parameter<int>("max_safe_frontiers", 60);

    occupancy_sub_ = this->create_subscription<nav_msgs::msg::OccupancyGrid>(
      "/occupancy", 10, std::bind(&FrontierBoundaryNode::occupancyCallback, this, _1));

    // Publicadores
    marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>("frontier_marker", 10);
    frontier_points_pub_ = this->create_publisher<geometry_msgs::msg::PoseArray>("frontier_points", 10);
    frontier_entropy_pub_ = this->create_publisher<std_msgs::msg::Float64MultiArray>("frontier_entropies", 10);
    total_entropy_pub_ = this->create_publisher<std_msgs::msg::Float64>("total_entropy", 10);

    safe_frontier_pub_ = this->create_publisher<geometry_msgs::msg::PoseArray>("safe_frontier_points", 10);
    safe_frontier_entropy_pub_ = this->create_publisher<std_msgs::msg::Float64MultiArray>("safe_frontier_entropy", 10);
    safe_marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>("safe_frontier_marker", 10);
    total_safe_entropy_pub_ = this->create_publisher<std_msgs::msg::Float64>("total_safe_entropy", 10);

    RCLCPP_INFO(this->get_logger(), "Nodo de frontera (OccupancyGrid only) con borde-OOB y reducción de densidad.");
  }

private:
  // Suscriptor
  rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr occupancy_sub_;

  // Publicadores
  rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_pub_;
  rclcpp::Publisher<geometry_msgs::msg::PoseArray>::SharedPtr frontier_points_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr frontier_entropy_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr total_entropy_pub_;

  rclcpp::Publisher<geometry_msgs::msg::PoseArray>::SharedPtr safe_frontier_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr safe_frontier_entropy_pub_;
  rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr safe_marker_pub_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr total_safe_entropy_pub_;

  // Estado
  nav_msgs::msg::OccupancyGrid::SharedPtr occupancy_grid_;

  // Parámetros
  int free_value_;
  int unknown_value_;
  int occupied_threshold_;
  double safe_distance_m_;
  double cluster_distance_m_;
  bool treat_oob_as_unknown_;
  int min_unknown_neighbors_;
  double voxel_size_m_;
  int max_safe_frontiers_;

  // --- Utilidades ---
  double computeCellEntropy(int8_t cell_value) const {
    double p;
    if (cell_value == unknown_value_) {
      p = 0.5;
    } else {
      p = std::clamp(static_cast<double>(cell_value) / 100.0, 0.0, 1.0);
    }
    if (p <= 0.0 || p >= 1.0) return 0.0;
    return -(p * std::log(p) + (1 - p) * std::log(1 - p));
  }

  inline bool inBounds(int x, int y, int w, int h) const {
    return (x >= 0 && x < w && y >= 0 && y < h);
  }

  // Cuenta OOB como "unknown" si treat_oob_as_unknown_ == true
  bool isFrontierCell(const nav_msgs::msg::OccupancyGrid &g, int x, int y) const {
    int w = g.info.width;
    int h = g.info.height;
    if (!inBounds(x,y,w,h)) return false;

    int idx = y * w + x;
    if (g.data[idx] != free_value_) return false; // solo partimos desde libre

    int unknown_cnt = 0;
    for (int dy = -1; dy <= 1; ++dy) {
      for (int dx = -1; dx <= 1; ++dx) {
        if (dx == 0 && dy == 0) continue;
        int nx = x + dx, ny = y + dy;
        if (!inBounds(nx, ny, w, h)) {
          if (treat_oob_as_unknown_) ++unknown_cnt; // borde de mapa = desconocido virtual
          continue;
        }
        int nidx = ny * w + nx;
        if (g.data[nidx] == unknown_value_) ++unknown_cnt;
      }
    }
    return (unknown_cnt >= std::max(1, min_unknown_neighbors_));
  }

  geometry_msgs::msg::Point cellToWorld(const nav_msgs::msg::OccupancyGrid &g, int x, int y) const {
    geometry_msgs::msg::Point p;
    p.x = g.info.origin.position.x + (x + 0.5) * g.info.resolution;
    p.y = g.info.origin.position.y + (y + 0.5) * g.info.resolution;
    p.z = 0.0;
    return p;
  }

  static double euclideanDistance(const geometry_msgs::msg::Point &a, const geometry_msgs::msg::Point &b) {
    return std::hypot(b.x - a.x, b.y - a.y);
  }

  bool occupiedWithinRadiusM(const nav_msgs::msg::OccupancyGrid &g,
                             const geometry_msgs::msg::Point &pt_world,
                             double radius_m) const
  {
    const double res = g.info.resolution;
    const double ox = g.info.origin.position.x;
    const double oy = g.info.origin.position.y;

    int w = g.info.width;
    int h = g.info.height;

    int cx = static_cast<int>(std::floor((pt_world.x - ox) / res));
    int cy = static_cast<int>(std::floor((pt_world.y - oy) / res));
    if (!inBounds(cx, cy, w, h)) return false;

    int r_cells = std::max(1, static_cast<int>(std::ceil(radius_m / res)));
    int x0 = std::max(0, cx - r_cells);
    int x1 = std::min(w - 1, cx + r_cells);
    int y0 = std::max(0, cy - r_cells);
    int y1 = std::min(h - 1, cy + r_cells);
    double r2 = radius_m * radius_m;

    for (int y = y0; y <= y1; ++y) {
      for (int x = x0; x <= x1; ++x) {
        int idx = y * w + x;
        int8_t v = g.data[idx];
        if (v >= occupied_threshold_) {
          double wx = ox + (x + 0.5) * res;
          double wy = oy + (y + 0.5) * res;
          double dx = wx - pt_world.x;
          double dy = wy - pt_world.y;
          if (dx*dx + dy*dy <= r2) return true;
        }
      }
    }
    return false;
  }

  double localEntropy3x3(const nav_msgs::msg::OccupancyGrid &g, int x, int y) const {
    int w = g.info.width;
    int h = g.info.height;
    double sum = 0.0;
    int cnt = 0;
    for (int dy = -1; dy <= 1; ++dy) {
      for (int dx = -1; dx <= 1; ++dx) {
        int nx = x + dx, ny = y + dy;
        if (!inBounds(nx, ny, w, h)) continue;
        sum += computeCellEntropy(g.data[ny * w + nx]);
        ++cnt;
      }
    }
    return (cnt > 0) ? (sum / cnt) : 0.0;
  }

  // --- Reducción de densidad: voxel filter manteniendo el punto con mayor entropía por voxel ---
  void voxelFilterKeepBestEntropy(
      const std::vector<geometry_msgs::msg::Point> &in_pts,
      const std::vector<double> &in_ent,
      std::vector<geometry_msgs::msg::Point> &out_pts,
      std::vector<double> &out_ent) const
  {
    out_pts.clear(); out_ent.clear();
    if (in_pts.empty()) return;
    if (voxel_size_m_ <= 0.0) { out_pts = in_pts; out_ent = in_ent; return; }

    struct Key { int vx; int vy; bool operator==(const Key& o) const { return vx==o.vx && vy==o.vy; } };
    struct Hash { size_t operator()(Key const& k) const noexcept {
      return (static_cast<size_t>(static_cast<uint32_t>(k.vx))<<32) ^ static_cast<uint32_t>(k.vy);
    }};

    std::unordered_map<Key, std::pair<geometry_msgs::msg::Point,double>, Hash> best;

    for (size_t i=0; i<in_pts.size(); ++i) {
      const auto &p = in_pts[i];
      double e = in_ent[i];
      // El origen del voxel no importa mientras sea consistente; uso 0,0 del mundo
      int vx = static_cast<int>(std::floor(p.x / voxel_size_m_));
      int vy = static_cast<int>(std::floor(p.y / voxel_size_m_));
      Key k{vx, vy};
      auto it = best.find(k);
      if (it == best.end() || e > it->second.second) {
        best[k] = {p, e};
      }
    }
    out_pts.reserve(best.size());
    out_ent.reserve(best.size());
    for (auto &kv : best) {
      out_pts.push_back(kv.second.first);
      out_ent.push_back(kv.second.second);
    }
  }

  geometry_msgs::msg::Point centroidOf(const std::vector<std::pair<geometry_msgs::msg::Point,double>> &pairs) const {
    geometry_msgs::msg::Point c;
    double sx=0.0, sy=0.0;
    for (auto &pr : pairs) { sx += pr.first.x; sy += pr.first.y; }
    c.x = sx / pairs.size();
    c.y = sy / pairs.size();
    c.z = 0.0;
    return c;
  }

  void occupancyCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg) {
    occupancy_grid_ = msg;
    processOccupancyGrid(*msg);
  }

  void processOccupancyGrid(const nav_msgs::msg::OccupancyGrid &g) {
    const int w = g.info.width;
    const int h = g.info.height;

    std::vector<geometry_msgs::msg::Point> frontier_points_raw;
    std::vector<double> frontier_local_entropies_raw;

    // Entropía total del mapa
    double total_entropy_sum = 0.0;
    for (size_t i = 0; i < g.data.size(); ++i) total_entropy_sum += computeCellEntropy(g.data[i]);
    double total_entropy = (g.data.empty() ? 0.0 : total_entropy_sum / g.data.size());

    // Detección de fronteras + entropía local (OOB cuenta como desconocido)
    for (int y = 0; y < h; ++y) {
      for (int x = 0; x < w; ++x) {
        if (!isFrontierCell(g, x, y)) continue;
        auto pt = cellToWorld(g, x, y);
        double le = localEntropy3x3(g, x, y);
        frontier_points_raw.push_back(pt);
        frontier_local_entropies_raw.push_back(le);
      }
    }

    // --- Reducción de densidad (mantener mejor entropía por voxel) ---
    std::vector<geometry_msgs::msg::Point> frontier_points;
    std::vector<double> frontier_local_entropies;
    voxelFilterKeepBestEntropy(frontier_points_raw, frontier_local_entropies_raw,
                               frontier_points, frontier_local_entropies);

    // Publicación de fronteras "raw" (tras voxel, para no saturar RViz)
    geometry_msgs::msg::PoseArray frontier_poses;
    frontier_poses.header = g.header;
    for (const auto &pt : frontier_points) {
      geometry_msgs::msg::Pose p; p.position = pt; p.orientation.w = 1.0;
      frontier_poses.poses.push_back(p);
    }
    frontier_points_pub_->publish(frontier_poses);

    visualization_msgs::msg::Marker mk;
    mk.header.frame_id = g.header.frame_id;
    mk.header.stamp = this->now();
    mk.ns = "frontier_boundary";
    mk.id = 0;
    mk.type = visualization_msgs::msg::Marker::SPHERE_LIST;
    mk.action = visualization_msgs::msg::Marker::ADD;
    mk.scale.x = g.info.resolution;
    mk.scale.y = g.info.resolution;
    mk.scale.z = g.info.resolution;
    mk.color.r = 1.0; mk.color.g = 0.0; mk.color.b = 0.0; mk.color.a = 1.0;
    mk.lifetime = rclcpp::Duration(0,0);
    mk.points = frontier_points;
    marker_pub_->publish(mk);

    std_msgs::msg::Float64MultiArray ent_arr; ent_arr.data = frontier_local_entropies;
    frontier_entropy_pub_->publish(ent_arr);

    std_msgs::msg::Float64 total_ent_msg; total_ent_msg.data = total_entropy;
    total_entropy_pub_->publish(total_ent_msg);
    RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 2000, "Entropía total del mapa: %.3f", total_entropy);

    // Pares (punto, entropía) para clusterizar
    std::vector<std::pair<geometry_msgs::msg::Point,double>> pairs;
    pairs.reserve(frontier_points.size());
    for (size_t i=0; i<frontier_points.size(); ++i) pairs.emplace_back(frontier_points[i], frontier_local_entropies[i]);

    clusterAndPublishFrontiers(g, pairs);
  }

  void clusterAndPublishFrontiers(
      const nav_msgs::msg::OccupancyGrid &g,
      const std::vector<std::pair<geometry_msgs::msg::Point,double>> &frontier_pairs)
  {
    // Clustering sencillo por distancia al centroide incremental
    std::vector<std::vector<std::pair<geometry_msgs::msg::Point,double>>> clusters;
    clusters.reserve(frontier_pairs.size());
    for (const auto &pr : frontier_pairs) {
      const auto &pt = pr.first;
      bool placed = false;
      for (auto &cl : clusters) {
        auto c = centroidOf(cl);
        if (euclideanDistance(pt, c) < cluster_distance_m_) { cl.push_back(pr); placed = true; break; }
      }
      if (!placed) clusters.push_back({pr});
    }

    // Centroides + entropía promedio y filtrado por obstáculo cercano
    std::vector<std::pair<geometry_msgs::msg::Point,double>> safe_centroids_with_e;
    safe_centroids_with_e.reserve(clusters.size());

    for (const auto &cl : clusters) {
      auto c = centroidOf(cl);
      double sumE = 0.0; for (auto &pr : cl) sumE += pr.second;
      double avgE = sumE / cl.size();

      if (occupiedWithinRadiusM(g, c, safe_distance_m_)) {
        continue; // descarta zona insegura
      }
      safe_centroids_with_e.emplace_back(c, avgE);
    }

    // Ordenar por entropía (desc) y limitar cantidad
    std::sort(safe_centroids_with_e.begin(), safe_centroids_with_e.end(),
              [](const auto &a, const auto &b){ return a.second > b.second; });
    if (max_safe_frontiers_ > 0 && static_cast<int>(safe_centroids_with_e.size()) > max_safe_frontiers_) {
      safe_centroids_with_e.resize(max_safe_frontiers_);
    }

    // Publicación SAFE
    geometry_msgs::msg::PoseArray safe_pa;
    safe_pa.header.frame_id = g.header.frame_id;
    safe_pa.header.stamp = this->now();
    std::vector<geometry_msgs::msg::Point> safe_points_only; safe_points_only.reserve(safe_centroids_with_e.size());
    std::vector<double> safe_entropies_only; safe_entropies_only.reserve(safe_centroids_with_e.size());
    for (const auto &pe : safe_centroids_with_e) {
      geometry_msgs::msg::Pose p; p.position = pe.first; p.orientation.w = 1.0; safe_pa.poses.push_back(p);
      safe_points_only.push_back(pe.first);
      safe_entropies_only.push_back(pe.second);
    }
    safe_frontier_pub_->publish(safe_pa);

    visualization_msgs::msg::Marker smk;
    smk.header.frame_id = g.header.frame_id;
    smk.header.stamp = this->now();
    smk.ns = "safe_frontier";
    smk.id = 1;
    smk.type = visualization_msgs::msg::Marker::SPHERE_LIST;
    smk.action = visualization_msgs::msg::Marker::ADD;
    smk.scale.x = 0.2; smk.scale.y = 0.2; smk.scale.z = 0.2;
    smk.color.r = 0.0; smk.color.g = 1.0; smk.color.b = 0.0; smk.color.a = 1.0;
    smk.points = safe_points_only;
    safe_marker_pub_->publish(smk);

    std_msgs::msg::Float64MultiArray safe_ent_arr; safe_ent_arr.data = safe_entropies_only;
    safe_frontier_entropy_pub_->publish(safe_ent_arr);

    double t_safe = 0.0; if (!safe_entropies_only.empty()) { for (double e : safe_entropies_only) t_safe += e; t_safe /= safe_entropies_only.size(); }
    std_msgs::msg::Float64 t_safe_msg; t_safe_msg.data = t_safe;
    total_safe_entropy_pub_->publish(t_safe_msg);

    RCLCPP_INFO(this->get_logger(), "SAFE centroides publicados: %zu (clusters: %zu).",
                safe_points_only.size(), clusters.size());
  }
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<FrontierBoundaryNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
