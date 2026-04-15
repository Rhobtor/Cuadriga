#include <vector>
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include <tf2/utils.h>
#include "nav_msgs/msg/path.hpp"
class Stanley_Ctrl
{
private:
    float look_ahead_distance ,K_angle,K_position,forward_speed, decelerate_init;
 
public:
    Stanley_Ctrl();
    struct {             // controller output structure declaration
        float linear_sp;         
        float angular_sp;
        int goal_idx;
        geometry_msgs::msg::PoseStamped look_ahead_pose;
        float cte;
        float dist_last_obj;   
    } stanley_output;        
    void params_init(float look_ahead_distance, float K_angle, float K_position, float forward_speed);
    //Stanley_Ctrl(float look_ahead_distance, float K_angle, float K_position, float forward_speed, int points_in_window );
    int  get_idx_min_distance_to_path(const nav_msgs::msg::Path& path);
    int find_goal_point(const nav_msgs::msg::Path& path, int min_dist_idx);
    void calc_stanley_actions(const nav_msgs::msg::Path& path);
    void control_init();
};


