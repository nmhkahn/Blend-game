#ifndef _UTIL_H_
#define _UTIL_H_

#include "cocos2d.h"
USING_NS_CC;

enum TYPE {NODE = 0, N_PIPE = 10, R_PIPE = 20, S_PIPE = 30, T_PIPE = 40};
enum COLOR {RED, BLUE, GREEN, YELLOW, MAGENTA, ORANGE};
enum COND {NONE, WIN, L_BLEND, L_NOCONN, L_TOTAL};
enum DIR {UP, DOWN, RIGHT, LEFT};

constexpr const int num_grid_width = 14;
constexpr const int num_grid_height = 8;

constexpr const int max_level = 5;

constexpr const float flow_speed = 0.05;

/* util function */
Vec2 gridToPos( const Vec2& grid );

#endif
