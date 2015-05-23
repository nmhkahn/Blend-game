#include <vector>
#include <iostream>
using namespace std;

#include "Materials.h"
#include "Util.h"

/* GRID */
bool Grid::init()
{
    _visit = false;
    
    return initWithTexture(nullptr, Rect::ZERO );;
}


/* COLORNODE */
ColorNode* ColorNode::create( const Vec2& coord )
{
    ColorNode* node = new (nothrow) ColorNode();
    if( node && node->init() )
    {
        node->autorelease();
        node->_coord = coord;
        node->setPosition(gridToPos(node->_coord));
    }
    else
    {
        CC_SAFE_DELETE(node);
    }
    
    return node;
}

void ColorNode::initColorNode( const int& color, const int& entity )
{
    if( color == COLOR::RED) _color = Color3B::RED, setColor(Color3B::RED);
    if( color == COLOR::BLUE) _color = Color3B::BLUE, setColor(Color3B::BLUE);
    if( color == COLOR::GREEN) _color = Color3B::GREEN, setColor(Color3B::GREEN);
    if( color == COLOR::YELLOW) _color = Color3B::YELLOW, setColor(Color3B::YELLOW);
    if( color == COLOR::MAGENTA) _color = Color3B::MAGENTA, setColor(Color3B::MAGENTA);
    if( color == COLOR::ORANGE) _color = Color3B::ORANGE, setColor(Color3B::ORANGE);
    _entity = entity;
    
    _connect.push_back(Vec2(_coord.x+1, _coord.y));
    _connect.push_back(Vec2(_coord.x-1, _coord.y));
    _connect.push_back(Vec2(_coord.x, _coord.y+1));
    _connect.push_back(Vec2(_coord.x, _coord.y-1));
    
    setTexture("res/node.png");
    setScale(0.75, 0.75);
    setTag(TYPE::NODE);
    setOpacity(_entity);
}


/* PIPE */
Pipe* Pipe::create( const Vec2& coord )
{
    Pipe* pipe = new (nothrow) Pipe();
    if( pipe && pipe->init() )
    {
        pipe->autorelease();
        pipe->_coord = coord;
        
        pipe->setPosition(gridToPos(pipe->_coord));
        pipe->setTag(TYPE::N_PIPE);
        pipe->setScale(0.75, 0.75);
    }
    else
    {
        CC_SAFE_DELETE(pipe);
    }
    
    return pipe;
}

void Pipe::initPipe( const int& pipeType, const int& rotate )
{
    _pipeType = pipeType;
    _rotate = rotate;
    _color = Color3B::WHITE;
    _entity = 0;
    
    _connect.clear();

    if( _pipeType == 0 )
    {
        setTexture("res/pipe0.png");
        
        if( _rotate % 2 == 0 )
        {
            _connect.push_back(Vec2(_coord.x+1, _coord.y));
            _connect.push_back(Vec2(_coord.x-1, _coord.y));
        }
        else
        {
            _connect.push_back(Vec2(_coord.x, _coord.y+1));
            _connect.push_back(Vec2(_coord.x, _coord.y-1));
        }
    }
    else if( _pipeType == 1 )
    {
        setTexture("res/pipe1.png");
        
        if( _rotate == 0 )
        {
            _connect.push_back(Vec2(_coord.x-1, _coord.y));
            _connect.push_back(Vec2(_coord.x, _coord.y+1));
        }
        else if( _rotate == 1 )
        {
            _connect.push_back(Vec2(_coord.x+1, _coord.y));
            _connect.push_back(Vec2(_coord.x, _coord.y+1));
        }
        else if( _rotate == 2 )
        {
            _connect.push_back(Vec2(_coord.x+1, _coord.y));
            _connect.push_back(Vec2(_coord.x, _coord.y-1));
        }
        else if( _rotate == 3 )
        {
            _connect.push_back(Vec2(_coord.x-1, _coord.y));
            _connect.push_back(Vec2(_coord.x, _coord.y-1));
        }
    }
    else if( _pipeType == 2 )
    {
        setTexture("res/pipe2.png");
        
        if( _rotate == 0 )
        {
            _connect.push_back(Vec2(_coord.x-1, _coord.y));
            _connect.push_back(Vec2(_coord.x+1, _coord.y));
            _connect.push_back(Vec2(_coord.x, _coord.y+1));
        }
        else if( _rotate == 1 )
        {
            _connect.push_back(Vec2(_coord.x+1, _coord.y));
            _connect.push_back(Vec2(_coord.x, _coord.y-1));
            _connect.push_back(Vec2(_coord.x, _coord.y+1));
        }
        else if( _rotate == 2 )
        {
            _connect.push_back(Vec2(_coord.x-1, _coord.y));
            _connect.push_back(Vec2(_coord.x+1, _coord.y));
            _connect.push_back(Vec2(_coord.x, _coord.y-1));
        }
        else if( _rotate == 3 )
        {
            _connect.push_back(Vec2(_coord.x-1, _coord.y));
            _connect.push_back(Vec2(_coord.x, _coord.y-1));
            _connect.push_back(Vec2(_coord.x, _coord.y+1));
        }
        
    }
    else if( _pipeType == 3 )
    {
        setTexture("res/pipe3.png");
        
        _connect.push_back(Vec2(_coord.x-1, _coord.y));
        _connect.push_back(Vec2(_coord.x, _coord.y+1));
        _connect.push_back(Vec2(_coord.x+1, _coord.y));
        _connect.push_back(Vec2(_coord.x, _coord.y-1));
    }
    
    setRotation(90*_rotate);
}


