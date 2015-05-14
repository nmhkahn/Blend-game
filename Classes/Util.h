#ifndef _UTIL_H_
#define _UTIL_H_

#include "cocos2d.h"
USING_NS_CC;

enum Type {COLOR_NODE = 0, PIPE = 10, ROTATABLE_PIPE = 20, ONEWAY_PIPE = 30};
enum Color {RED, GREEN, BLUE, YELLOW, MAGENTA, ORANGE, WHITE};
enum Direc {UP, DOWN, RIGHT, LEFT};

constexpr const int num_grid_width = 14; //  14 x 8 grid
constexpr const int num_grid_height = 8; //

constexpr const int max_stage = 2;


// util function for rotate
Vec2 CRotate( Vec2 target, float dtheta );
Vec2 CRotateOnAnchor( Vec2 target, float dtheta, Vec2 anchor );

#endif
