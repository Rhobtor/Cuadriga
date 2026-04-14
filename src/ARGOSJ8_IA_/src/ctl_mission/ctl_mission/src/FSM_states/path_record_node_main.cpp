#include "rclcpp/rclcpp.hpp"
#include "ctl_mission/PathRecordNode.hpp"

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);

    // Create a node instance
    auto node = std::make_shared<PathRecordNode>("path_record_node");

    // Spin the node
    rclcpp::spin(node->get_node_base_interface());

    // Shutdown ROS 2
    rclcpp::shutdown();
    return 0;
}
