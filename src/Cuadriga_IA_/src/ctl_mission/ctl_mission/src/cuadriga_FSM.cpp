#include <map>
#include <tuple>
#include "ctl_mission/cuadriga_FSM.h"


// Implementation notes
// --------------------
// This file contains the "pure" FSM logic. It has no ROS dependencies.
// The ROS node (CtlMissionNode) consumes this class to decide whether a
// requested transition is valid.
//
// The transition table is defined once in the constructor.
// New modes/transitions should be added BOTH in the enums (cuadriga_FSM.h) and here.




    cuadriga_FSM::cuadriga_FSM() {
        // Transition table: (current mode, transition) -> new mode.
        // This encodes the allowed state diagram.
        transitionTable = {
            {{Mode::Ready, Transition::ReadytoPath}, Mode::PathFollowing},
            {{Mode::PathFollowing, Transition::PathtoReady}, Mode::Ready},
            {{Mode::Ready, Transition::ReadytoTele}, Mode::Teleoperation},
            {{Mode::Teleoperation, Transition::TeletoReady}, Mode::Ready},
            {{Mode::GoingHome, Transition::HometoReady}, Mode::Ready},
            {{Mode::Ready, Transition::ReadytoHome}, Mode::GoingHome},
            {{Mode::EmergencyStop, Transition::EstoptoReady}, Mode::Ready},
            {{Mode::Ready, Transition::ReadytoRecordPath}, Mode::RecordPath},
            {{Mode::RecordPath, Transition::RecordPathtoReady}, Mode::Ready},
            {{Mode::Ready, Transition::AlltoEstop}, Mode::EmergencyStop},
            {{Mode::Ready, Transition::ReadytoFollowZED}, Mode::FollowZED},
            {{Mode::FollowZED, Transition::FollowZEDtoReady}, Mode::Ready},

            // MPPI/SAC relay mode
            {{Mode::Ready, Transition::ReadytoMppiSac}, Mode::MppiSac},
            {{Mode::MppiSac, Transition::MppiSactoReady}, Mode::Ready}
            // ... other transitions
        };
        // Start always in Ready.
        init_FSM();
    }

    void cuadriga_FSM::init_FSM() {
        current_mode = Mode::Ready;
    }

    Mode cuadriga_FSM::get_FSM_mode() const{
        return current_mode;
    }

Mode cuadriga_FSM::Finite_Machine_State(Transition transition) {
    // Emergency Stop is modelled as a global transition.
    // Note that CtlMissionNode additionally performs the actual lifecycle actions.
    if (transition == Transition::AlltoEstop) {
        current_mode = Mode::EmergencyStop;
    } else {
        // Handle other transitions using the transition table
        StateTransition key{current_mode, transition};
        auto it = transitionTable.find(key);

        if (it != transitionTable.end()) {
            // Transition found in the table, update the mode
            current_mode = it->second;
        } else {
            // Transition not found => invalid request.
            // We intentionally keep the mode unchanged.
        }
    }
    return current_mode;
}
std::vector<int> cuadriga_FSM::get_possible_transitions() const {
    // Contract used by both the GUI and scripts:
    // - Size: NumberOfModes
    // - Index: target Mode
    // - Value: Transition id to go from current_mode -> target Mode (or -1)
    std::vector<int> possible_transitions(static_cast<int>(Mode::NumberOfModes), -1);

    for (const auto& entry : transitionTable) {
        // entry.first is a StateTransition struct
        if (entry.first.currentMode == current_mode) {
            int target_state_index = static_cast<int>(entry.second); // entry.second is the target Mode
            possible_transitions[target_state_index] = static_cast<int>(entry.first.transition); // entry.first.transition is the Transition
        }
    }

    // Always add the transition to EmergencyStop.
    // This makes E-Stop always visible/actionable regardless of current mode.
    int emergency_stop_index = static_cast<int>(Mode::EmergencyStop);
    possible_transitions[emergency_stop_index] = static_cast<int>(Transition::AlltoEstop);

    return possible_transitions;
}