#include <iostream>
using namespace std;

#include "GameScene.h"
#include "LoadScene.h"
#include "Util.h"
#include "rapidjson/document.h"

Scene* GameScene::createScene( size_t level )
{
    auto scene = Scene::create();
    auto layer = GameScene::create(level);
    
    scene->addChild(layer);
    return scene;
}

GameScene* GameScene::create( size_t level )
{
    GameScene *ret = new (std::nothrow) GameScene();
    if (ret && ret->init(level))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool GameScene::init( size_t level )
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    _size   = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();
    _center = _origin + _size / 2;
    
    _background = Sprite::create("res/background.png");
    _background->setPosition(_center);
    this->addChild(_background);
    
    _level = level;
    _clearCond = 0;
    initLevel();
    
    schedule(CC_SCHEDULE_SELECTOR(GameScene::update));
    
    return true;
}
 
void GameScene::onEnter()
{
    Node::onEnter();
    
    auto listner = EventListenerTouchOneByOne::create();
    
    listner->setSwallowTouches(true);
    
    listner->onTouchBegan = [&]( Touch* const touch, Event* const event ) -> bool
    {
        return true;
    };
    
    listner->onTouchMoved = [&]( Touch* const touch, Event* const event )
    {
    };
    
    listner->onTouchEnded = [&]( Touch* const touch, Event* const event )
    {
        Vec2 loc = touch->getLocation();
        
        // touch color node
        for( auto iter : _grids )
        {
            if( (*iter).getTag() == Type::COLOR_NODE )
            {
                auto node = static_cast<ColorNode*>(iter);
            
                if( node->_color == Color3B::WHITE ) continue;
            
                auto rect = node->getBoundingBox();
                if( rect.containsPoint(loc) )
                {
                    flow(node);
                    draw(node);
                }
            }
            else if( (*iter).getTag() == Type::ROTATABLE_PIPE )
            {
                auto rect = (*iter).getBoundingBox();
                if( rect.containsPoint(loc) )
                {
                    rotatePipe(static_cast<Pipe*>(iter));
                }
            }
        }
    };
    
    listner->onTouchCancelled = [&]( Touch* const touch, Event* const event )
    {
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listner, 1);
}

void GameScene::onExit()
{
    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
    
    Node::onExit();
}

void GameScene::initLevel()
{
    string level = to_string(_level);
    string path  = "level/level"+level+".json";
        
    auto futil = FileUtils::getInstance();
    auto str = (futil->getStringFromFile(path)).data();
    
    rapidjson::Document document;
    document.Parse<0>(str);
        
    const rapidjson::Value& nodes = document["Nodes"];
    const rapidjson::Value& pipes = document["Pipes"];
    
    for( int i = 0; i < pipes.Size(); i++ )
    {
        const rapidjson::Value& v = pipes[i];
        
        auto pipe = Pipe::create(v["gridX"].GetInt(), v["gridY"].GetInt());
        addChild(pipe);
        pipe->initPipe(v["type"].GetInt(), v["pipe"].GetInt(), v["rotate"].GetInt());
        _grids.pushBack(pipe);
    }
    
    for( int i = 0; i < nodes.Size(); i++ )
    {
        const rapidjson::Value& v = nodes[i];
        
        auto node = ColorNode::create(v["gridX"].GetInt(), v["gridY"].GetInt());
        addChild(node);
        node->initColorNode(v["color"].GetInt(), v["entity"].GetInt());
        _grids.pushBack(node);
    }
    
    // text for help to see entity
    for( auto it : _grids )
    {
        if( (*it).getTag() != Type::COLOR_NODE ) continue;
        
        auto node = static_cast<ColorNode*>(it);
        
        auto text =  Text::create();
        text->setPosition(node->getPosition());
        text->setString(to_string(node->_entity));
        text->setFontSize(30);
        
        _textList.pushBack(text);
        addChild(text);
    }
}

void GameScene::stageOver()
{
    cout << "STAGE OVER" << endl << endl;
    changeScene();
}

void GameScene::stageClear()
{
    cout << "STAGE CLEAR" << endl << endl;
    if( _level < max_stage ) _level++;
    changeScene();
}
                 
void GameScene::changeScene()
{
    auto seq = Sequence::create(DelayTime::create(0.5f),
                                CallFunc::create([&]()
                                {
                                    Director::getInstance()->replaceScene(LoadScene::createScene(_level));
                                }), nullptr);
    runAction(seq);
}

