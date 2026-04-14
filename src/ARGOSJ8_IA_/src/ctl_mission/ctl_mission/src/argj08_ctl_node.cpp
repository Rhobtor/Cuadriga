// argj08_ctl_node.cpp
//
// What this node is
// -----------------
// This is the *mission orchestrator* node for the J8.
//
// Responsibilities:
//   1) Own the FSM state (`J8_FSM`) and apply requested transitions.
//   2) Expose a small ROS API so external tools can:
//      - request a transition (service: ChangeMode)
//      - read current mode (service: GetMode, topic: fsm_mode)
//      - read possible transitions (service: GetPossibleTransitions, topic: possible_transitions)
//   3) When a transition is accepted, trigger lifecycle transitions on the
//      nodes that implement each capability (path following, teleop, etc).
//
// Why lifecycle nodes:
//   - Each capability is an independent LifecycleNode.
//   - The mission orchestrator can activate/deactivate them deterministically,
//     ensuring publishers and timers are only running in the right mode.
//
// Parameters (relative names; commonly namespaced under /ARGJ801):
//   - fsm.change_fsm_mode_srv_name (default: change_fsm_mode)
//   - fsm.get_fsm_srv_name         (default: get_fsm_mode)
//   - fsm.get_fsm_topic_name       (default: fsm_mode)
//   - fsm.get_possible_transition_srv_name   (default: get_possible_transitions)
//   - fsm.get_possible_transition_topic_name (default: possible_transitions)
//
// ROS API published by this node (names can be remapped via the params above):
//   Topics:
//     - <get_fsm_topic_name>                std_msgs/Int32
//     - <get_possible_transition_topic_name> std_msgs/Int32MultiArray
//   Services:
//     - <change_fsm_mode_srv_name>          ctl_mission_interfaces/srv/ChangeMode
//     - <get_fsm_srv_name>                  ctl_mission_interfaces/srv/GetMode
//     - <get_possible_transition_srv_name>  ctl_mission_interfaces/srv/GetPossibleTransitions
//
// Lifecycle clients used (hard-coded endpoints):
//   - path_following_node/change_state
//   - mpc_node/change_state
//   - controller_node/change_state
//   - teleoperation_node/change_state
//   - path_record_node/change_state
//   - ready_node/change_state
//   - estop_node/change_state
//   - back_home_node/change_state
//   - follow_zed_node/change_state
//
// NOTE: The mission node itself is also a LifecycleNode.

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

#include "ctl_mission/CtlMissionNode.hpp"
#include <map>
#include <functional>
#include <chrono>

using namespace std::chrono_literals;

CtlMissionNode::CtlMissionNode(const std::string & node_name, bool intra_process_comms)
: rclcpp_lifecycle::LifecycleNode(node_name, 
    rclcpp::NodeOptions().use_intra_process_comms(intra_process_comms)) {

    // Transition handlers map a Transition id -> "what lifecycle changes to perform".
    // The FSM only decides if a transition is valid. These handlers perform the actual work.
    transitionHandlers[Transition::ReadytoPath] = [this]() { this->onReadyToPathTransition(); };
    transitionHandlers[Transition::PathtoReady] = [this]() { this->onPathToReadyTransition(); };
    transitionHandlers[Transition::ReadytoTele] = [this]() { this->onReadyToTeleTransition(); };
    transitionHandlers[Transition::TeletoReady] = [this]() { this->onTeleToReadyTransition(); };
    transitionHandlers[Transition::HometoReady] = [this]() { this->onHomeToReadyTransition(); };
    transitionHandlers[Transition::ReadytoHome] = [this]() { this->onReadyToHomeTransition(); };
    transitionHandlers[Transition::EstoptoReady] = [this]() { this->onEstopToReadyTransition(); };
    transitionHandlers[Transition::ReadytoRecordPath] = [this]() { this->onReadyToRecordPathTransition(); };
    transitionHandlers[Transition::RecordPathtoReady] = [this]() { this->onRecordPathToReadyTransition(); };
    transitionHandlers[Transition::ReadytoFollowZED] = [this]() { this->onReadyToFollowZEDTransition(); };
    transitionHandlers[Transition::FollowZEDtoReady] = [this]() { this->onFollowZEDToReadyTransition(); };

    // MPPI/SAC relay mode
    transitionHandlers[Transition::ReadytoMppiSac] = [this]() { this->onReadyToMppiSacTransition(); };
    transitionHandlers[Transition::MppiSactoReady] = [this]() { this->onMppiSacToReadyTransition(); };

    
    // Emergency stop is handled separately because it needs to deactivate whichever
    // mode is active and then activate the estop node.
    // transitionHandlers[Transition::AlltoEstop] = [this]() { this->onAllToEstopTransition(/* ??? */); };
    this->declare_parameter("fsm.change_fsm_mode_srv_name", "change_fsm_mode");
    this->declare_parameter("fsm.get_fsm_srv_name", "get_fsm_mode");
    this->declare_parameter("fsm.get_fsm_topic_name", "fsm_mode");
    this->declare_parameter("fsm.get_possible_transition_srv_name", "get_possible_transitions");
    this->declare_parameter("fsm.get_possible_transition_topic_name", "possible_transitions");

    j8_fsm.init_FSM();
}

