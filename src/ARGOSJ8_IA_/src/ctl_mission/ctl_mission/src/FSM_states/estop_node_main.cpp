#include  "ctl_mission/EstopNode.hpp"

int main(int argc, char *argv[]) {
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);
    rclcpp::init(argc, argv);
    auto node = std::make_shared<EstopNode>("lc_estop_node");

    // Get the NodeBaseInterface and pass it to spin
    auto node_base_interface = node->get_node_base_interface();
    rclcpp::spin(node_base_interface);

    rclcpp::shutdown();
    return 0;
}