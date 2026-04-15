#ifndef DEMAIAS_CLASS_HPP
#define DEMAIAS_CLASS_HPP

#include <bits/stdc++.h>
#include "DEMAIAS_directives.hpp"
#include "nav_msgs/msg/path.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"

#ifdef VERBOSE_MATLAB
#include <mat.h>  // including extended MATLAB functionalities (exporting MAT files)
#endif


typedef double REAL_TYPE;
typedef long long int FOR_TYPE;
typedef unsigned long long int KEY_TYPE;

class Planner {
public:
    Planner();
    ~Planner();

      /*std::pair<nav_msgs::msg::Path, nav_msgs::msg::Path> plan(const std::string& dem_filename,
                                                             const std::string& mapping_filename,
                                                             const std::string& ltl_filename,
                                                             const std::string& orthophoto_filename);*/
        std::pair<nav_msgs::msg::Path, nav_msgs::msg::Path> plan(const geometry_msgs::msg::Point& start,
                                                             const geometry_msgs::msg::Point& goal);
        void dumb_fnc(void);

private:
    double distance(const geometry_msgs::msg::Point& p1, const geometry_msgs::msg::Point& p2);
    struct __SYS_STATE {
        REAL_TYPE x, y, z;
        __SYS_STATE() : x(0.0), y(0.0), z(0.0) {}
        __SYS_STATE(REAL_TYPE a, REAL_TYPE b, REAL_TYPE dem_type, REAL_TYPE inf) 
            : x(a), y(b), z(dem_type == 2 ? 0.0 : inf) {}
    };

    struct segmentItem {
        __SYS_STATE point1;
        __SYS_STATE point2;
        std::vector<__SYS_STATE> perp;
        REAL_TYPE refSpeed;
        REAL_TYPE proposedSlope;
        REAL_TYPE slope;
        REAL_TYPE tilt;
        segmentItem(__SYS_STATE a, __SYS_STATE b) : point1(a), point2(b), refSpeed(0.0) {}
        segmentItem(REAL_TYPE inf) : point1(__SYS_STATE(inf, inf, 0, inf)), point2(__SYS_STATE(inf, inf, 0, inf)), refSpeed(0.0) {}
    };

    struct prodNode {
        __SYS_STATE pose;
        int Qstate;
        REAL_TYPE g, h, f;
        std::vector<segmentItem> segment;
        prodNode* parent;
    };

    struct __PATH {
        int id;
        std::deque<prodNode*> node;
        std::deque<__SYS_STATE> waypoint;
        std::deque<REAL_TYPE> refSpeed;
        std::deque<REAL_TYPE> proposedSlope;
        std::deque<REAL_TYPE> slope;
        std::deque<REAL_TYPE> tilt;
        REAL_TYPE total_cost;
        FOR_TYPE iterations;
    };

    struct arguments {
        std::unordered_map<KEY_TYPE, REAL_TYPE> XYZgrid;
        std::unordered_map<KEY_TYPE, std::vector<unsigned int>> RGBgrid;
        std::vector<std::vector<REAL_TYPE>> vRef;
        __SYS_STATE s0;
    };

    std::string __nodeKey(const prodNode* node);
    __PATH* new_path_node(REAL_TYPE ini_cost);
    prodNode* new_prod_graph_node(__SYS_STATE pose, int Qstate);
    REAL_TYPE euclidean(std::vector<REAL_TYPE> pA, std::vector<REAL_TYPE> pB);
    arguments* initialize(const std::string& dem_filename, const std::string& proposition_map_filename,
                          const std::string& mission_filename, const std::string& orthophoto_filename);
    void compute_accepting_product_automaton_states();
    std::deque<__PATH*> compute_suffix_prefix_paths(bool cycle);
    __PATH* path_search(prodNode* srcNode, std::vector<prodNode*> destNodes, REAL_TYPE cost);
    bool isEqual(prodNode* nodeA, prodNode* nodeB);
    void create_automaton_from_ltl(const char* command);

#ifdef VERBOSE_MATLAB
    void write_MATfile(std::vector<REAL_TYPE>);
#endif

    // Helper functions
    __SYS_STATE _utm_to_xy(const std::vector<REAL_TYPE>& utm);
    KEY_TYPE __keyGrid(const __SYS_STATE& point);

    // Variables for representing DEM
    int __DEMtype;
    int __dimX, __dimY;
    REAL_TYPE __minX, __minY, __DEM_resolution;
    int __ORTHO_dimX, __ORTHO_dimY;
    REAL_TYPE __ORTHO_minX, __ORTHO_minY, __ORTHO_resolution;

    // Variables for representing Büchi automaton equivalent with LTL formula
    FOR_TYPE __Qstates_cnt;
    FOR_TYPE __Qtransitions_cnt;
    std::vector<int> __accepting_Qstates;
    std::unordered_map<std::string, int> __bool_prop;
    std::unordered_map<int, std::string> __id_prop;
    std::vector<std::vector<__SYS_STATE>> __SstatesByProp;
    std::unordered_map<KEY_TYPE, std::vector<int>> __propByKey;
    std::vector<__SYS_STATE> __SstatesWithProp;

    // Variables for representing pruned (simplified) Büchi automaton equivalent with LTL formula
    std::vector<std::vector<std::vector<std::vector<int>>>> __Qtransitions;
    std::vector<std::vector<std::vector<std::vector<int>>>> __negativeQtransitions;
    std::vector<int> __negativeQtransitions_nonLoop;

    // Other member variables
    const REAL_TYPE INFINITO;
    const REAL_TYPE OBSTACLE;
    const REAL_TYPE EPSILON;
    __PATH* __suffix_path;
    __PATH* __prefix_path;
    std::unordered_map<KEY_TYPE, prodNode*> __discoveredNode;
};


#endif // DEMAIAS_CLASS_HPP
