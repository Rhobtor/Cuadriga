# Copyright 2020 Open Source Robotics Foundation, Inc.
# All rights reserved.
# 
# Software License Agreement (BSD License 2.0)
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above
#   copyright notice, this list of conditions and the following
#   disclaimer in the documentation and/or other materials provided
#   with the distribution.
# * Neither the name of {copyright_holder} nor the names of its
#   contributors may be used to endorse or promote products derived
#   from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

"""Launch the ublox gps driver for base, rover and ntrip client."""

import os

import ament_index_python.packages
import launch
import launch_ros.actions
from launch.actions import DeclareLaunchArgument
from launch.actions import SetEnvironmentVariable
from launch.substitutions import LaunchConfiguration

def generate_launch_description():

    ld = launch.LaunchDescription()
    config_directory = os.path.join(
        ament_index_python.packages.get_package_share_directory('ublox_gps'),
        'config')
    params_base = os.path.join(config_directory, 'zed_base.yaml')
    ublox_gps_node_base= launch_ros.actions.Node(package='ublox_gps',
                                             executable='ublox_gps_node',
                                             #namespace='my_ns',
                                             name='ublox_gps_node_base',
                                             output='both',
                                             remappings=[('/ublox_gps_node/fix','/base/fix'),
                                             ('/ublox_gps_node/fix_velocity','/ARGJ801/base/gps/fix_velocity'),
                                             ('/ublox_gps_node/navpvt','/ARGJ801/base/gps/navpvt'),
                                             ('/ublox_gps_node_base/fix','/ARGJ801/base/gps/fix'),
                                             ('/ublox_gps_node/navpvt','/ARGJ801/base/gps/navpvt'),
                                             ('/navrelposned','/navrelposned_base')],
                                             parameters=[params_base])
    
    gps_base_exit_action = launch.actions.RegisterEventHandler(
                                         event_handler=launch.event_handlers.OnProcessExit(
                                             target_action=ublox_gps_node_base,
                                             on_exit=[launch.actions.EmitEvent(
                                                 event=launch.events.Shutdown())],
                                         ))
    
    params_rover = os.path.join(config_directory, 'zed_rover.yaml')

    ublox_gps_node_rover = launch_ros.actions.Node(package='ublox_gps',
                                             executable='ublox_gps_node',
                                             #namespace='my_ns',
                                             name='ublox_gps_node_rover',
                                             output='both',
                                             remappings=[('/ublox_gps_node/fix','/rover/fix'),
                                             ('/ublox_gps_node/fix_velocity','/ARGJ801/rover/gps/fix_velocity'),
                                             ('/ublox_gps_node_rover/fix','/ARGJ801/rover/gps/fix'),
                                             ('/ublox_gps_node/navpvt','/ARGJ801/rover/gps/navpvt'),
                                             #('/rtcm','/rtcm_noo_sub'),
                                             ('/ublox_gps_node','/ARGJ801_gps_rover_node')],
                                             parameters=[params_rover])
    gps_rover_exit_action = launch.actions.RegisterEventHandler(
                                         event_handler=launch.event_handlers.OnProcessExit(
                                             target_action=ublox_gps_node_rover,
                                             on_exit=[launch.actions.EmitEvent(
                                                 event=launch.events.Shutdown())],
                                         ))
    
    ntrip_node = launch_ros.actions.Node(
                name='ntrip_client_node',
                namespace='ntrip_client',
                package='ntrip_client',
                executable='ntrip_ros.py',
                parameters=[
                  {
                    # Required parameters used to connect to the NTRIP server
                    'host': LaunchConfiguration('host'),
                    'port': LaunchConfiguration('port'),
                    'mountpoint': LaunchConfiguration('mountpoint'),
                    # Optional parameter that will set the NTRIP version in the initial HTTP request to the NTRIP caster.
                    'ntrip_version': LaunchConfiguration('ntrip_version'),
                    # If this is set to true, we will read the username and password and attempt to authenticate. If not, we will attempt to connect unauthenticated
                    'authenticate': LaunchConfiguration('authenticate'),
                    # If authenticate is set the true, we will use these to authenticate with the server
                    'username': LaunchConfiguration('username'),
                    'password': LaunchConfiguration('password'),
                    # Whether to connect with SSL. cert, key, and ca_cert options will only take effect if this is true
                    'ssl': LaunchConfiguration('ssl'),
                    # If the NTRIP caster uses cert based authentication, you can specify the cert and keys to use with these options
                    'cert': LaunchConfiguration('cert'),
                    'key':  LaunchConfiguration('key'),
                    # If the NTRIP caster uses self signed certs, or you need to use a different CA chain, specify the path to the file here
                    'ca_cert': LaunchConfiguration('ca_cert'),
                    # Not sure if this will be looked at by other ndoes, but this frame ID will be added to the RTCM messages published by this node
                    'rtcm_frame_id': 'odom',
                    # Optional parameters that will allow for longer or shorter NMEA messages. Standard max length for NMEA is 82
                    'nmea_max_length': 82,
                    'nmea_min_length': 3,
                    # Use this parameter to change the type of RTCM message published by the node. Defaults to "mavros_msgs", but we also support "rtcm_msgs"
                    'rtcm_message_package': LaunchConfiguration('rtcm_message_package'),
                    # Will affect how many times the node will attempt to reconnect before exiting, and how long it will wait in between attempts when a reconnect occurs
                    'reconnect_attempt_max': 10,
                    'reconnect_attempt_wait_seconds': 5,
                    # How many seconds is acceptable in between receiving RTCM. If RTCM is not received for this duration, the node will attempt to reconnect
                    'rtcm_timeout_seconds': 4
                  }
                ],
                # Uncomment the following section and replace "/gq7/nmea/sentence" with the topic you are sending NMEA on if it is not the one we requested
                #remappings=[
                #  ("/ntrip_client/nmea", "/gx5/nmea/sentence")
                #],
                remappings=[
                ("/ntrip_client/rtcm", "/rtcm")
                ],
          )
    
    ld.add_action(ublox_gps_node_base)
    ld.add_action(gps_base_exit_action)
    ld.add_action(ublox_gps_node_rover)
    ld.add_action(gps_rover_exit_action)
    ld.add_action(DeclareLaunchArgument('host',default_value='217.12.26.252'))
    ld.add_action(DeclareLaunchArgument('port',default_value='2102'))
    ld.add_action(DeclareLaunchArgument('mountpoint',default_value='MLGA3'))
    ld.add_action(DeclareLaunchArgument('ntrip_version',default_value='None'))
    ld.add_action(DeclareLaunchArgument('authenticate',default_value='True'))
    ld.add_action(DeclareLaunchArgument('username', default_value='dahui'))
    ld.add_action(DeclareLaunchArgument('password', default_value='rap01318'))
    ld.add_action(DeclareLaunchArgument('ssl', default_value='False'))
    ld.add_action(DeclareLaunchArgument('cert', default_value='None'))
    ld.add_action(DeclareLaunchArgument('key', default_value='None'))
    ld.add_action(DeclareLaunchArgument('ca_cert', default_value='None'))
    ld.add_action(DeclareLaunchArgument('debug', default_value='false'))
    ld.add_action(DeclareLaunchArgument('rtcm_message_package',  default_value='rtcm_msgs'))
    ld.add_action(ntrip_node)
    return ld
                                     
