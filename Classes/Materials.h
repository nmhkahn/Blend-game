#ifndef _MATERIALS_H_
#define _MATERIALS_H_

#include "cocos2d.h"
USING_NS_CC;

struct Grid : public Sprite
{
    virtual bool init();
    
    int _gridX, _gridY;
    bool _visit;
    std::vector<std::pair<int, int>> _connect; // connected grid -> pair(gridX, gridY)
};

struct ColorNode : public Grid
{
    static ColorNode* create( const int& gridX,
                              const int& gridY );
    void initColorNode( const int& color, const int& entity );
    
    Color3B _color;
    int _entity; // 0 to 255
};

struct Pipe : public Grid
{
    static Pipe* create( const int& gridX,
                         const int& gridY );
    void initPipe( const int& type, const int& pipe, const int& rotate );
    
    int _type; // e.g normal, rotatable ....
    int _pipe; // type of pipe
    int _rotate;
    
    Color3B _carry_color;
    int _carry_entity;
};

#endif