void GameScene::findAdjacent( Grid* grid, int& numAdjacent )
{
    // find adjacent grid of pop_back one's
    // for-all grids
    for( auto it : _grids )
    {
        // for-all connected-grid of start's
        for( auto it2 : grid->_connect )
        {
            // if target is not visited &&
            // is connect to start -> target
            if( !(*it)._visit &&
               (*it)._gridPos == it2 )
            {
                // for-all connected-grid of target's
                for( auto it3 : (*it)._connect )
                {
                    // if connect to target -> start
                    if( grid->_gridPos == it3 )
                    {
                        (*it)._visit = true;
                        _queue.pushBack(&(*it));
                        _route.pushBack(&(*it));
                        numAdjacent++;
                    }
                }
            }
        }
    }
}

void GameScene::flowAdjacent( Grid* grid, const int& numAdjacent )
{
    // variable for next flow
    int flowEntity;
    Color3B flowColor;
    
    // set color for next flow
    if( grid->getTag() == Type::COLOR_NODE )
    {
        auto node = static_cast<ColorNode*>(grid);
        
        flowEntity = node->_entity / numAdjacent;
        flowColor = node->_color;
        
        //node->_entity = 0;
        //node->_color = Color3B::WHITE;
    }
    else
    {
        auto pipe = static_cast<Pipe*>(grid);
        
        flowEntity = pipe->_carry_entity / numAdjacent;
        flowColor = pipe->_carry_color;
        
        //pipe->_carry_entity = 0;
        //pipe->_carry_color = Color3B::WHITE;
    }
    
    for( int i = 0; i < numAdjacent; i++ )
    {
        // set color of lastest grid
        auto grid = _queue.at(_queue.size()-1-i);
        
        if( grid->getTag() == Type::COLOR_NODE )
        {
            auto node = static_cast<ColorNode*>(grid);
            
            if( node->_color == Color3B::WHITE )
            {
                node->_entity = flowEntity;
                node->_color = flowColor;
            }
            // check lose condition
            // 1. blend with other color
            else if( node->_color != flowColor )
            {
                cout << "blend with other color" << endl;
                _clearCond = lose_blend;
            }
            else
            {
                node->_entity += flowEntity;
            }
        }
        else
        {
            auto pipe = static_cast<Pipe*>(grid);
            
            pipe->_carry_entity = flowEntity;
            pipe->_carry_color = flowColor;
        }
    }
}

void GameScene::flow( ColorNode* start )
{
    start->_visit = true;
    _queue.pushBack(start);
    _route.pushBack(start);
    
    // path finding : BFS
    while( !_queue.empty() )
    {
        int numAdjacent = 0;
        auto grid = _queue.front();
        
        // same as pop_back
        _queue.erase(_queue.begin());
        
        // if poped-one is node (not start node)
        // then stop flow
        if( grid->_gridPos != start->_gridPos &&
            grid->getTag() == Type::COLOR_NODE )
        {
            continue;
        }
        
        findAdjacent(grid, numAdjacent);
        
        // check lose condition
        // 1. no connected grid
        if( !numAdjacent &&
            grid->getTag() != Type::COLOR_NODE )
        {
            cout << "no connected" << endl;
            _clearCond = lose_noconnect;
            return;
        }
        // if no adjacent grid, continue
        else if( !numAdjacent ) continue;
        
        // flow to adjacent grid
        flowAdjacent(grid, numAdjacent);
        
        //updateColor();
    }
}

void GameScene::drawAction( Node* sender, Grid* grid )
{
    int opacity;
    Color3B color;
    
    if( grid->getTag() == Type::COLOR_NODE )
    {
        auto node = static_cast<ColorNode*>(grid);
        
        opacity = node->_entity;
        color = node->_color;
    }
    else
    {
        auto pipe = static_cast<Pipe*>(grid);
        
        opacity = pipe->_carry_entity;
        color = pipe->_carry_color;
    }
    
    grid->setOpacity(opacity);
    grid->setColor(color);
}

void GameScene::after( Node* sender, Grid* grid )
{
    if( grid->getTag() != Type::COLOR_NODE )
    {
        auto pipe = static_cast<Pipe*>(grid);
        
        pipe->_carry_entity = 255;
        pipe->_carry_color = Color3B::WHITE;
        
        pipe->setOpacity(pipe->_carry_entity);
        pipe->setColor(pipe->_carry_color);
    }
}

