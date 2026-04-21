#include <algorithm>
#include <cmath>
#include <limits>

#include "geometry_msgs/msg/pose_stamped.hpp"
#include "nav_msgs/msg/path.hpp"

class FollowTheCarrot
{
private:
    float look_ahead_distance;
    float forward_speed;
    float min_turning_speed;
    float heading_gain;
    float max_angular_speed;
    float slow_turn_angle;
    float turn_in_place_angle;
    float goal_tolerance;

public:
    FollowTheCarrot();

    struct {
        float linear_sp;
        float angular_sp;
        int goal_idx;
        geometry_msgs::msg::PoseStamped look_ahead_pose;
        float cte;
        float dist_last_obj;
    } output;

    void control_init(
        float look_ahead_distance,
        float forward_speed,
        float min_turning_speed,
        float heading_gain,
        float max_angular_speed,
        float slow_turn_angle,
        float turn_in_place_angle,
        float goal_tolerance);

    void control_config(float look_ahead_distance, float forward_speed);
    int get_idx_min_distance_to_path(const nav_msgs::msg::Path& path);
    int find_goal_point(const nav_msgs::msg::Path& path, int min_dist_idx);
    void calc_actions(const nav_msgs::msg::Path& path, float dist_last_obj);
};