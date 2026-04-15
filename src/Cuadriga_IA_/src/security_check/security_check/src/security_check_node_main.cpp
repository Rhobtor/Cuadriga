#include "security_check/SecurityCheckNode.hpp"

int main(int argc, char * argv[]) {
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);
    rclcpp::init(argc, argv);

    auto executor = std::make_shared<rclcpp::executors::SingleThreadedExecutor>();
    auto node = std::make_shared<SecurityCheckNode>("security_check_node");

    executor->add_node(node->get_node_base_interface());
    executor->spin();

    rclcpp::shutdown();
    return 0;
}

