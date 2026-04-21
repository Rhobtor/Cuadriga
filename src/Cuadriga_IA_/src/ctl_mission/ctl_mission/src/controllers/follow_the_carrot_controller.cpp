#include "ctl_mission/Follow_The_Carrot.h"

FollowTheCarrot::FollowTheCarrot()
{
    look_ahead_distance = 1.0f;
    forward_speed = 0.35f;
    min_turning_speed = 0.1f;
    heading_gain = 0.9f;
    max_angular_speed = 1.0f;
    slow_turn_angle = 0.35f;
    turn_in_place_angle = 0.75f;
    goal_tolerance = 1.0f;
    output.linear_sp = 0.0f;
    output.angular_sp = 0.0f;
    output.goal_idx = 0;
    output.cte = 0.0f;
    output.dist_last_obj = 0.0f;
}

void FollowTheCarrot::control_init(
    float look_ahead_distance,
    float forward_speed,
    float min_turning_speed,
    float heading_gain,
    float max_angular_speed,
    float slow_turn_angle,
    float turn_in_place_angle,
    float goal_tolerance)
{
    this->look_ahead_distance = look_ahead_distance;
    this->forward_speed = forward_speed;
    this->min_turning_speed = min_turning_speed;
    this->heading_gain = heading_gain;
    this->max_angular_speed = max_angular_speed;
    this->slow_turn_angle = slow_turn_angle;
    this->turn_in_place_angle = turn_in_place_angle;
    this->goal_tolerance = goal_tolerance;
}

void FollowTheCarrot::control_config(float look_ahead_distance, float forward_speed)
{
    this->look_ahead_distance = look_ahead_distance;
    this->forward_speed = forward_speed;
}

int FollowTheCarrot::get_idx_min_distance_to_path(const nav_msgs::msg::Path& path)
{
    float min_dist = std::numeric_limits<float>::max();
    int idx = 0;

    for (unsigned int i = 0; i < path.poses.size(); ++i) {
        const auto& pose = path.poses[i];
        const float distance = std::hypot(pose.pose.position.x, pose.pose.position.y);
        if (distance < min_dist) {
            min_dist = distance;
            idx = static_cast<int>(i);
        }
    }

    return idx;
}

int FollowTheCarrot::find_goal_point(const nav_msgs::msg::Path& path, int min_dist_idx)
{
    if (path.poses.empty()) {
        return 0;
    }

    int idx = static_cast<int>(path.poses.size()) - 1;
    for (int i = std::max(0, min_dist_idx); i < static_cast<int>(path.poses.size()); ++i) {
        const auto& pose = path.poses[i];
        const float distance = std::hypot(pose.pose.position.x, pose.pose.position.y);
        if (distance >= this->look_ahead_distance && pose.pose.position.x >= 0.0) {
            idx = i;
            break;
        }
    }

    return idx;
}

void FollowTheCarrot::calc_actions(const nav_msgs::msg::Path& path, float dist_last_obj)
{
    if (path.poses.empty()) {
        output.linear_sp = 0.0f;
        output.angular_sp = 0.0f;
        output.goal_idx = 0;
        output.cte = 0.0f;
        output.dist_last_obj = dist_last_obj;
        return;
    }

    const int min_idx = get_idx_min_distance_to_path(path);
    const int goal = find_goal_point(path, min_idx);
    const auto& look_ahead_pose = path.poses[goal];
    const float heading_error = std::atan2(look_ahead_pose.pose.position.y, look_ahead_pose.pose.position.x);
    const float abs_heading_error = std::fabs(heading_error);

    float linear_command = forward_speed;
    if (dist_last_obj <= goal_tolerance) {
        linear_command = 0.0f;
    } else if (abs_heading_error >= turn_in_place_angle) {
        linear_command = 0.0f;
    } else if (abs_heading_error >= slow_turn_angle) {
        linear_command = min_turning_speed;
    }

    float angular_command = heading_gain * heading_error;
    angular_command = std::clamp(angular_command, -max_angular_speed, max_angular_speed);
    if (dist_last_obj <= goal_tolerance) {
        angular_command = 0.0f;
    }

    output.linear_sp = linear_command;
    output.angular_sp = angular_command;
    output.goal_idx = goal;
    output.look_ahead_pose = look_ahead_pose;
    output.cte = std::hypot(path.poses[min_idx].pose.position.x, path.poses[min_idx].pose.position.y);
    output.dist_last_obj = dist_last_obj;
}