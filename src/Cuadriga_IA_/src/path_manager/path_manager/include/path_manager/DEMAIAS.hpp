#ifndef DEMAIAS_HPP
#define DEMAIAS_HPP

#include <bits/stdc++.h>
#include "DEMAIAS_directives.hpp"
#ifdef DEMAIAS_ROS2node
	#include "nav_msgs/msg/path.hpp"
	#include "geometry_msgs/msg/pose_stamped.hpp"
#endif

namespace DEMAIAS
{
// Including MATLAB exporting functionality
// WARNING: This functionality penalizes the computational cost due to the additional functions included to compile
//          and even causes segmentation faults when multiple instances of planner are called from the main function.
//          Possibly due to memory deallocation failures, even only including the heading file.
#ifdef VERBOSE_MATLAB
	#include <mat.h>  // including extended MATLAB functionalities (exporting MAT files)
#endif


// constant as number of hyphen characters in the text string as a separator in the terminal
#define MAX_STRING 150

// User-defined data type for real numbers (non integers), storing floating-point numbers:
//  float        : requires  4-bytes for storing in memory and holds floating values up to  7 digits;
//  double       : requires  8-bytes for storing in memory and holds floating values up to 15 digits; or
//  long double  : requires 16-bytes for storing in memory and holds floating values up to 23 digits.
typedef double REAL_TYPE;

// User-defined data type for loop-for statements
typedef long long int FOR_TYPE;

// user-defined data type for mapping vectors
typedef unsigned long long int KEY_TYPE;


// Variables for representing DEM
int __DEMtype;													// type of DEM (2:2D, 3:2.5D o 6:2.5D+RGB)
int __dimX, __dimY;												// DEM dimension in X and Y axes
REAL_TYPE __minX, __minY, __DEM_resolution;						// DEM minimum XY coordinates and resolution
int __ORTHO_dimX, __ORTHO_dimY;									// Orthophoto dimension in X and Y axes
REAL_TYPE __ORTHO_minX, __ORTHO_minY, __ORTHO_resolution;		// Orthophoto minimum XY coordinates and resolution


// User-defined data type for system state (modelling of UGV motion, i.e. XY integer coordinates and orientation angle)
const REAL_TYPE INFINITO = std::numeric_limits<REAL_TYPE>::infinity();
const REAL_TYPE OBSTACLE = std::numeric_limits<REAL_TYPE>::infinity();
struct __SYS_STATE {
	REAL_TYPE x,y;    // XY coordinates
	REAL_TYPE z;      // elevation (in case of 2.5D environment)
//	REAL_TYPE theta;  // orientation angle
	__SYS_STATE(){ x = 0.0; y = 0.0; z = (__DEMtype == 2) ? 0.0 : INFINITO; } // theta = 0.0; }
	__SYS_STATE(REAL_TYPE a, REAL_TYPE b) { x = a; y = b; z = (__DEMtype == 2) ? 0.0 : INFINITO; } // theta = 0.0; }
//	__SYS_STATE(REAL_TYPE a, REAL_TYPE b, REAL_TYPE c) { x = a; y = b; theta = c; z = (__DEMtype == 2) ? 0.0 : INFINITO; }
};

// Function to compute the key-index related with XY coordinates, and orientation angle (in case of 2.5D DEM)
KEY_TYPE __keyGrid(__SYS_STATE& point) { return KEY_TYPE (point.x + point.y * __dimX); }

// Function to convert from UTM coordinates to system state (integer XY coordinates and orientation angle)
__SYS_STATE _utm_to_xy(std::vector <REAL_TYPE> &utm) {
	__SYS_STATE result;
	result.x = (REAL_TYPE)round((utm[0] - __minX) / __DEM_resolution);
	result.y = (REAL_TYPE)round((utm[1] - __minY) / __DEM_resolution);
//	result.theta = 0.0;
	return result;
}

// Function to convert from UTM coordinates to integer XY coordinates, over orthophoto
__SYS_STATE _utm_to_orthophoto(std::vector <REAL_TYPE> &utm) {
	__SYS_STATE result;
	result.x = (REAL_TYPE)((utm[0] - __ORTHO_minX) / __ORTHO_resolution);
	result.y = (REAL_TYPE)((utm[1] - __ORTHO_minY) / __ORTHO_resolution);
//	result.theta = 0.0;
	return result;
}

// Function to compute the key-index related with XY coordinates, and orientation angle over orthophoto
KEY_TYPE __keyOrthophoto(__SYS_STATE& point) { return KEY_TYPE (point.x + point.y * __ORTHO_dimX); }

// Function to compute the key-index related with XY coordinates, and orientation angle (in case of 2.5D DEM)
KEY_TYPE __key(__SYS_STATE& point) { return KEY_TYPE (point.x + point.y * __dimX); } // + point.theta * __dimX * __dimY); }



// Variables for representing Büchi automaton equivalent with LTL formula
FOR_TYPE __Qstates_cnt;											// total number of automaton states, each state corresponds to a number being zero the initial state
FOR_TYPE __Qtransitions_cnt;									// total number of automaton transitions
std::vector <int> __accepting_Qstates;							// set of accepting automaton states
std::unordered_map <std::string, int> __bool_prop;				// variable as key-value mapping of atomic propositions: identificator-number 
std::unordered_map <int, std::string> __id_prop;				// variable as key-value mapping of atomic propositions: number-identificator
std::vector <std::vector <__SYS_STATE>> __SstatesByProp;		// given a proposition (number), indicates a vector of system states where that proposition is satisfied
std::unordered_map <KEY_TYPE, std::vector <int>> __propByKey;	// given a point key, indicates a vector of propositions (numbers) that are satisfied at that system state
std::vector <__SYS_STATE> __SstatesWithProp;					// system states list where some proposition is satisfied


// Variables for representing pruned (simplified) Büchi automaton equivalent with LTL formula
std::vector <std::vector <std::vector <std::vector <int>>>> __Qtransitions;		// each state pair corresponds to a integer vector (proposition numbers involved in a logical conjunction)
std::vector <std::vector <std::vector <std::vector <int>>>> __negativeQtransitions;
std::vector <std::vector <std::vector <std::vector <int>>>> __positiveQtransitions;
std::vector <int> __negativeQtransitions_nonLoop;								// number of negative transitions (non self-loop) for each automaton state


// User-defined data type for input arguments of DAMAIAS planner
struct arguments {
	std::unordered_map <KEY_TYPE, REAL_TYPE>					XYZgrid;	// items are -1 (obstacles) or real numeric values (elevation Z in 2.5D environment)
	std::unordered_map <KEY_TYPE, std::vector <unsigned int>>	RGBgrid;
//	automata													automaton;
//	mappingRelation												mapping;
	std::vector <std::vector <REAL_TYPE>>						vRef;
	__SYS_STATE													s0;
};
const REAL_TYPE __step = 0.01;												// can be changed to 0.01 rad (0.573 degree) or 0.001 rad (0.0573 degree)
arguments* __inputs;
REAL_TYPE __symmetricSlope;



struct segmentItem {
	__SYS_STATE					point1;
	__SYS_STATE					point2;
	std::vector <__SYS_STATE>	perp;
	REAL_TYPE					refSpeed;
	REAL_TYPE					proposedSlope;
	REAL_TYPE					slope;
	REAL_TYPE					tilt;
	segmentItem(__SYS_STATE a, __SYS_STATE b) {
		point1					= a;
		point2					= b;
		perp					= std::vector <__SYS_STATE>(0);
		refSpeed				= 0.0;
	}
	segmentItem() {
		__SYS_STATE nullPoint	= __SYS_STATE(INFINITO,INFINITO);
		point1					= nullPoint;
		point2					= nullPoint;
		perp					= std::vector <__SYS_STATE>(0);
		refSpeed				= 0.0;
	}
};


// User-defined data type for product automaton nodes and resulting trajectory/path
struct prodNode {
	__SYS_STATE					pose;
	int							Qstate;
	REAL_TYPE					g, h, f;
	std::vector <segmentItem>	segment;
	prodNode*					parent;
};
struct __PATH {
	int							id;
	std::deque <prodNode*>		node;
	std::deque <__SYS_STATE>	waypoint;
	std::deque <REAL_TYPE>		refSpeed;
	std::deque <REAL_TYPE> 		proposedSlope;
	std::deque <REAL_TYPE> 		slope;
	std::deque <REAL_TYPE> 		tilt;
	REAL_TYPE					total_cost;
	FOR_TYPE					iterations;
};
const int NON_QSTATE	= std::numeric_limits<int>::max();

// Function to compute the key-index related with XY coordinates and state
KEY_TYPE __nodeKey(prodNode* node) {
	return KEY_TYPE (node->pose.x + node->pose.y * __dimX + node->Qstate * __dimX * __dimY ); // + node->pose.theta * __dimX * __dimY * __Qstates_cnt);
}

std::unordered_map <KEY_TYPE, prodNode*> __discoveredNode;									// each item is a pointer to a product automaton node when this is discovered (after node exploration)



// Function to allocate a path node
__PATH* new_path_node(REAL_TYPE ini_cost) {
	__PATH* node		= new __PATH;
	node->id			= 0;
	node->node			= std::deque <prodNode*>(0);
	node->waypoint		= std::deque <__SYS_STATE>(0);
	node->total_cost	= ini_cost;
	node->iterations	= 0;
	return node;
}
const REAL_TYPE EPSILON	= 1e-10;
__PATH* __suffix_path = new_path_node(INFINITO);
__PATH* __prefix_path = new_path_node(0.0);

// Function to allocate a product graph node
prodNode* new_prod_graph_node(__SYS_STATE pose, int Qstate) {
	prodNode* node		= new prodNode;
	node->pose			= pose;
	node->Qstate		= Qstate;
	node->g				= 0.0;
	node->h				= 0.0;
	node->f				= 0.0;
	node->parent		= NULL;
	return node;
}

// Function to compute the heuristic value associated with a system state, distance between two points
REAL_TYPE euclidean (std::vector<REAL_TYPE> pA, std::vector<REAL_TYPE> pB) {
	REAL_TYPE dist = 0.0;
	for (FOR_TYPE i = 0; i < (FOR_TYPE) pA.size(); i++) dist += std::pow(pB[i] - pA[i], 2);
	return (REAL_TYPE) std::sqrt(dist);
}


// VARIABLES TO CONTAIN GRID POINTS WHERE AUTOMATON TRANSITIONS ARE SATISFIED
// variable computed for 'compute_accepting_product_automaton_states' function
// used to vector queries from 'compute_accepting_paths' function
std::vector< std::vector <int>> _qsystate;		// given an automaton state (specially an accepting one),
												//  indicates a vector of point indices (to use only with __SstatesWithProp) where some incoming transition is satisfied

// variable computed for 'compute_accepting_product_automaton_states' function
// used to vector queries from 'astar_path_src_to_dest', 'cal_edge_cost', 'modifyedges' and 'expand_prod_graph_node' function
std::vector< std::vector< std::vector<int>>> _qsytrans;  // given an pair of automaton states A and B,
                                           //  indicates a vector of point indices (for __SstatesWithProp) where an transition A->B is satisfied




struct prod_graph_comparator {
	bool operator()(prodNode* a, prodNode* b) { return (a->f > b->f); }
};



// Main function for LTL-based strategic UGV mission planner.
// Input arguments: LTL mission (.dat), DEM (.dat or xyz) and atomic propositions mapping (.dat) specification files
#ifdef DEMAIAS_ROS2node
	std::pair <nav_msgs::msg::Path, nav_msgs::msg::Path> planner(char* DEM, char* mapping, char* LTL, char* orthophoto);
#else
	void planner(char* DEM, char* mapping, char* LTL, char* orthophoto);
#endif

// Inner functions.
arguments* initialize(char* dem_filename, char* proposition_map_filename, char* mission_filename, char* orthophoto_filename);
void compute_accepting_product_automaton_states();
std::deque <__PATH*> compute_suffix_prefix_paths(bool cycle);
__PATH* path_search(prodNode* srcNode, std::vector <prodNode*> destNodes, REAL_TYPE);

#ifdef VERBOSE_MATLAB
	void write_MATfile(std::vector <REAL_TYPE>);
#endif

}

#endif	// DEMAIAS_HPP