void CtlMissionNode::sendLifecycleStateRequest(
    const rclcpp::Client<lifecycle_msgs::srv::ChangeState>::SharedPtr& client,
    uint8_t transition_id) 
{
    // Async request: handler functions call this multiple times in sequence.
    // A timer polls the future; we currently don't block waiting for success.
    // Ensure the client is valid
    if (!client) {
        RCLCPP_ERROR(this->get_logger(), "Lifecycle client is not initialized");
        return;
    }

    if (!client->wait_for_service(1s)) {
        RCLCPP_ERROR(
            this->get_logger(),
            "Lifecycle service not available: '%s' (transition_id=%u)",
            client->get_service_name(), static_cast<unsigned>(transition_id));
        return;
    }

    // Prepare the request
    auto request = std::make_shared<lifecycle_msgs::srv::ChangeState::Request>();
    request->transition.id = transition_id;

    // Send the request and store the future
    service_future_ = client->async_send_request(request);

    // Start a timer to periodically check the future
    future_check_timer_ = this->create_wall_timer(
        100ms, std::bind(&CtlMissionNode::checkFuture, this));
}

void CtlMissionNode::checkFuture() {
    if (service_future_.valid() && service_future_.wait_for(0s) == std::future_status::ready) {
        auto response = service_future_.get();
        // NOTE: We currently ignore the response payload.
        // If needed, this is the place to log failures.
        future_check_timer_->cancel();
    }
}

