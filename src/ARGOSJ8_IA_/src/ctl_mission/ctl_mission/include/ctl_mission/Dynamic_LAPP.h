#include <vector>
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include <tf2/utils.h>
#include "nav_msgs/msg/path.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"

class DynamicLAPP
{
private:
    double look_ahead_v_gain ,max_speed,max_ang_acc,max_ang_dec, max_lin_acc, max_lin_dec, 
            last_wz_sp, last_vx_sp, dt, min_speed, min_look_ahead_d, speed_power;
    int  min_goal_idx;
public:
    DynamicLAPP();
    struct {             // controller output structure declaration
        float linear_sp;         
        float angular_sp;
        int goal_idx;
        geometry_msgs::msg::PoseStamped look_ahead_pose;
        float cte;
        float dist_last_obj;   
    } dynamic_lapp_output;        
    void params_init( double look_ahead_v_gain, double max_speed, double max_ang_acc, double max_ang_dec, 
                                double max_lin_acc, double max_lin_dec, double speed_pow, double min_look_ahead_d);
    int find_goal_point(const nav_msgs::msg::Path& pose_vector,  const double& la_distance);
    void calc_dynamic_lapp_actions(const nav_msgs::msg::Path& pose_vector, float dist_last_obj, double v_x_odom, double w_z_odom);
    void control_init(double look_ahead_v_gain, double max_speed, double max_ang_acc, double max_ang_dec, 
                                double max_lin_acc, double max_lin_dec, double dt, double min_speed);
    double calculateTotal2DDistance(const nav_msgs::msg::Path& path);
    void controller_reinit();
    std::pair<double, double> calculateCommands(const geometry_msgs::msg::PoseStamped& look_ahead_pose, double last_vx_sp, 
                                            double last_wz_sp, double max_ang_acc, double max_lin_acc, double dt, double max_speed);


};


