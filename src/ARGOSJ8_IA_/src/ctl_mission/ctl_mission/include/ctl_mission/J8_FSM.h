// J8_FSM.h
//
// High-level overview
// -------------------
// This file defines the Finite State Machine (FSM) used by the J8 mission layer.
//
// The idea of this project is:
//   - A single "mission" lifecycle node (CtlMissionNode) keeps track of the current
//     Mode (Ready, PathFollowing, Teleoperation, ...).
//   - External clients (GUI, scripts, safety nodes) request mode changes by calling
//     a service with a Transition id.
//   - The FSM decides whether the requested transition is valid from the current mode.
//   - If valid, CtlMissionNode triggers lifecycle transitions (activate/deactivate)
//     on the nodes that implement each capability.
//
// Why this is robust:
//   - Each capability runs in its own lifecycle node (activate/deactivate) so we can
//     reliably start/stop controllers and publishers.
//   - The FSM is the single source of truth for "what the robot is doing now".
//
// Notes for new developers
// ------------------------
// - The numeric values of Mode/Transition are part of the ROS contract:
//   topics and services use integer ids. Changing the order breaks compatibility.
// - See `ctl_mission/src/argj08_ctl_node.cpp` for the node that exposes the ROS API.
//
// Topics/services that depend on these enums (default names, without namespace):
//   - topic: fsm_mode (std_msgs/Int32): publishes Mode id
//   - topic: possible_transitions (std_msgs/Int32MultiArray): vector indexed by Mode
//            where each entry is the Transition id to reach that Mode, or -1 if not
//            reachable from current mode.
//   - service: change_fsm_mode (ctl_mission_interfaces/srv/ChangeMode): request by Transition id
//
// Names can be remapped via parameters in CtlMissionNode.
#ifndef J8_FSM_H
#define J8_FSM_H

#include <map>
#include <tuple>
#include <vector>
// Define modes and transitions
// Modes represent "what the mission layer is currently doing".
//
// IMPORTANT: do not reorder existing modes without updating all dependent tooling.
enum class Mode {
    Ready = 0,
    PathFollowing,
    Teleoperation,
    GoingHome,
    EmergencyStop,
    RecordPath,
    FollowZED,

    // MPPI/SAC bridge mode:
    // This mode doesn't compute control here; it acts as a relay from an external
    // AI/MPPI module into the rover's secured cmd_vel channel.
    MppiSac,
    NumberOfModes,
    
    // ... other modes
};

// Transitions represent "requested change".
// A transition is only valid for some current modes; see the transition table.
//
// IMPORTANT: do not reorder existing transitions without updating all dependent tooling.
enum class Transition {
    ReadytoPath = 0,
    PathtoReady,
    ReadytoTele,
    TeletoReady,
    ReadytoHome,
    HometoReady,
    ReadytoRecordPath,
    RecordPathtoReady,
    EstoptoReady,
    AlltoEstop,
    ReadytoFollowZED,
    FollowZEDtoReady,

    // Ready <-> MPPI/SAC relay mode.
    ReadytoMppiSac,
    MppiSactoReady,
    // ... other transitions
};

// Define a structure for state transitions
// Key type for the transition table.
// It combines "current mode" + "requested transition".
struct StateTransition {
    Mode currentMode;
    Transition transition;

    bool operator<(const StateTransition& other) const {
        return std::tie(currentMode, transition) < std::tie(other.currentMode, other.transition);
    }
};

class J8_FSM {
    Mode current_mode;
    std::map<StateTransition, Mode> transitionTable;

public:
    J8_FSM();

    // Reset FSM to initial mode.
    void init_FSM();

    // Get current mode.
    Mode get_FSM_mode() const;

    // Apply `transition` to the current mode.
    // If the transition is valid, `current_mode` changes.
    // If invalid, `current_mode` is kept.
    Mode Finite_Machine_State(Transition transition);

    // Returns a vector sized `Mode::NumberOfModes`.
    // Each position i corresponds to "transition to reach Mode(i)".
    // If unreachable, entry is -1.
    std::vector<int> get_possible_transitions() const;
};

#endif // J8_FSM_H