void GameScene::draw( ColorNode* start )
{
    Vector<FiniteTimeAction*> vfta;
    
    vfta.pushBack(CallFuncN::create(CC_CALLBACK_1(GameScene::drawAction, this, _route.front())));
    vfta.pushBack(DelayTime::create(0.1));
    vfta.pushBack(CallFunc::create([&]()
                  {
                      start->_color = Color3B::WHITE;
                      start->_entity = 255;
                      
                      start->setOpacity(start->_entity);
                      start->setColor(start->_color);
                  }));
    
    for( auto it : _route )
    {
        vfta.pushBack(CallFuncN::create(CC_CALLBACK_1(GameScene::drawAction, this, &(*it))));
        vfta.pushBack(DelayTime::create(0.1));
        vfta.pushBack(CallFuncN::create(CC_CALLBACK_1(GameScene::after, this, &(*it))));
    }
    
    vfta.pushBack(CallFunc::create(CC_CALLBACK_0(GameScene::updateColor, this)));
    vfta.pushBack(CallFunc::create(CC_CALLBACK_0(GameScene::updateText, this)));
    vfta.pushBack(CallFunc::create(CC_CALLBACK_0(GameScene::clearGrid, this)));
    vfta.pushBack(CallFunc::create([&]()
                  {
                      // check win/lose condition
                      // 1. win  : all node's entity reach 255
                      // 2. lose : total entity belows 250
                      bool isFull = true;
                      int totalEntity = 0;
                      
                      for( auto it : _grids )
                      {
                          if( (*it).getTag() != Type::COLOR_NODE ) continue;
                          auto node = static_cast<ColorNode*>(it);
                          
                          if( node->_entity < 250 )
                          {
                              isFull = false;
                          }
                          else if( node->_color != Color3B::WHITE )
                          {
                              totalEntity += node->_entity;
                          }
                      }
                      
                      if( isFull )
                      {
                          cout << "all node is full" << endl;
                          _clearCond = win;
                      }
                      if( totalEntity < 250 )
                      {
                          cout << "total is < 250" << endl;
                          _clearCond = lose_total;
                      }
                      
                      switch( _clearCond )
                      {
                          case lose_blend:
                              stageOver();
                              break;
                          case lose_noconnect:
                              stageOver();
                              break;
                          case lose_total:
                              stageOver();
                              break;
                          case win:
                              stageClear();
                              break;
                      }
                  }));
    
    auto seq = Sequence::create(vfta);
    runAction(seq);
}

void GameScene::updateColor()
{
    for( auto it : _grids )
    {
        if( (*it).getTag() != Type::COLOR_NODE ) continue;
        auto node = static_cast<ColorNode*>(it);
        
        if( node->_entity > 250 ||
            node->_color == Color3B::WHITE )
        {
            node->_entity = 255;
        }
        
        if( node->_entity == 0 )
        {
            node->_entity = 255;
            node->_color = Color3B::WHITE;
        }
        
        node->setColor(node->_color);
        
        auto fto = FadeTo::create(10, node->_entity);
        node->runAction(fto);
    }
}

void GameScene::updateText()
{
    for( auto it : _grids )
    {
        if( (*it).getTag() != Type::COLOR_NODE ) continue;
        auto node = static_cast<ColorNode*>(it);
        
        string entity = to_string(node->_entity);
        
        for( auto it2 : _textList )
        {
            if( (*it2).getPosition() == node->getPosition() )
            {
                (*it2).setString(entity);
            }
        }
    }
}

void GameScene::clearGrid()
{
    for( auto it : _grids )
    {
        // reset visit
        (*it)._visit = false;
        
        // clear carry in pipe
        if( (*it).getTag() == Type::COLOR_NODE )
        {
        }
        else
        {
            auto pipe = static_cast<Pipe*>(it);
            
            pipe->_carry_entity = 0;
            pipe->_carry_color = Color3B::WHITE;
            
            pipe->setOpacity(255);
            pipe->setColor(pipe->_carry_color);
        }
    }
    
    _route.clear();
}

void GameScene::rotatePipe( Pipe* pipe )
{
    ++pipe->_rotate %= 4;
    pipe->initPipe(pipe->_type, pipe->_pipe, pipe->_rotate);
}