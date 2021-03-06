#include <vector>
#include <iostream>
using namespace std;

#include "Materials.h"
#include "Util.h"

/* GRID */
Grid* Grid::create( const Vec2& coord )
{
    Grid* grid = new (nothrow) Grid();
    if( grid && grid->init() )
    {
        grid->autorelease();
        grid->_coord = coord;
        grid->setPosition(gridToPos(grid->_coord));
        grid->_back->setPosition(grid->getPosition());
    }
    else
    {
        CC_SAFE_DELETE(grid);
    }
    
    return grid;
}

bool Grid::init()
{
    _back = Sprite::create();
    _back->setScale(0.75, 0.75);
    _back->setColor(Color3B::WHITE);
    
    setScale(0.75, 0.75);
    return initWithTexture(nullptr, Rect::ZERO );
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
    if( color == COLOR::RED)     _color = Color3B::RED,     setColor(Color3B::RED);
    if( color == COLOR::BLUE)    _color = Color3B::BLUE,    setColor(Color3B::BLUE);
    if( color == COLOR::GREEN)   _color = Color3B::GREEN,   setColor(Color3B::GREEN);
    if( color == COLOR::YELLOW)  _color = Color3B::YELLOW,  setColor(Color3B::YELLOW);
    if( color == COLOR::MAGENTA) _color = Color3B::MAGENTA, setColor(Color3B::MAGENTA);
    if( color == COLOR::ORANGE)  _color = Color3B::ORANGE,  setColor(Color3B::ORANGE);
    if( color == COLOR::WHITE)   _color = Color3B::WHITE,   setColor(Color3B::WHITE);
    _entity = entity;
    
    _connect.push_back(Vec2(_coord.x+1, _coord.y));
    _connect.push_back(Vec2(_coord.x-1, _coord.y));
    _connect.push_back(Vec2(_coord.x, _coord.y+1));
    _connect.push_back(Vec2(_coord.x, _coord.y-1));
    
    setTexture("res/node.png");
    setScale(0.75, 0.75);
    setTag(TYPE::NODE);
    setOpacity(_entity);
    
    _back->setTexture("res/node.png");
    _back->setPosition(getPosition());
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
    
    setOpacity(_entity);
    _back->setPosition(getPosition());
    
    setSpriteByRotate();
}

void Pipe::setSpriteByRotate()
{
    _connect.clear();
    
    if( _pipeType == 0 )
    {
        setTexture("res/pipe0.png");
        _back->setTexture("res/pipe0.png");
        
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
        _back->setTexture("res/pipe1.png");
        
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
        _back->setTexture("res/pipe2.png");
        
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
        _back->setTexture("res/pipe3.png");
        
        _connect.push_back(Vec2(_coord.x-1, _coord.y));
        _connect.push_back(Vec2(_coord.x, _coord.y+1));
        _connect.push_back(Vec2(_coord.x+1, _coord.y));
        _connect.push_back(Vec2(_coord.x, _coord.y-1));
    }
    
    setRotation(90*_rotate);
    _back->setRotation(90*_rotate);
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
    
    _ground = Sprite::create("res/r_ground.png");
    _ground->setPosition(getPosition());
    _ground->setScale(0.75, 0.75);
}

void RotatablePipe::rotatePipe()
{
    ++_rotate %= 4;
    setSpriteByRotate();
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

void SwitchPipe::initSPipe( const int &rotate1, const int &rotate2 )
{
    _current = 0;
    _switch[0] = 0, _switch[1] = 1;
    _swtrot[0] = rotate1, _swtrot[1] = rotate2;
    
    initPipe(_switch[0], _swtrot[0]);
    
    _other = Sprite::create("res/pipe1.png");
    _other->setOpacity(128);
    _other->setPosition(getPosition());
    _other->setScale(0.75, 0.75);
    _other->setRotation(90*_swtrot[1]);
    
    _ground = Sprite::create("res/s_ground.png");
    _ground->setPosition(getPosition());
    _ground->setScale(0.75, 0.75);
}

void SwitchPipe::switchPipe()
{
    _connect.clear();
    _current = ++_current%2;
    
    if( _current )
    {
        if( _swtrot[_current] == 0 )
        {
            _connect.push_back(Vec2(_coord.x-1, _coord.y));
            _connect.push_back(Vec2(_coord.x, _coord.y+1));
        }
        else if( _swtrot[_current] == 1 )
        {
            _connect.push_back(Vec2(_coord.x+1, _coord.y));
            _connect.push_back(Vec2(_coord.x, _coord.y+1));
        }
        else if( _swtrot[_current] == 2 )
        {
            _connect.push_back(Vec2(_coord.x+1, _coord.y));
            _connect.push_back(Vec2(_coord.x, _coord.y-1));
        }
        else if( _swtrot[_current] == 3 )
        {
            _connect.push_back(Vec2(_coord.x-1, _coord.y));
            _connect.push_back(Vec2(_coord.x, _coord.y-1));
        }
        
        setTexture("res/pipe1.png");
        _back->setTexture("res/pipe1.png");
        _other->setTexture("res/pipe0.png");
        
        setRotation(90*_swtrot[1]);
        _back->setRotation(90*_swtrot[1]);
        _other->setRotation(90*_swtrot[0]);
    }
    else
    {
        if( _swtrot[_current] % 2 == 0 )
        {
            _connect.push_back(Vec2(_coord.x+1, _coord.y));
            _connect.push_back(Vec2(_coord.x-1, _coord.y));
        }
        else
        {
            _connect.push_back(Vec2(_coord.x, _coord.y+1));
            _connect.push_back(Vec2(_coord.x, _coord.y-1));
        }
        
        setTexture("res/pipe0.png");
        _back->setTexture("res/pipe0.png");
        _other->setTexture("res/pipe1.png");
        
        setRotation(90*_swtrot[0]);
        _back->setRotation(90*_swtrot[0]);
        _other->setRotation(90*_swtrot[1]);
    }
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

void TunnelPipe::initTPipe( const Vec2& end, const int& type, const int& rotate )
{
    _end = end;
    _type = type;
    initPipe(0, rotate);
    
    setTexture("res/t_pipe.png");
    _back->setTexture("res/t_pipe.png");
    
    _connect.clear();
    _connect.push_back(_end);
    
    if( _rotate == 0 )      _connect.push_back(Vec2(_coord.x-1, _coord.y));
    else if( _rotate == 1 ) _connect.push_back(Vec2(_coord.x, _coord.y+1));
    else if( _rotate == 2 ) _connect.push_back(Vec2(_coord.x+1, _coord.y));
    else if( _rotate == 3 ) _connect.push_back(Vec2(_coord.x, _coord.y-1));
    
    string path = "res/t_tunnel" + int_to_string(_type) + ".png";
    _tunnel = Sprite::create(path);

    _tunnel->setPosition(getPosition());
    _tunnel->setRotation(90*_rotate);
    _tunnel->setScale(0.75, 0.75);
}