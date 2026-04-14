// Directive to enable the MATLAB exporting functionality: VERBOSE_MATLAB.
#ifndef DEMAIAS_ROS2node
	#define VERBOSE_MATLAB
#endif

// Directive to define whether or not using automaton prunning.
// If this directive is not defined, automaton prunning is applied.
// #define NON_AUTOMATON_PRUNING

// Directives to define: (1) whether or not using any-angle path-search algorithm, and (2) type of neighbourhood criterion (4, or 8).
// If these directives are not defined, any-angle path-search algorithm and 8-neighbourhood are used.
// #define NON_ANY_ANGLE
// #define EIGHT-NEIGHBOURHOOD

// Directive to define whether or not using symmetric slope estimations as 3Dana algorithm.
// If this directive is not defined, asymmetric slope-awareness is used, as DEM-AIA planner algorithm.
// #define NON_ASYMMETRIC

// Directive to define type of heuristic function used in graph-based path search algorithm: (1) Euclidean travel time, or (2) octile travel time.
// If this directive is not defined, Dijkstra-like algorithm (non-heuristic) is used.
// #define EUCLIDEAN_TIME
// #define OCTILE_TIME
// #define OCTILE_2D_TIME

// Directive to define the verbose level (select one of them): VERBOSE_NONE, VERBOSE_TIME, VERBOSE_DEFAULT, VERBOSE_COMPLETE or VERBOSE_DEBUG.
// If not defined, the verbose level will be VERBOSE_DEFAULT (show running times, basic data of candidates suffix and prefix paths, and resulting path).
// #define VERBOSE_NONE			// show nothing
#define VERBOSE_TIME			// show resulting path and running times
// #define VERBOSE_COMPLETE		// show Buchi automata and candidates suffix and prefix paths
// #define VERBOSE_DEBUG		// show detailed information in each iteration of the path-search process

// Directive to define the UGV used for planning (select one of them): UMA_CUADRIGA or ARGOS_J8.
// If not defined, UMA_CUADRIGA will be used.
// #define UMA_CUADRIGA			// selecting UMA Cuadriga
#define ARGOS_J8				// selecting Argos J8