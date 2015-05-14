#include <vector>
#include <iostream>
using namespace std;

#include "Materials.h"
#include "Util.h"

bool Grid::init()
{
    _visit = false;
    
    return initWithTexture(nullptr, Rect::ZERO );;
}

ColorNode* ColorNode::create( const int& gridX,
                              const int& gridY )
{
    ColorNode* node = new (nothrow) ColorNode();
    if( node && node->init() )
    {
        node->autorelease();
        node->_gridX = gridX;
        node->_gridY = gridY;
    }
    else
    {
        CC_SAFE_DELETE(node);
    }

    node->setPosition(Vec2(90*gridX + 100, 90*gridY + 80));
    
    return node;
}

void ColorNode::initColorNode( const int& color, const int& entity )
{
    _entity = entity;
    
    _connect.push_back(pair<int, int>(_gridX+1, _gridY));
    _connect.push_back(pair<int, int>(_gridX-1, _gridY));
    _connect.push_back(pair<int, int>(_gridX, _gridY+1));
    _connect.push_back(pair<int, int>(_gridX, _gridY-1));
    
    setTexture("res/node.png");
    setScale(0.75, 0.75);
    setTag(Type::COLOR_NODE);
    
    if( color == Color::RED) _color = Color3B::RED, setColor(Color3B::RED);
    if( color == Color::BLUE) _color = Color3B::BLUE, setColor(Color3B::BLUE);
    if( color == Color::GREEN) _color = Color3B::GREEN, setColor(Color3B::GREEN);
    if( color == Color::YELLOW) _color = Color3B::YELLOW, setColor(Color3B::YELLOW);
    if( color == Color::MAGENTA) _color = Color3B::MAGENTA, setColor(Color3B::MAGENTA);
    if( color == Color::ORANGE) _color = Color3B::ORANGE, setColor(Color3B::ORANGE);
    
    setOpacity(_entity);
}

Pipe* Pipe::create( const int& gridX,
                    const int& gridY )
{
    Pipe* pipe = new (nothrow) Pipe();
    if( pipe && pipe->init() )
    {
        pipe->autorelease();
        pipe->_gridX = gridX;
        pipe->_gridY = gridY;
    }
    else
    {
        CC_SAFE_DELETE(pipe);
    }
    
    pipe->setPosition(Vec2(90*gridX + 100, 90*gridY + 80));
    
    return pipe;
}

void Pipe::initPipe( const int& type, const int& pipe, const int& rotate )
{
    _type = type;
    _pipe = pipe;
    _rotate = rotate;
    _carry_color = Color3B::WHITE;
    _carry_entity = 0;
    
    _connect.clear();

    if( _pipe == 0 )
    {
        if( _type == Type::ROTATABLE_PIPE ) setTag(Type::ROTATABLE_PIPE), setTexture("res/pipe0_1.png");
        else setTag(Type::PIPE), setTexture("res/pipe0.png");
        
        if( _rotate % 2 == 0 )
        {
            _connect.push_back(pair<int, int>(_gridX+1, _gridY));
            _connect.push_back(pair<int, int>(_gridX-1, _gridY));
        }
        else
        {
            _connect.push_back(pair<int, int>(_gridX, _gridY+1));
            _connect.push_back(pair<int, int>(_gridX, _gridY-1));
        }
    }
    else if( _pipe == 1 )
    {
        if( _type == Type::ROTATABLE_PIPE ) setTag(Type::ROTATABLE_PIPE), setTexture("res/pipe1_1.png");
        else setTag(Type::PIPE), setTexture("res/pipe1.png");
        
        if( _rotate == 0 )
        {
            _connect.push_back(pair<int, int>(_gridX-1, _gridY));
            _connect.push_back(pair<int, int>(_gridX, _gridY+1));
        }
        else if( _rotate == 1 )
        {
            _connect.push_back(pair<int, int>(_gridX+1, _gridY));
            _connect.push_back(pair<int, int>(_gridX, _gridY+1));
        }
        else if( _rotate == 2 )
        {
            _connect.push_back(pair<int, int>(_gridX+1, _gridY));
            _connect.push_back(pair<int, int>(_gridX, _gridY-1));
        }
        else if( _rotate == 3 )
        {
            _connect.push_back(pair<int, int>(_gridX-1, _gridY));
            _connect.push_back(pair<int, int>(_gridX, _gridY-1));
        }
    }
    else if( _pipe == 2 )
    {
        if( _type == Type::ROTATABLE_PIPE ) setTag(Type::ROTATABLE_PIPE), setTexture("res/pipe2_1.png");
        else setTag(Type::PIPE), setTexture("res/pipe2.png");
        
        if( _rotate == 0 )
        {
            _connect.push_back(pair<int, int>(_gridX-1, _gridY));
            _connect.push_back(pair<int, int>(_gridX+1, _gridY));
            _connect.push_back(pair<int, int>(_gridX, _gridY+1));
        }
        else if( _rotate == 1 )
        {
            _connect.push_back(pair<int, int>(_gridX+1, _gridY));
            _connect.push_back(pair<int, int>(_gridX, _gridY-1));
            _connect.push_back(pair<int, int>(_gridX, _gridY+1));
        }
        else if( _rotate == 2 )
        {
            _connect.push_back(pair<int, int>(_gridX-1, _gridY));
            _connect.push_back(pair<int, int>(_gridX+1, _gridY));
            _connect.push_back(pair<int, int>(_gridX, _gridY-1));
        }
        else if( _rotate == 3 )
        {
            _connect.push_back(pair<int, int>(_gridX-1, _gridY));
            _connect.push_back(pair<int, int>(_gridX, _gridY-1));
            _connect.push_back(pair<int, int>(_gridX, _gridY+1));
        }
        
    }
    else if( _pipe == 3 )
    {
        setTag(Type::PIPE);
        setTexture("res/pipe3.png");
        _connect.push_back(pair<int, int>(_gridX-1, _gridY));
        _connect.push_back(pair<int, int>(_gridX, _gridY+1));
        _connect.push_back(pair<int, int>(_gridX+1, _gridY));
        _connect.push_back(pair<int, int>(_gridX, _gridY-1));
    }
    
    setScale(0.75, 0.75);
    
    setRotation(90*_rotate);
}