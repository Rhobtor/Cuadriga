// #include "rclcpp/rclcpp.hpp"
// #include "ctl_mission/FollowZED.hpp"

// int main(int argc, char *argv[]) {
//     rclcpp::init(argc, argv);

//     // Create a node instance
//     auto node = std::make_shared<FollowZEDNode>("follow_zed_node");

//     // Spin the node
//     rclcpp::spin(node->get_node_base_interface());

//     // Shutdown ROS 2
//     rclcpp::shutdown();
//     return 0;
// }

#include <rclcpp/rclcpp.hpp>
#include "ctl_mission/FollowZED.hpp"

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);

  auto node = std::make_shared<FollowZEDNode>("follow_zed_node");

  rclcpp::executors::SingleThreadedExecutor exec;
  exec.add_node(node->get_node_base_interface());
  exec.spin();

  rclcpp::shutdown();
  return 0;
}
