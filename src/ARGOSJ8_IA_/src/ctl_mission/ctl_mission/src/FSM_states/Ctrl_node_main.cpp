#include "rclcpp/rclcpp.hpp"
#include "ctl_mission/CtrlNode.hpp"

int main(int argc, char * argv[]) {

    setvbuf(stdout, NULL, _IONBF, BUFSIZ);

    rclcpp::init(argc, argv);

    rclcpp::executors::SingleThreadedExecutor exe;

    std::shared_ptr<CtrlNode> lc_node =
        std::make_shared<CtrlNode>("lc_stanley_ctrl_node");

    exe.add_node(lc_node->get_node_base_interface());

    exe.spin();

    rclcpp::shutdown();

    return 0;
}

