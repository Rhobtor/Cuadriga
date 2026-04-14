/**
 * @file path_manager.hpp
 * @brief Nodo ROS 2 (Lifecycle) para gestión de rutas en el rover.
 *
 * Este paquete expone un nodo llamado típicamente `path_manager_node` (namespace `ARGJ801`)
 * que centraliza la lectura/escritura y distribución de rutas. En el stack del rover:
 *
 * - Recibe rutas en lat/lon (p.ej. frame_id="ll") y mantiene un `robot_path` interno.
 * - Publica rutas en un "fixed frame" (nav_msgs/Path) para que controladores/seguidores
 *   puedan consumirlas.
 * - Expone varios servicios (ver includes en `path_manager_interfaces/srv/`) para:
 *   - leer/escribir rutas de fichero,
 *   - consultar la ruta actual,
 *   - solicitar rutas en diferentes frames,
 *   - recibir una ruta desde otro nodo,
 *   - invocar un planificador (DEMAIAS o emergencia).
 *
 * Nota sobre unidades/frames:
 * - Muchas operaciones de conversión usan GeographicLib para pasar entre UTM y lat/lon.
 * - Este header no fija una convención única (depende del origen de la ruta); por eso
 *   es importante documentar el frame_id y la semántica de cada servicio.
 */

#ifndef PATH_MANAGER_HPP
#define PATH_MANAGER_HPP

#include <chrono>
#include <memory>
#include <string>
#include <fstream>
#include <iomanip> 
#include <ament_index_cpp/get_package_share_directory.hpp>
#include <GeographicLib/Geocentric.hpp>
#include <GeographicLib/UTMUPS.hpp>
#include "ament_index_cpp/get_package_share_directory.hpp"
#include <iostream>
#include <filesystem>
#include "lifecycle_msgs/msg/transition.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/publisher.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_srvs/srv/trigger.hpp"
#include "path_manager_interfaces/srv/read_path_from_file.hpp"
#include "path_manager_interfaces/srv/write_path_to_file.hpp"
#include "path_manager_interfaces/srv/return_robot_path.hpp"
#include "path_manager_interfaces/srv/get_ll_path.hpp"
#include "path_manager_interfaces/srv/get_fix_frame_path.hpp"
#include "path_manager_interfaces/srv/plan_path.hpp"
#include "path_manager_interfaces/srv/robot_path.hpp"
#include "path_manager_interfaces/srv/assist_emergency.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "nav_msgs/msg/path.hpp"
using namespace std::chrono_literals;
namespace DEMAIAS
{

/**
 * @brief API del planificador DEMAIAS.
 *
 * Devuelve un par de rutas (p.ej. ruta UTM + ruta auxiliar). La implementación real
 * vive en la carpeta `src/Demaias_planner/`.
 */
std::pair <nav_msgs::msg::Path, nav_msgs::msg::Path> planner(char* DEM, char* mapping, char* LTL);

}

/**
 * @class PathManager
 * @brief LifecycleNode que ofrece servicios y topics para gestión de rutas.
 *
 * Ciclo de vida:
 * - on_configure(): declara parámetros, crea publishers/subscriptions/services.
 * - on_activate(): activa publishers lifecycle (si aplica) y deja el nodo operativo.
 *
 * Parámetros (nombres típicos, ver `argj801_setup/config/J8_params.yaml`):
 * - localization_method (string)
 * - read_path_service, write_path_service, return_path_service
 * - get_ll_path_service, get_fix_frame_path_service
 * - fixed_frame_topic, ll_path_topic
 */
class PathManager : public rclcpp_lifecycle::LifecycleNode {
public:
  /**
   * @brief Constructor.
   * @param node_name Nombre del nodo (p.ej. "path_manager_node").
   * @param intra_process_comms Si true, habilita intra-process (optimización).
   */
  explicit PathManager(const std::string & node_name, bool intra_process_comms = false);

  /**
   * @brief Publicación periódica (si se usa timer_).
   *
   * Actualmente está vacío en el código; se deja como hook para futuras
   * publicaciones (p.ej. latched path, estado, debug).
   */
  void publish();

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_configure(const rclcpp_lifecycle::State &) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_activate(const rclcpp_lifecycle::State & state) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_deactivate(const rclcpp_lifecycle::State & state) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_cleanup(const rclcpp_lifecycle::State &) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_shutdown(const rclcpp_lifecycle::State & state) override;

  void ll_path_callback(const std::shared_ptr<nav_msgs::msg::Path> msg);

  /**
   * @brief Servicio: leer ruta de fichero.
   *
   * Implementación en .cpp. Espera un path/identificador y rellena `robot_path`.
   */
  void readPath(const std::shared_ptr<path_manager_interfaces::srv::ReadPathFromFile::Request> req,
                const std::shared_ptr<path_manager_interfaces::srv::ReadPathFromFile::Response> res);

  /**
   * @brief Devuelve carpeta base donde se guardan/leen rutas.
   *
   * Suele apuntar a un directorio dentro de share/ del paquete o a un path de trabajo.
   */
  std::string getPathFolder();

