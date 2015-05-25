#ifndef _MATERIALS_H_
#define _MATERIALS_H_

#include "cocos2d.h"
USING_NS_CC;

struct Grid : public Sprite
{
    virtual bool init();
    
    Vec2 _coord;                // grid coordinate
    bool _visit;                // visit flag for bfs search
    std::vector<Vec2> _connect; // store connected grid
    
    Color3B _color;             // carry color
    int _entity;                // carry color entity
};

struct ColorNode : public Grid
{
    static ColorNode* create( const Vec2& coord );
    void initColorNode( const int& color, const int& entity );
};

struct Pipe : public Grid
{
    static Pipe* create( const Vec2& coord );
    void initPipe( const int& pipeType, const int& rotate );
    void setSpriteByRotate();
    
    int _pipeType;  // pipe type : 0, 1, 2, 3
    int _rotate;    // clockwise : 0, 1, 2, 3
};

struct RotatablePipe : public Pipe
{
    static RotatablePipe* create( const Vec2& coord );
    void initRPipe( const int& pipeType, const int& rotate );
    void rotatePipe();
    
    Sprite* _ground; // ground sprite
};

struct SwitchPipe : public Pipe
{
    static SwitchPipe* create( const Vec2& coord );
    void initSPipe( const int& rotate1, const int& rotate2 );
    void switchPipe();
    
    int _switch[2];  // store pipe type for each switch
    int _swtrot[2];  // store rotate for each switch
    int _current;    // current switch
    Sprite* _ground; // ground sprite
};

struct TunnelPipe : public Pipe
{
    static TunnelPipe* create( const Vec2& coord );
    void initTPipe( const Vec2& end, const int& type, const int& rotate );
    
    int _type;       // rotate of tunnel (change of sprite)
    Vec2 _end;       // coordinate of connected tunnel
    Sprite* _tunnel; // tunnel sprite
};

#endif