void CtlMissionNode::onReadyToPathTransition() {
    // Ready -> PathFollowing:
    // - Deactivate Ready
    // - Activate path following + planner + controller
    sendLifecycleStateRequest(ready_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
    sendLifecycleStateRequest(path_follow_client, lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
    sendLifecycleStateRequest(mpc_planner_client, lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
    sendLifecycleStateRequest(stanley_ctrl_client, lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
    RCLCPP_INFO(get_logger(), "Transitioning from Ready to Path");
}

void CtlMissionNode::onPathToReadyTransition() {
    // PathFollowing -> Ready:
    // - Deactivate path following stack
    // - Activate Ready
    sendLifecycleStateRequest(path_follow_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
    sendLifecycleStateRequest(mpc_planner_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
    sendLifecycleStateRequest(stanley_ctrl_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
    sendLifecycleStateRequest(ready_client, lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
    RCLCPP_INFO(get_logger(), "Transitioning from Path to Ready");
}

void CtlMissionNode::onReadyToTeleTransition() {
    // Ready -> Teleoperation
    sendLifecycleStateRequest(ready_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
    sendLifecycleStateRequest(teleoperation_client, lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
    RCLCPP_INFO(get_logger(), "Transitioning from Ready to Teleoperation");
}

void CtlMissionNode::onTeleToReadyTransition() {
    // Teleoperation -> Ready
    sendLifecycleStateRequest(teleoperation_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
    sendLifecycleStateRequest(ready_client, lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
    RCLCPP_INFO(get_logger(), "Transitioning from Teleoperation to Ready");
}

void CtlMissionNode::onHomeToReadyTransition() {
    // GoingHome -> Ready
    sendLifecycleStateRequest(back_home_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
    sendLifecycleStateRequest(ready_client, lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
    RCLCPP_INFO(get_logger(), "Transitioning from Home to Ready");
}

void CtlMissionNode::onReadyToHomeTransition() {
    // Ready -> GoingHome
    sendLifecycleStateRequest(ready_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
    sendLifecycleStateRequest(back_home_client, lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
    RCLCPP_INFO(get_logger(), "Transitioning from Ready to Home");
}

void CtlMissionNode::onEstopToReadyTransition() {
    // EmergencyStop -> Ready
    sendLifecycleStateRequest(estop_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
    sendLifecycleStateRequest(ready_client, lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
    RCLCPP_INFO(get_logger(), "Transitioning from Estop to Ready");
}

void CtlMissionNode::onRecordPathToReadyTransition() {
    // RecordPath -> Ready
    sendLifecycleStateRequest(path_record_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
    sendLifecycleStateRequest(ready_client, lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
    RCLCPP_INFO(get_logger(), "Transitioning from Record to Ready");
}

void CtlMissionNode::onReadyToRecordPathTransition() {
    // Ready -> RecordPath
    sendLifecycleStateRequest(ready_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
    sendLifecycleStateRequest(path_record_client, lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
    RCLCPP_INFO(get_logger(), "Transitioning from Ready to Record Path");
}

void CtlMissionNode::onFollowZEDToReadyTransition() {
    // FollowZED -> Ready
    sendLifecycleStateRequest(follow_zed_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
    sendLifecycleStateRequest(ready_client, lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
    RCLCPP_INFO(get_logger(), "Transitioning from Follow ZED to Ready");
}

void CtlMissionNode::onReadyToFollowZEDTransition() {
    // Ready -> FollowZED
    sendLifecycleStateRequest(follow_zed_client, lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
    sendLifecycleStateRequest(ready_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
    RCLCPP_INFO(get_logger(), "Transitioning from Ready to Follwow ZED");
}

void CtlMissionNode::onReadyToMppiSacTransition() {
    // Ready -> MPPI/SAC relay
    // - Deactivate Ready
    // - Activate relay node, which will forward AI Twist commands to secured cmd_vel
    sendLifecycleStateRequest(ready_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
    sendLifecycleStateRequest(mppi_sac_relay_client, lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
    RCLCPP_INFO(get_logger(), "Transitioning from Ready to MPPI/SAC relay");
}

void CtlMissionNode::onMppiSacToReadyTransition() {
    // MPPI/SAC relay -> Ready
    sendLifecycleStateRequest(mppi_sac_relay_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
    sendLifecycleStateRequest(ready_client, lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
    RCLCPP_INFO(get_logger(), "Transitioning from MPPI/SAC relay to Ready");
}

void CtlMissionNode::onAllToEstopTransition(Mode old_mode) {
    // Emergency stop is special:
    // - We deactivate whatever was active (depending on old_mode)
    // - Then activate the estop node
    RCLCPP_INFO(get_logger(), "Handling Emergency Stop from mode: %i", static_cast<int>(old_mode));
    switch (old_mode) {
        case Mode::PathFollowing:
            RCLCPP_INFO(get_logger(), "Estop from PathFollowing");
            sendLifecycleStateRequest(path_follow_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
            sendLifecycleStateRequest(mpc_planner_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
            sendLifecycleStateRequest(stanley_ctrl_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
            break;
        case Mode::Teleoperation:
            RCLCPP_INFO(get_logger(), "Estop from Teleoperation");
            sendLifecycleStateRequest(teleoperation_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
            break;
        case Mode::GoingHome:
            RCLCPP_INFO(get_logger(), "Estop from GoingHome");
            sendLifecycleStateRequest(back_home_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
            break;
        case Mode::RecordPath:
            RCLCPP_INFO(get_logger(), "Estop from RecordPath");
            sendLifecycleStateRequest(path_record_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
            break;
        case Mode::Ready:
            RCLCPP_INFO(get_logger(), "Estop from Ready");
            sendLifecycleStateRequest(ready_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
            break;
        case Mode::FollowZED:
            RCLCPP_INFO(get_logger(), "Estop from FollowZED");
            sendLifecycleStateRequest(follow_zed_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
            break;
        case Mode::MppiSac:
            RCLCPP_INFO(get_logger(), "Estop from MPPI/SAC relay");
            sendLifecycleStateRequest(mppi_sac_relay_client, lifecycle_msgs::msg::Transition::TRANSITION_DEACTIVATE);
            break;
        case Mode::EmergencyStop:
        default:
            RCLCPP_INFO(get_logger(), "No action needed for current mode or EmergencyStop");
            // No action needed for EmergencyStop or any undefined mode
            break;
    }
    // Activate the Emergency Stop state
    sendLifecycleStateRequest(estop_client, lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
    RCLCPP_INFO(get_logger(), "Emergency Stop activated");
}

void CtlMissionNode::changeMode(const std::shared_ptr<ctl_mission_interfaces::srv::ChangeMode::Request> req,
                const std::shared_ptr<ctl_mission_interfaces::srv::ChangeMode::Response> res) {
    // Main service entrypoint:
    // - Convert requested transition id
    // - Ask FSM to compute new mode
    // - If mode changed, execute lifecycle handler
    Mode old_mode = j8_fsm.get_FSM_mode();            
    Transition transition = static_cast<Transition>(req->transition);
    Mode newMode = j8_fsm.Finite_Machine_State(transition);

    // Special case for Estop transition
    if (transition == Transition::AlltoEstop) {
        RCLCPP_INFO(this->get_logger(), "Emergency Stop requested");
        onAllToEstopTransition(old_mode);
        res->success = true;
        return;
    }
    
    // RCLCPP_INFO(this->get_logger(), "current state %i", j8_fsm.get_FSM_mode() );
    // RCLCPP_INFO(this->get_logger(), "new state %i", newMode );
    RCLCPP_INFO(this->get_logger(), "current state %d", static_cast<int>(j8_fsm.get_FSM_mode()));
    RCLCPP_INFO(this->get_logger(), "new state %d", static_cast<int>(newMode));
    RCLCPP_INFO(this->get_logger(), "asked transition %i", req->transition );
    
    if (newMode != old_mode) {
        auto handlerIt = transitionHandlers.find(transition);
        if (handlerIt != transitionHandlers.end()) {
            handlerIt->second();
        }
        res->success = true;
    } else {
        res->success = false; // Invalid transition
    }
}

void CtlMissionNode::getMode(const std::shared_ptr<ctl_mission_interfaces::srv::GetMode::Request> req,
             const std::shared_ptr<ctl_mission_interfaces::srv::GetMode::Response> res) {
    res->mode = static_cast<int>(j8_fsm.get_FSM_mode());
}

void CtlMissionNode::handle_get_possible_transitions(
    const std::shared_ptr<ctl_mission_interfaces::srv::GetPossibleTransitions::Request> request,
    std::shared_ptr<ctl_mission_interfaces::srv::GetPossibleTransitions::Response> response)
{
    // Service endpoint for tools that prefer request/response.
    // This mirrors what we publish periodically in the topic.
    response->possible_transitions = j8_fsm.get_possible_transitions();
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn CtlMissionNode::on_configure(const rclcpp_lifecycle::State &) {
    // Read parameterized ROS names.
    this->get_parameter("fsm.change_fsm_mode_srv_name", change_fsm_mode_srv_name);
    this->get_parameter("fsm.get_fsm_srv_name", get_fsm_srv_name);
    this->get_parameter("fsm.get_fsm_topic_name", get_fsm_topic_name);
    this->get_parameter("fsm.get_possible_transition_srv_name", get_possible_transition_srv_name);
    this->get_parameter("fsm.get_possible_transition_topic_name", get_possible_transition_topic_name);

    // Publishers are lifecycle publishers; they only publish while node is ACTIVE.
    fsm_mode_pub_ = this->create_publisher<std_msgs::msg::Int32>(get_fsm_topic_name, 10);
    possible_transitions_pub_ = this->create_publisher<std_msgs::msg::Int32MultiArray>(get_possible_transition_topic_name, 10);
    changeModeServ = this->create_service<ctl_mission_interfaces::srv::ChangeMode>(change_fsm_mode_srv_name, 
    std::bind(&CtlMissionNode::changeMode, this, std::placeholders::_1, std::placeholders::_2));
    getModeServ = this->create_service<ctl_mission_interfaces::srv::GetMode>(get_fsm_srv_name, 
    std::bind(&CtlMissionNode::getMode, this, std::placeholders::_1, std::placeholders::_2));
    get_transitions_service_ = this->create_service<ctl_mission_interfaces::srv::GetPossibleTransitions>(
    get_possible_transition_srv_name,
    std::bind(&CtlMissionNode::handle_get_possible_transitions, this, std::placeholders::_1, std::placeholders::_2));
    // Clients for lifecycle transitions of subnodes.
    // NOTE: these endpoints are currently hard-coded (not parameterized).
    path_follow_client = this->create_client<lifecycle_msgs::srv::ChangeState>("path_following_node/change_state");
    mpc_planner_client = this->create_client<lifecycle_msgs::srv::ChangeState>("mpc_node/change_state");
    stanley_ctrl_client = this->create_client<lifecycle_msgs::srv::ChangeState>("controller_node/change_state");
    teleoperation_client = this->create_client<lifecycle_msgs::srv::ChangeState>("teleoperation_node/change_state");
    path_record_client = this->create_client<lifecycle_msgs::srv::ChangeState>("path_record_node/change_state");
    ready_client = this->create_client<lifecycle_msgs::srv::ChangeState>("ready_node/change_state");
    estop_client = this->create_client<lifecycle_msgs::srv::ChangeState>("estop_node/change_state");
    back_home_client  = this->create_client<lifecycle_msgs::srv::ChangeState>("back_home_node/change_state");
    follow_zed_client = this->create_client<lifecycle_msgs::srv::ChangeState>("follow_zed_node/change_state");
    mppi_sac_relay_client = this->create_client<lifecycle_msgs::srv::ChangeState>("mppi_sac_relay_node/change_state");
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn CtlMissionNode::on_activate(const rclcpp_lifecycle::State & state) {
    LifecycleNode::on_activate(state);
    RCLCPP_INFO(get_logger(), "Node is activated.");
    fsm_mode_pub_->on_activate();
    possible_transitions_pub_->on_activate();
    timer_ = this->create_wall_timer(
        500ms, std::bind(&CtlMissionNode::publishFsmStateAndTransitions, this));
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn CtlMissionNode::on_deactivate(const rclcpp_lifecycle::State & state) {
    LifecycleNode::on_deactivate(state);
    RCLCPP_INFO(get_logger(), "Node is deactivated.");
    timer_.reset();
    fsm_mode_pub_->on_deactivate();
    possible_transitions_pub_->on_deactivate();
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn CtlMissionNode::on_cleanup(const rclcpp_lifecycle::State &) {
    timer_.reset();
    fsm_mode_pub_.reset();
    possible_transitions_pub_.reset();
    RCLCPP_INFO(get_logger(), "Node cleanup is completed.");
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn CtlMissionNode::on_shutdown(const rclcpp_lifecycle::State & state) {
    timer_.reset();
    fsm_mode_pub_.reset();
    possible_transitions_pub_.reset();
    RCLCPP_INFO(get_logger(), "Node shutdown from state %s.", state.label().c_str());
    return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

void CtlMissionNode::publishFsmStateAndTransitions() {
    // Periodic publication used by GUIs:
    // - current mode
    // - all transitions currently valid
    // Publish FSM mode
    std_msgs::msg::Int32 mode_msg;
    mode_msg.data = static_cast<int>(j8_fsm.get_FSM_mode());
    fsm_mode_pub_->publish(mode_msg);

    // Publish possible transitions
    std_msgs::msg::Int32MultiArray transitions_msg;
    transitions_msg.data = j8_fsm.get_possible_transitions();
    possible_transitions_pub_->publish(transitions_msg);
}

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::executors::SingleThreadedExecutor exe;
    auto lc_node = std::make_shared<CtlMissionNode>("ctl_mission_node");
    exe.add_node(lc_node->get_node_base_interface());
    exe.spin();
    rclcpp::shutdown();
    return 0;
}
