//
//  Implement Parsing and Event Handler
//

#include <iostream>
using namespace std;

#include "GameScene.h"
#include "LoadScene.h"
#include "Util.h"
#include "rapidjson/document.h"

Scene* GameScene::createScene( const int& level )
{
    auto scene = Scene::create();
    auto layer = GameScene::create(level);
    
    scene->addChild(layer);
    return scene;
}

GameScene* GameScene::create( const int& level )
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

bool GameScene::init( const int& level )
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
    this->addChild(_background, 0);
    
    _level = level;
    _winLoseCnd = COND::NONE;
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
        
        // when touched buttons
        if( _prev != nullptr &&
            _prev->getBoundingBox().containsPoint(loc) )
        {
            _level--;
            changeScene();
        }
        
        if( _next != nullptr &&
            _next->getBoundingBox().containsPoint(loc) )
        {
            _level++;
            changeScene();
        }
        
        // when touched grids
        for( auto it : _grids )
        {
            switch( it->getTag() )
            {
                case TYPE::NODE:
                {
                    auto node = static_cast<ColorNode*>(it);
                    if( node->_color == Color3B::WHITE ) continue;
                    
                    auto rect = node->getBoundingBox();
                    if( rect.containsPoint(loc) )
                    {
                        flow(node);
                        flowAfter(node);
                    }
                    break;
                }
                case TYPE::R_PIPE:
                {
                    auto rect = it->getBoundingBox();
                    if( rect.containsPoint(loc) )
                    {
                        auto pipe = static_cast<RotatablePipe*>(it);
                        pipe->rotatePipe();
                    }
                    break;
                }
                case TYPE::S_PIPE:
                {
                    auto rect = it->getBoundingBox();
                    if( rect.containsPoint(loc) )
                    {
                        auto pipe = static_cast<SwitchPipe*>(it);
                        pipe->switchPipe();
                    }
                    break;
                }
            }
        }
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
    initUI();
    parseJSON();
}

void GameScene::initUI()
{
    if( _level <= 1 )
    {
        _level = 1;
        _prev = nullptr;
        _next = Sprite::create("res/node.png");
        _next->setPosition(_size.width-30, _size.height-30);
        
        addChild(_next);
    }
    else if( _level >= max_level )
    {
        _level = max_level;
        _prev = Sprite::create("res/node.png");
        _prev->setPosition(30, _size.height-30);
        _next = nullptr;
        
        addChild(_prev);
    }
    else
    {
        _prev = Sprite::create("res/node.png");
        _prev->setPosition(30, _size.height-30);
        _next = Sprite::create("res/node.png");
        _next->setPosition(_size.width-30, _size.height-30);
        
        addChild(_prev);
        addChild(_next);
    }
}

void GameScene::parseJSON()
{
    string level = int_to_string(_level);
    string path  = "level/level"+level+".json";
        
    auto futil = FileUtils::getInstance();
    auto str = (futil->getStringFromFile(path));
    
    rapidjson::Document document;
    document.Parse<0>(str.data());
    
    const rapidjson::Value& info   = document["INFO"];
    const rapidjson::Value& nodes  = document["NODE"];
    const rapidjson::Value& npipes = document["N_PIPE"];
    const rapidjson::Value& rpipes = document["R_PIPE"];
    const rapidjson::Value& spipes = document["S_PIPE"];
    const rapidjson::Value& tpipes = document["T_PIPE"];
    
    _numColor = info["numColor"].GetInt();
    
    for( int i = 0; i < nodes.Size(); i++ )
    {
        const rapidjson::Value& v = nodes[i];
        
        auto node = ColorNode::create(Vec2(v["gridX"].GetInt(), v["gridY"].GetInt()));
        node->initColorNode(v["color"].GetInt(), v["entity"].GetInt());
        
        addChild(node, 10);
        addChild(node->_back, 1);
        _grids.pushBack(node);
    }
    
    for( int i = 0; i < npipes.Size(); i++ )
    {
        const rapidjson::Value& v = npipes[i];
        
        auto pipe = Pipe::create(Vec2(v["gridX"].GetInt(), v["gridY"].GetInt()));
        pipe->initPipe(v["pipe"].GetInt(), v["rotate"].GetInt());
        
        addChild(pipe, 10);
        addChild(pipe->_back, 1);
        _grids.pushBack(pipe);
    }
        
    for( int i = 0; i < rpipes.Size(); i++ )
    {
        const rapidjson::Value& v = rpipes[i];
        
        auto pipe = RotatablePipe::create(Vec2(v["gridX"].GetInt(), v["gridY"].GetInt()));
        pipe->initRPipe(v["pipe"].GetInt(), v["rotate"].GetInt());
        
        addChild(pipe, 10);
        addChild(pipe->_back, 1);
        addChild(pipe->_ground, 5);
        _grids.pushBack(pipe);
    }
    
    for( int i = 0; i < spipes.Size(); i++ )
    {
        const rapidjson::Value& v = spipes[i];
        
        auto pipe = SwitchPipe::create(Vec2(v["gridX"].GetInt(), v["gridY"].GetInt()));
        pipe->initSPipe(v["rot1"].GetInt(), v["rot2"].GetInt());
        
        addChild(pipe, 10);
        addChild(pipe->_back, 1);
        addChild(pipe->_ground, 5);
        _grids.pushBack(pipe);
    }
    
    for( int i = 0; i < tpipes.Size(); i++ )
    {
        const rapidjson::Value& v = tpipes[i];
        
        auto pipe = TunnelPipe::create(Vec2(v["gridX"].GetInt(), v["gridY"].GetInt()));
        pipe->initTPipe(Vec2(v["endX"].GetInt(), v["endY"].GetInt()),
                        v["type"].GetInt(), v["rotate"].GetInt());
        
        addChild(pipe, 10);
        addChild(pipe->_back, 1);
        addChild(pipe->_tunnel, 20);
        _grids.pushBack(pipe);
    }
}

void GameScene::stageOver()
{
    changeScene();
}

void GameScene::stageClear()
{
    if( _level < max_level ) _level++;
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

void GameScene::update( float dt )
{
}