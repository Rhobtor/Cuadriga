#ifndef LCM_INTERFACE_HPP
#define LCM_INTERFACE_HPP

#include <lcm/lcm-cpp.hpp>

#include "lcmmessages/cmd_discrete_device_msg.hpp"
#include "lcmmessages/header2.hpp"
#include "lcmmessages/DiscreteDevice.hpp"
#include "lcmmessages/cmd_throttle_msg.hpp"
#include "lcmmessages/request_connection_msg.hpp"
#include "lcmmessages/cmd_velocity_msg.hpp"
#include "lcmmessages/dat_station_heartbeat_msg.hpp"
#include "lcmmessages/dat_driveline_msg.hpp"
#include "lcmmessages/dat_lidar_scan_line_msg.hpp"
#include "lcmmessages/dat_vehicle_heartbeat_msg.hpp"
#include "lcmmessages/dat_vehicle_telemetry_msg.hpp"
#include "lcmmessages/request_connection_response_msg.hpp"
#include "lcmmessages/dat_cpu_msg.hpp"
#include "lcmmessages/dat_hardware_status_msg.hpp"
#include "lcmmessages/dat_temperature_msg.hpp"
#include "lcmmessages/dat_battery_msg.hpp"
#include "lcmmessages/dat_component_state_msg.hpp"

#include <yaml-cpp/yaml.h>
#include <iostream>
#include <memory>
#include <semaphore.h>
#include <sys/time.h>

#define LCM_TIMEOUT 100  //ms

namespace argj801_lcm {
    
    enum class RequestConnectionType{Control,DisconectControl,Monitor,DisconectMonitor};
    enum class ControlMode{USBGamepad=0,LCM=1,RadioController=5,FollowMe=7};
    enum class DiscreteDeviceType{Contractors =1, SoftEStop =51};
    enum class PlatformState{EmergencyStop=-1,Stopped=2,LinearMoving=3};

    class LCMInterface {
    private:
        std::shared_ptr<lcm::LCM> lcmHandleExternal;
        std::shared_ptr<lcm::LCM> lcmHandleExternalDriveLine;
        std::shared_ptr<lcm::LCM> lcmHandleExternalLidar;
        std::shared_ptr<lcm::LCM> lcmHandleBroadcast;
        std::shared_ptr<lcm::LCM> lcmHandleBroadcastHeartbeat;
        std::shared_ptr<lcm::LCM> lcmHandleBroadcastTelemetry;
        std::shared_ptr<lcm::LCM> lcmHandleExternalRequestConnectionResponse;
        std::shared_ptr<lcm::LCM> lcmHandleExternalBattery;
        std::shared_ptr<lcm::LCM> lcmHandleExternalComponentState;
        std::shared_ptr<lcm::LCM> lcmHandleExternalCpu;
        std::shared_ptr<lcm::LCM> lcmHandleExternalHardwareStatus;
        std::shared_ptr<lcm::LCM> lcmHandleExternalTemperature;

        std::string configPath;

        std::string external_url;
        std::string broadcast_url;

        lcmmessages::cmd_discrete_device_msg discreteDeviceMsg;
        lcmmessages::header2 header_from_station;
        lcmmessages::DiscreteDevice discreteDevice;
        lcmmessages::request_connection_msg requestConnectionMsg;
        lcmmessages::cmd_throttle_msg throttleMsg;
        lcmmessages::cmd_velocity_msg velocityMsg;
        lcmmessages::dat_station_heartbeat_msg stationHeartbeatMsg;

        lcmmessages::dat_driveline_msg datDrivelineMsg;
        lcmmessages::dat_lidar_scan_line_msg datLidarScanLineMsg;
        lcmmessages::dat_vehicle_heartbeat_msg datPlatformHeartbeatMsg;
        lcmmessages::dat_vehicle_telemetry_msg datPlatformTelemetryMsg;
        lcmmessages::request_connection_response_msg requestConnectionResponseMsg;
        lcmmessages::dat_cpu_msg datCpuMsg;
        lcmmessages::dat_hardware_status_msg datHardwareStatusMsg;
        lcmmessages::dat_temperature_msg datTemperatureMsg;
        lcmmessages::dat_battery_msg datBatteryMsg;
        lcmmessages::dat_component_state_msg datComponentStateMsg;

