

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_array.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <cmath>
#include <vector>
#include <algorithm>

class FilteredNavigationNodes : public rclcpp::Node
{
public:
  FilteredNavigationNodes() : Node("filtered_navigation_nodes")
  {
    // Declarar parámetros configurables:
    // obstacle_projection_radius: para filtrar nodos muy próximos a los obstáculos.
    // obstacle_ray_threshold: distancia mínima permitida desde el rayo a un obstáculo para considerarlo colisionado.
    this->declare_parameter("obstacle_projection_radius", 0.5);
    this->declare_parameter("obstacle_ray_threshold", 0.3);
    obstacle_projection_radius_ = this->get_parameter("obstacle_projection_radius").as_double();
    obstacle_ray_threshold_ = this->get_parameter("obstacle_ray_threshold").as_double();

    // Suscripción a los nodos navegables del suelo.
    ground_nodes_sub_ = this->create_subscription<geometry_msgs::msg::PoseArray>(
      "ground_navigation_nodes", 10,
      std::bind(&FilteredNavigationNodes::groundNodesCallback, this, std::placeholders::_1)
    );

    // Suscripción a los nodos de obstáculos (obtenidos de obstacle_navigation_nodes).
    obstacle_nodes_sub_ = this->create_subscription<geometry_msgs::msg::PoseArray>(
      "obstacle_navigation_nodes", 10,
      std::bind(&FilteredNavigationNodes::obstacleNodesCallback, this, std::placeholders::_1)
    );

    // Suscripción a la odometría para obtener la posición actual del robot.
    odom_sub_ = this->create_subscription<nav_msgs::msg::Odometry>(
      "odom", 10,
      std::bind(&FilteredNavigationNodes::odomCallback, this, std::placeholders::_1)
    );

    // Publicadores para los nodos filtrados y para la visualización (markers) en RViz.
    filtered_nodes_pub_ = this->create_publisher<geometry_msgs::msg::PoseArray>("filtered_navigation_nodes", 10);
    filtered_markers_pub_ = this->create_publisher<visualization_msgs::msg::MarkerArray>("filtered_nodes_markers", 10);

    RCLCPP_INFO(this->get_logger(), "Nodo FilteredNavigationNodes iniciado. Radio: %.2f, umbral de rayo: %.2f",
                obstacle_projection_radius_, obstacle_ray_threshold_);
  }

private:
  // Callback para recibir los nodos de obstáculos.
  void obstacleNodesCallback(const geometry_msgs::msg::PoseArray::SharedPtr msg)
  {
    obstacle_nodes_ = *msg;
  }

  // Callback para la odometría (posición del robot).
  void odomCallback(const nav_msgs::msg::Odometry::SharedPtr msg)
  {
    robot_pose_ = msg->pose.pose;
    robot_pose_received_ = true;
  }

  // Función que calcula la distancia desde un punto p a la línea (segmento) definido por v y w.
  double distancePointToSegment(const geometry_msgs::msg::Point &p,
                                const geometry_msgs::msg::Point &v,
                                const geometry_msgs::msg::Point &w)
  {
    double vx = w.x - v.x;
    double vy = w.y - v.y;
    double lengthSq = vx * vx + vy * vy;
    if (lengthSq == 0)
      return std::hypot(p.x - v.x, p.y - v.y);
    double t = ((p.x - v.x) * vx + (p.y - v.y) * vy) / lengthSq;
    t = std::max(0.0, std::min(1.0, t));
    double projX = v.x + t * vx;
    double projY = v.y + t * vy;
    return std::hypot(p.x - projX, p.y - projY);
  }

  // Función para verificar si el rayo (segmento entre start y end) está libre de colisiones
  // con obstáculos (usando los nodos de obstacle_navigation_nodes).
  bool isRayCollisionFree(const geometry_msgs::msg::Point &start,
                          const geometry_msgs::msg::Point &end)
  {
    // Iterar sobre cada obstáculo disponible.
    for (const auto &obs_pose : obstacle_nodes_.poses)
    {
      double dist = distancePointToSegment(obs_pose.position, start, end);
      if (dist < obstacle_ray_threshold_)
      {
        return false;  // Se detectó un obstáculo lo suficientemente cerca del rayo.
      }
    }
    return true;
  }

  // Callback para los nodos navegables del suelo, integrando ambos filtros.
  void groundNodesCallback(const geometry_msgs::msg::PoseArray::SharedPtr msg)
  {
    geometry_msgs::msg::PoseArray filtered_nodes;
    filtered_nodes.header = msg->header;
    visualization_msgs::msg::MarkerArray markers;
    int marker_id = 0;

    if (!robot_pose_received_)
    {
      RCLCPP_WARN(this->get_logger(), "No se ha recibido la odometría del robot; se omite la verificación de colisión en el rayo.");
    }

    // Recorrer cada nodo navegable.
    for (const auto &ground_pose : msg->poses)
    {
      bool keep_node = true;
      
      // Filtro 1: Descarta el nodo si está muy cerca de algún obstáculo.
      for (const auto &obs_pose : obstacle_nodes_.poses)
      {
        double dx = ground_pose.position.x - obs_pose.position.x;
        double dy = ground_pose.position.y - obs_pose.position.y;
        double distance = std::sqrt(dx * dx + dy * dy);
        if (distance < obstacle_projection_radius_)
        {
          keep_node = false;
          break;
        }
      }
      
      // Filtro 2: Comprueba la línea de visión (rayo) desde la posición del robot hasta el nodo.
      if (keep_node && robot_pose_received_)
      {
        if (!isRayCollisionFree(robot_pose_.position, ground_pose.position))
        {
          keep_node = false;
        }
      }
      
      if (keep_node)
      {
        filtered_nodes.poses.push_back(ground_pose);
        
        // Crear marker para visualización en RViz.
        visualization_msgs::msg::Marker marker;
        marker.header = msg->header;
        marker.ns = "filtered_nodes";
        marker.id = marker_id++;
        marker.type = visualization_msgs::msg::Marker::SPHERE;
        marker.action = visualization_msgs::msg::Marker::ADD;
        marker.pose = ground_pose;
        marker.scale.x = 0.1;
        marker.scale.y = 0.1;
        marker.scale.z = 0.1;
        marker.color.r = 0.0;
        marker.color.g = 1.0;
        marker.color.b = 0.0;
        marker.color.a = 1.0;
        marker.lifetime = rclcpp::Duration(0, 0);
        markers.markers.push_back(marker);
      }
    }
    
    filtered_nodes_pub_->publish(filtered_nodes);
    filtered_markers_pub_->publish(markers);
  }

  // Variables miembro
  double obstacle_projection_radius_;
  double obstacle_ray_threshold_;
  geometry_msgs::msg::PoseArray obstacle_nodes_;
  geometry_msgs::msg::Pose robot_pose_;
  bool robot_pose_received_ = false;
  
  // Suscriptores
  rclcpp::Subscription<geometry_msgs::msg::PoseArray>::SharedPtr ground_nodes_sub_;
  rclcpp::Subscription<geometry_msgs::msg::PoseArray>::SharedPtr obstacle_nodes_sub_;
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub_;
  
  // Publicadores
  rclcpp::Publisher<geometry_msgs::msg::PoseArray>::SharedPtr filtered_nodes_pub_;
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr filtered_markers_pub_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<FilteredNavigationNodes>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
