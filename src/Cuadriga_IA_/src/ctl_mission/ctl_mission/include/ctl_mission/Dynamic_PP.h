#include <vector>
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include <tf2/utils.h>
#include "nav_msgs/msg/path.hpp"

class DynamicPP
{
private:
    double look_ahead_distance ,max_speed,max_ang_acc,max_ang_dec, max_lin_acc, max_lin_dec, last_wz_sp, last_vx_sp, dt, min_speed;
    int  min_goal_idx;
public:
    DynamicPP();
    struct {             // controller output structure declaration
        float linear_sp;         
        float angular_sp;
        int goal_idx;
        geometry_msgs::msg::PoseStamped look_ahead_pose;
        float cte;
        float dist_last_obj;   
    } dynamic_pp_output;        
    void params_init( double look_ahead_distance, double max_speed, double max_ang_acc, double max_ang_dec, 
                                double max_lin_acc, double max_lin_dec);
    int find_goal_point(const nav_msgs::msg::Path& pose_vector);
    void calc_dynamic_pp_actions(const nav_msgs::msg::Path& pose_vector);
    void control_init(double look_ahead_distance, double max_speed, double max_ang_acc, double max_ang_dec, 
                                double max_lin_acc, double max_lin_dec, double dt, double min_speed);
    double calculateTotal2DDistance(const nav_msgs::msg::Path& path);
    double calculateCrossTrackError(const nav_msgs::msg::Path& path);

};


