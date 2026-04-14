#include <csignal>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "argj801_ctl_platform/argj801_ctl_platform_node.hpp"

// Entry-point del paquete `argj801_ctl_platform`.
//
// Aquí se construye el `CtlPlatformNode` (LifecycleNode) y se ejecuta dentro de
// un `MultiThreadedExecutor`.
//
// El handler de SIGINT fuerza una transición ordenada del lifecycle:
// deactivate -> cleanup -> shutdown, para minimizar efectos indeseados (por
// ejemplo, dejar actuadores con el último comando válido).

std::shared_ptr<CtlPlatformNode> node;

void sigintHandler(int signal)
{
  node->deactivate();
  node->cleanup();
  node->shutdown();
  exit(0);
}

/**
 * A lifecycle node has the same node API
 * as a regular node. This means we can spawn a
 * node, give it a name and add it to the executor.
 */
int main(int argc, char *argv[])
{
  // force flush of the stdout buffer.
  // this ensures a correct sync of all prints
  // even when executed simultaneously within the launch file.
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);

  rclcpp::init(argc, argv);

  std::signal(SIGINT, sigintHandler);

  rclcpp::executors::MultiThreadedExecutor exe;
  // rclcpp::executors::SingleThreadedExecutor exe;

  node =
      std::make_shared<CtlPlatformNode>("argj08_ctl_platform_node");

  // Install the SIGINT signal handler
  std::signal(SIGINT, sigintHandler);

  exe.add_node(node->get_node_base_interface());

  exe.spin();

  rclcpp::shutdown();

  return 0;
}
