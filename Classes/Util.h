#ifndef _UTIL_H_
#define _UTIL_H_

#include "cocos2d.h"
USING_NS_CC;

enum TYPE {NODE = 0, N_PIPE = 10, R_PIPE = 20, S_PIPE = 30, T_PIPE = 40};
enum COLOR {RED = 0, BLUE = 1, GREEN = 2, YELLOW = 3, MAGENTA = 4, ORANGE = 5, WHITE = 6};
enum COND {NONE, WIN, L_BLEND, L_NOCONN, L_TOTAL};
enum DIR {UP, DOWN, RIGHT, LEFT};

constexpr const int num_grid_width = 13;
constexpr const int num_grid_height = 7;

constexpr const int start_level = 1;
constexpr const int max_level = 10;

constexpr const float flow_speed = 0.1;

/* util function */
Vec2 gridToPos( const Vec2& grid ); // change grid coordinate to Vec2 position
std::string int_to_string( int input );  // instead std::to_string

#endif
