// mppi_sac_relay_node_main.cpp
//
// Entry point for the MPPI/SAC relay lifecycle node.
// This node is activated/deactivated by the mission orchestrator.

#include <rclcpp/rclcpp.hpp>

#include "ctl_mission/MppiSacRelayNode.hpp"

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<MppiSacRelayNode>("mppi_sac_relay_node");
  rclcpp::spin(node->get_node_base_interface());

  rclcpp::shutdown();
  return 0;
}
