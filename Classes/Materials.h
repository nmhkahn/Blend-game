#ifndef _MATERIALS_H_
#define _MATERIALS_H_

#include "cocos2d.h"
USING_NS_CC;

struct Grid : public Sprite
{
    virtual bool init();
    
    Vec2 _coord;                /* grid coordinate */
    bool _visit;                /* for bfs search */
    std::vector<Vec2> _connect; /* connected grid */
    
    Color3B _color;
    int _entity;
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
    
    int _pipeType;
    int _rotate;
};

struct RotatablePipe : public Pipe
{
    static RotatablePipe* create( const Vec2& coord );
    void initRPipe( const int& pipeType, const int& rotate );
    void rotatePipe();
    
    Sprite* _ground;
};

struct SwitchPipe : public Pipe
{
    static SwitchPipe* create( const Vec2& coord );
    void initSPipe( const int& rotate1, const int& rotate2 );
    void switchPipe();
    
    int _switch[2];
    int _swtrot[2];
    int _current;
    Sprite* _ground;
};

struct TunnelPipe : public Pipe
{
    static TunnelPipe* create( const Vec2& coord );
    void initTPipe( const Vec2& end, const int& type, const int& rotate );
    
    int _type;
    Vec2 _end;
    Sprite* _tunnel;
};

#endif