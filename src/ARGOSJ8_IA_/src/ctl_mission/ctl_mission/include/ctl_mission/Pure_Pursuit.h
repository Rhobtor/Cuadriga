#include <vector>
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include <tf2/utils.h>
#include "nav_msgs/msg/path.hpp"
class PurePursuit
{
private:
    float look_ahead_distance ,forward_speed, current_speed, dt, min_speed, breaking_acc;
 
public:
    PurePursuit();
    struct {             // controller output structure declaration
        float linear_sp;         
        float angular_sp;
        int goal_idx;
        geometry_msgs::msg::PoseStamped look_ahead_pose;
        float cte;
        float dist_last_obj;   
    } pure_pursuit_output;        
    void control_config(float look_ahead_distance, float forward_speed);
    int  get_idx_min_distance_to_path(const nav_msgs::msg::Path& path);
    int find_goal_point(const nav_msgs::msg::Path& path, int min_dist_idx);
    void calc_pure_pursuit_actions(const nav_msgs::msg::Path& path, float dist_last_obj);
    void control_init(double dt, float look_ahead_distance, float forward_speed, float min_speed, float breaking_acc);
    double calculateTotal2DDistance(const nav_msgs::msg::Path& path);

};