  /**
   * @brief Servicio: planificar ruta.
   *
   * Expuesto como `PlanPath`. Puede usar DEMAIAS o lógica simplificada según el código.
   */
  void planPath(const std::shared_ptr<path_manager_interfaces::srv::PlanPath::Request> req,
                           std::shared_ptr<path_manager_interfaces::srv::PlanPath::Response> res);

  /**
   * @brief Servicio: escribir ruta a fichero.
   */
  void writePath(const std::shared_ptr<path_manager_interfaces::srv::WritePathToFile::Request> req,
                 const std::shared_ptr<path_manager_interfaces::srv::WritePathToFile::Response> res);

  /**
   * @brief Servicio: obtener ruta en coordenadas lat/lon.
   */
  void getLatLonPath(const std::shared_ptr<path_manager_interfaces::srv::GetLLPath::Request> req,
                     const std::shared_ptr<path_manager_interfaces::srv::GetLLPath::Response> res);

  /**
   * @brief Servicio: obtener ruta en el "fixed frame" usado por el controlador.
   */
  void getFFPath(const std::shared_ptr<path_manager_interfaces::srv::GetFixFramePath::Request> req,
                 const std::shared_ptr<path_manager_interfaces::srv::GetFixFramePath::Response> res);

  /**
   * @brief Servicio: devolver la ruta interna que seguirá el robot.
   */
  void returnPath(const std::shared_ptr<path_manager_interfaces::srv::ReturnRobotPath::Request> req,
                  const std::shared_ptr<path_manager_interfaces::srv::ReturnRobotPath::Response> res);

  /**
   * @brief Conversión lat/lon a UTM.
   */
  void latLonToUTM(double latitude, double longitude, double& utmNorthing, double& utmEasting, int& zone, bool& isNorth);

  /**
   * @brief Servicio: recibir una ruta enviada por otro nodo (RobotPath).
   */
  void receivePath(const std::shared_ptr<path_manager_interfaces::srv::RobotPath::Request> req,
                           std::shared_ptr<path_manager_interfaces::srv::RobotPath::Response> res);

  /**
   * @brief Servicio de emergencia: planificar/retornar una ruta de asistencia.
   */
  void assistEmergency(const std::shared_ptr<path_manager_interfaces::srv::AssistEmergency::Request> req,
                           std::shared_ptr<path_manager_interfaces::srv::AssistEmergency::Response> res);

  /**
   * @brief Conversión a ECEF (Earth-Centered, Earth-Fixed).
   */
  geometry_msgs::msg::PoseStamped ConvertToECEF(double latitude, double longitude, double altitude);

  /**
   * @brief Conversión UTM -> lat/lon usando GeographicLib.
   */
  void ConvertUTMToLatLon(double utmNorthing, double utmEasting, int zone, bool north, double& lat, double& lon);


private:
  /// Timer de publicación periódica (se inicializa en on_configure).
  std::shared_ptr<rclcpp::TimerBase> timer_;

  // ---------------------------------------------------------------------------
  // Servicios ROS
  // ---------------------------------------------------------------------------
  rclcpp::Service<path_manager_interfaces::srv::WritePathToFile>::SharedPtr writePathServ;
  rclcpp::Service<path_manager_interfaces::srv::ReadPathFromFile>::SharedPtr readPathServ;
  rclcpp::Service<path_manager_interfaces::srv::ReturnRobotPath>::SharedPtr getRobotPath;
  rclcpp::Service<path_manager_interfaces::srv::GetLLPath>::SharedPtr getLLPath;
  rclcpp::Service<path_manager_interfaces::srv::GetFixFramePath>::SharedPtr getFixFramePath;
  rclcpp::Service<path_manager_interfaces::srv::PlanPath>::SharedPtr plannerPathServ;
  rclcpp::Service<path_manager_interfaces::srv::RobotPath>::SharedPtr receivePathserv;
    rclcpp::Service<path_manager_interfaces::srv::AssistEmergency>::SharedPtr assistEmersrv;

  // ---------------------------------------------------------------------------
  // Pub/Sub ROS
  // ---------------------------------------------------------------------------
  rclcpp_lifecycle::LifecyclePublisher<nav_msgs::msg::Path>::SharedPtr path_publisher_;
  std::shared_ptr<rclcpp::Subscription<nav_msgs::msg::Path>> sub_ll_path;

  /// Ruta actual en memoria (la que se pretende seguir en autónomo).
  nav_msgs::msg::Path robot_path;
  nav_msgs::msg::Path ll_path;

  /// Frame fijo y método de localización (típicamente "Fixposition").
  std::string fixed_frame, localization_method, global_param;
  // WGS-84 Earth model constants
  static constexpr double a = 6378137.0;  // Semi-major axis
  static constexpr double e_squared = 0.00669437999014;  // Eccentricity squared
};

#endif  // PATH_MANAGER_HPP
