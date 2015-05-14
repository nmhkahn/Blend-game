#include <iostream>
using namespace std;

#include "GameScene.h"
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
    initLevel();
    
    schedule(CC_SCHEDULE_SELECTOR(GameScene::update));
    
    return true;
}
 
void GameScene::onEnter()
{
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
}

void GameScene::update( float delta )
{
    // visualize?
}

void GameScene::initLevel()
{
    string level = to_string(_level);
    string path  = "level/level"+level+".json";
    
    //cout << "path : " << path << endl;
    
    auto futil = FileUtils::getInstance();
    auto str = (futil->getStringFromFile(path)).data();
    //cout << "start" << endl << str << endl << "end" << endl;
    
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
    
    
    // for test
    _stageText = Text::create();
    _stageText->setPosition(_center+Vec2(0, 300));
    _stageText->setString("stage "+to_string(_level));
    _stageText->setFontSize(30);
    addChild(_stageText);
}

void GameScene::stageOver()
{
    cout << "STAGE OVER" << endl << endl;
    _stageText->setString("stage over");
    
    for( auto it : _grids )
    {
        (*it).setVisible(false);
    }
    
    Director::getInstance()->replaceScene(GameScene::createScene(_level));
}

void GameScene::stageClear()
{
    cout << "STAGE CLEAR" << endl << endl;
    _stageText->setString("stage clear");
    
    for( auto it : _grids )
    {
        (*it).setVisible(false);
    }
    
    //if( _level < max_stage ) _level++;
    //else _level = 0;
    
    Director::getInstance()->replaceScene(GameScene::createScene(_level));
}

void GameScene::flow( ColorNode* start )
{
    // queue for store adjacent grid
    Vector<Grid*> queue;
    
    start->_visit = true;
    queue.pushBack(start);
    
    while( !queue.empty() )
    {
        int numAdjacent = 0;
        auto grid = queue.front();
        
        // pop_back
        queue.erase(queue.begin());
        
        // find adjacent grid of pop_back one's
        for( auto it : _grids )
        {
            for( auto it2 : grid->_connect )
            {
                if( !(*it)._visit &&
                     (*it)._gridX == it2.first &&
                     (*it)._gridY == it2.second )
                {
                    for( auto it3 : (*it)._connect )
                    {
                        //cout << "x:" << (*it)._gridX << " y:" << (*it)._gridY << endl;
                        //cout << "connect L " << it3.first << it3.second << endl << endl;
                        if( grid->_gridX == it3.first &&
                            grid->_gridY == it3.second )
                        {
                            (*it)._visit = true;
                            queue.pushBack(&(*it));
                            numAdjacent++;
                        }
                    }
                }
            }
        }
        
        // check lose condition
        if( !numAdjacent &&
            grid->getTag() != Type::COLOR_NODE )
        {
            cout << "no connected" << endl;
            stageOver();
            return;
        }
        // if no adjacent grid, continue
        else if( !numAdjacent ) continue;
        
        // variable for next flow
        int flowEntity;
        Color3B flowColor;
        
        // set color for next flow
        if( grid->getTag() == Type::COLOR_NODE )
        {
            auto node = static_cast<ColorNode*>(grid);
            
            flowEntity = node->_entity / numAdjacent;
            flowColor = node->_color;
            
            node->_entity = 0;
            node->_color = Color3B::WHITE;
        }
        else
        {
            auto pipe = static_cast<Pipe*>(grid);
            
            flowEntity = pipe->_carry_entity / numAdjacent;
            flowColor = pipe->_carry_color;
            
            pipe->_carry_entity = 0;
            pipe->_carry_color = Color3B::WHITE;
        }
        
        // flow to adjacent grid
        for( int i = 0; i < numAdjacent; i++ )
        {
            // set color of lastest grid
            auto grid = queue.at(queue.size()-1-i);
            
            if( grid->getTag() == Type::COLOR_NODE )
            {
                auto node = static_cast<ColorNode*>(grid);
                
                if( node->_color == Color3B::WHITE )
                {
                    node->_entity = flowEntity;
                    node->_color = flowColor;
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
        
        updateColor();
        updateText();
    }
    
    clearGrid();
    
    // check win/lose condition
    int totalEntity = 0;
    
    for( auto it : _grids )
    {
        if( (*it).getTag() != Type::COLOR_NODE ) continue;
        auto node = static_cast<ColorNode*>(it);
        
        if( node->_color != Color3B::WHITE &&
            node->_entity == 255 )
        {
            stageClear();
        }
        else if( node->_color != Color3B::WHITE )
        {
            totalEntity += node->_entity;
        }
    }
    if( totalEntity < 250 )
    {
        cout << "total is < 250" << endl;
        stageOver();
    }
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
        node->setOpacity(node->_entity);
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
        if( (*it).getTag() == Type::COLOR_NODE ) continue;
        auto pipe = static_cast<Pipe*>(it);
        
        pipe->_carry_color = Color3B::WHITE;
        pipe->_carry_entity = 0;
    }
}

void GameScene::rotatePipe( Pipe* pipe )
{
    pipe->_connect.clear();
    pipe->initPipe(pipe->_type, pipe->_type, ++pipe->_rotate);
}