        sem_t semDatDrivelineMsg;
        sem_t semDatLidarScanLineMsg;
        sem_t semDatPlatformHeartbeatMsg;
        sem_t semDatPlatformTelemetryMsg;
        sem_t semRequestConnectionResponseMsg;
        sem_t semDatCpuMsg;
        sem_t semDatHardwareStatusMsg;
        sem_t semDatTemperatureMsg;
        sem_t semDataBatteryMsg;
        sem_t semDataCommponetStateMsg;


        bool data_drive_line_msg;
        bool data_lidar_scan_line_msg;
        bool data_platform_heartbeat_msg;
        bool data_platform_telemetry_msg;
        bool request_connection_response_msg;
        bool data_battery_msg;
        bool data_component_state_msg;
        bool data_cpu_msg;
        bool data_hardware_status_msg;
        bool data_temperature_msg;

        void loadConfig();
        void datDrivelineMsgCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name, const lcmmessages::dat_driveline_msg* msg);
        void datLidarScanLineMsgCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name, const lcmmessages::dat_lidar_scan_line_msg* msg);
        void datPlatformHeartbeatMsgCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name, const lcmmessages::dat_vehicle_heartbeat_msg* msg);
        void datPlatformTelemetryMsgCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name, const lcmmessages::dat_vehicle_telemetry_msg* msg);
        void requestConnectionResponseMsgCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name, const lcmmessages::request_connection_response_msg* msg);
        void datCpuMsgCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name, const lcmmessages::dat_cpu_msg* msg);
        void datHardwareStatusMsgCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name, const lcmmessages::dat_hardware_status_msg* msg);
        void datTemperatureMsgCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name, const lcmmessages::dat_temperature_msg* msg);
        void datBatteryMsgCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name, const lcmmessages::dat_battery_msg* msg);
        void datComponentStateMsgCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name, const lcmmessages::dat_component_state_msg* msg);
        void genericCallback(const lcm::ReceiveBuffer* rbuf, const std::string& channel_name);

        double time_helper();

        double start_time;
    public:
        // Constructor
        LCMInterface(std::string configPath,bool data_drive_line_msg,bool data_lidar_scan_line_msg,bool data_platform_heartbeat_msg,bool data_platform_telemetry_msg,bool request_connection_response_msg,bool data_battery_msg, bool data_commponet_state_msg, bool data_cpu_msg,bool data_hardware_status_msg,bool data_temperature_msg);

        // Destructor
        ~LCMInterface();

        void sendDiscreteDeviceMsg(DiscreteDeviceType type, bool state);
        void sendThrottleMsg(float throttle,float steering);
        void sendVelocityMsg(float forward_velocity, float angular_velocity);
        void sendStationHeartbeatMsg();
        void sendRequestConnectionMsg(RequestConnectionType connection_type);
        lcmmessages::dat_driveline_msg receiveDatDrivelineMsg(int timeout_ms=100);
        lcmmessages::dat_lidar_scan_line_msg receiveDatLidarScanLineMsg(int timeout_ms=100);
        lcmmessages::dat_vehicle_heartbeat_msg receiveDatPlatformHeartbeatMsg(int timeout_ms=100);
        lcmmessages::dat_vehicle_telemetry_msg receiveDatPlatformTelemetryMsg(int timeout_ms=100);
        lcmmessages::request_connection_response_msg receiveRequestConnectionResponseMsg(int timeout_ms=100);
        lcmmessages::dat_cpu_msg receiveDatCpuMsg(int timeout_ms=100);
        lcmmessages::dat_hardware_status_msg receiveDatHardwareStatusMsg(int timeout_ms=100);
        lcmmessages::dat_temperature_msg receiveDatTemperatureMsg(int timeout_ms=100);
        lcmmessages::dat_battery_msg receiveDataBatteryMsg(int timeout_ms=100);
        lcmmessages::dat_component_state_msg receiveDataCommponetStateMsg(int timeout_ms=100);

    };

    class LCMException  : public std::exception {
        private:
        std::string error;
        public:
        LCMException(const std::string msg) : error(msg) {}

        const char* what() const noexcept override {
            return error.c_str();
        }
    };
}
#endif // LCM_INTERFACE_HPP
