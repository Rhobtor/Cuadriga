#ifndef CTL_MISSION_NODE_HPP
#define CTL_MISSION_NODE_HPP

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_lifecycle/lifecycle_node.hpp>
#include "lifecycle_msgs/srv/change_state.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/int32_multi_array.hpp"
#include "ctl_mission/J8_FSM.h"
#include "ctl_mission_interfaces/srv/change_mode.hpp"
#include "ctl_mission_interfaces/srv/get_mode.hpp"
#include "ctl_mission_interfaces/srv/get_possible_transitions.hpp"
#include <map>
#include <functional>
#include <chrono>

using namespace std::chrono_literals;

class CtlMissionNode : public rclcpp_lifecycle::LifecycleNode {
private:
    J8_FSM j8_fsm;
    std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::String>> pub_;
    std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::Int32>> fsm_mode_pub_;
    std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::Int32MultiArray>> possible_transitions_pub_;
    std::shared_ptr<rclcpp::TimerBase> timer_;
    std::shared_ptr<rclcpp::Service<ctl_mission_interfaces::srv::ChangeMode>> changeModeServ;
    std::shared_ptr<rclcpp::Service<ctl_mission_interfaces::srv::GetMode>> getModeServ;
    rclcpp::Service<ctl_mission_interfaces::srv::GetPossibleTransitions>::SharedPtr get_transitions_service_;
    rclcpp::Client<lifecycle_msgs::srv::ChangeState>::SharedPtr path_follow_client, stanley_ctrl_client, teleoperation_client, 
    path_record_client,follow_zed_client, mppi_sac_relay_client, ready_client, estop_client, back_home_client, mpc_planner_client;
    using TransitionHandler = std::function<void()>;
    std::map<Transition, TransitionHandler> transitionHandlers;
    std::shared_future<lifecycle_msgs::srv::ChangeState::Response::SharedPtr> service_future_;
    rclcpp::TimerBase::SharedPtr future_check_timer_;
    std::string change_fsm_mode_srv_name, get_fsm_srv_name, get_fsm_topic_name, get_possible_transition_srv_name, 
                get_possible_transition_topic_name;
    rclcpp::CallbackGroup::SharedPtr client_callback_group_;



public:
    explicit CtlMissionNode(const std::string & node_name, bool intra_process_comms = false);

    // Lifecycle transition callbacks
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_configure(const rclcpp_lifecycle::State &);
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_activate(const rclcpp_lifecycle::State &);
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State &);
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_cleanup(const rclcpp_lifecycle::State &);
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_shutdown(const rclcpp_lifecycle::State &);

    // Service callbacks and methods
    void changeMode(const std::shared_ptr<ctl_mission_interfaces::srv::ChangeMode::Request>,
                    const std::shared_ptr<ctl_mission_interfaces::srv::ChangeMode::Response>);
    void getMode(const std::shared_ptr<ctl_mission_interfaces::srv::GetMode::Request>,
                 const std::shared_ptr<ctl_mission_interfaces::srv::GetMode::Response>);
    void sendLifecycleStateRequest(const rclcpp::Client<lifecycle_msgs::srv::ChangeState>::SharedPtr&, uint8_t transition_id);
    bool sendLifecycleStateRequestSync(const rclcpp::Client<lifecycle_msgs::srv::ChangeState>::SharedPtr& client, uint8_t transition_id); 
    void checkFuture();
    void handle_get_possible_transitions(
        const std::shared_ptr<ctl_mission_interfaces::srv::GetPossibleTransitions::Request> request,
        std::shared_ptr<ctl_mission_interfaces::srv::GetPossibleTransitions::Response> response);
    
    // Transition handler functions
    void onReadyToPathTransition();
    void onPathToReadyTransition();
    void onReadyToTeleTransition();
    void onTeleToReadyTransition();
    void onHomeToReadyTransition();
    void onReadyToHomeTransition();
    void onEstopToReadyTransition();
    void onReadyToFollowZEDTransition();
    void onFollowZEDToReadyTransition();
    void onReadyToRecordPathTransition();
    void onRecordPathToReadyTransition();
    void onReadyToMppiSacTransition();
    void onMppiSacToReadyTransition();
    void onAllToEstopTransition(Mode old_mode);

    // Publish methods
    void publishFsmStateAndTransitions();
};

#endif // CTL_MISSION_NODE_HPP
