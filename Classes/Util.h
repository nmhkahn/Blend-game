#ifndef _UTIL_H_
#define _UTIL_H_

#include "cocos2d.h"
USING_NS_CC;

enum TYPE {NODE = 0, N_PIPE = 10, R_PIPE = 20, S_PIPE = 30, T_PIPE = 40};
enum COLOR {RED, BLUE, GREEN, YELLOW, MAGENTA, ORANGE};
enum DIR {UP, DOWN, RIGHT, LEFT};

constexpr const int num_grid_width = 14;
constexpr const int num_grid_height = 8;

constexpr const int max_stage = 4;

constexpr const int lose_blend = 1;
constexpr const int lose_noconnect = 2;
constexpr const int lose_total = 3;
constexpr const int win = 4;

/* util function */
Vec2 gridToPos( const Vec2& grid );

#endif