/* ROTATABLE PIPE */
RotatablePipe* RotatablePipe::create( const Vec2& coord )
{
    RotatablePipe* pipe = new (nothrow) RotatablePipe();
    if( pipe && pipe->init() )
    {
        pipe->autorelease();
        pipe->_coord = coord;
        
        pipe->setPosition(gridToPos(pipe->_coord));
        pipe->setTag(TYPE::R_PIPE);
        pipe->setScale(0.75, 0.75);
    }
    else
    {
        CC_SAFE_DELETE(pipe);
    }
    
    return pipe;
}

void RotatablePipe::initRPipe( const int& pipeType, const int& rotate )
{
    initPipe(pipeType, rotate);
    
    _ground->setTexture("res/r_ground");
    _ground->setPosition(getPosition());
}


/* SWITCH PIPE */
SwitchPipe* SwitchPipe::create( const Vec2& coord )
{
    SwitchPipe* pipe = new (nothrow) SwitchPipe();
    if( pipe && pipe->init() )
    {
        pipe->autorelease();
        pipe->_coord = coord;
       
        pipe->setPosition(gridToPos(pipe->_coord));
        pipe->setTag(TYPE::S_PIPE);
        pipe->setScale(0.75, 0.75);
    }
    else
    {
        CC_SAFE_DELETE(pipe);
    }
    
    return pipe;
}

void SwitchPipe::initSPipe( const int &pipeType1, const int &pipeType2, const int &rotate )
{
    _pipeType[0] = pipeType1, _pipeType[1] = pipeType2;
    initPipe(_pipeType[0], rotate);
    
    _ground->setTexture("res/s_ground");
    _ground->setPosition(getPosition());
}


/* TUNNEL PIPE */
TunnelPipe* TunnelPipe::create( const Vec2& coord )
{
    TunnelPipe* pipe = new (nothrow) TunnelPipe();
    if( pipe && pipe->init() )
    {
        pipe->autorelease();
        pipe->_coord = coord;
        
        pipe->setPosition(gridToPos(pipe->_coord));
        pipe->setTag(TYPE::T_PIPE);
        pipe->setScale(0.75, 0.75);
    }
    else
    {
        CC_SAFE_DELETE(pipe);
    }
    
    return pipe;
}

void TunnelPipe::initTPipe( const int &pipeType, const int &type, const int &rotate )
{
    _type = type;
    initPipe(pipeType, rotate);
    
    _tunnel->setTexture("res/t_tunnel");
    _tunnel->setPosition(getPosition());
}