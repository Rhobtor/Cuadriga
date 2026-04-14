#include "cuadriga_actuator/cuadriga_actuator_node.hpp"

#include <rclcpp/rclcpp.hpp>

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<cuadriga_actuator::CuadrigaActuatorNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}