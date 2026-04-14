#include <csignal>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/utilities.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "argj801_sensors/argj801_sensors_node.hpp"
#include "argj801_sensors/SensorBuilder.hpp"
#include "argj801_sensors/argj801_camera_builder.hpp"
#include "argj801_sensors/argj801_lcm_sensors_builder.hpp"

std::shared_ptr<Argj801SensorsNode> node;

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

  // ROS 2 launch injects its own arguments (e.g. --ros-args, remaps, params).
  // Strip those so we can reliably parse our own positional args.
  const auto non_ros_args = rclcpp::remove_ros_arguments(argc, argv);

  std::signal(SIGINT, sigintHandler);

  // rclcpp::executors::SingleThreadedExecutor exe;
  rclcpp::executors::MultiThreadedExecutor exe;

  std::shared_ptr<Builder::SensorBuilder> builder;

  std::string sensor_source = "lcm_sensors";
  if (non_ros_args.size() >= 2) {
    sensor_source = non_ros_args[1];
  }

  if (sensor_source == "lcm_sensors") {
    builder = std::make_shared<Builder::Argj801LCMSensorBuilder>();
    node = std::make_shared<Argj801SensorsNode>(builder);
  } else if (sensor_source == "camera") {
    builder = std::make_shared<Builder::Argj801CameraBuilder>();
    node = std::make_shared<Argj801SensorsNode>(builder);
  } else {
    fprintf(stderr,
            "Invalid arguments. Usage: argj801_sensors [sensor_source]\n"
            " sensor_source:\n"
            " - lcm_sensors\n"
            " - camera\n");
    return 1;
  }

  exe.add_node(node->get_node_base_interface());

  exe.spin();

  rclcpp::shutdown();

  return 0;
}
