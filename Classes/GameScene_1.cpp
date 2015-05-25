//
//  Implement Flow
//

#include <iostream>
using namespace std;

#include "GameScene.h"
#include "Util.h"

void GameScene::findAdj( Grid* grid, int& numAdjacent )
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
               (*it)._coord == it2 )
            {
                // for-all connected-grid of target's
                for( auto it3 : (*it)._connect )
                {
                    // if connect to target -> start
                    if( grid->_coord == it3 )
                    {
                        (*it)._visit = true;
                        _adjacent.pushBack(&(*it));
                        _route.pushBack(&(*it));
                        numAdjacent++;
                    }
                }
            }
        }
    }
}

void GameScene::flowAdj( Grid* grid, const int& numAdjacent )
{
    // variable for next flow
    int flowEntity;
    Color3B flowColor;
    
    // set color for next flow
    if( grid->getTag() == TYPE::NODE )
    {
        auto node = static_cast<ColorNode*>(grid);
        
        flowEntity = node->_entity / numAdjacent;
        flowColor = node->_color;
    }
    else
    {
        auto pipe = static_cast<Pipe*>(grid);
        
        flowEntity = pipe->_entity / numAdjacent;
        flowColor = pipe->_color;
    }
    
    for( int i = 0; i < numAdjacent; i++ )
    {
        // set color of lastest grid
        auto grid = _adjacent.at(_adjacent.size()-1-i);
        
        if( grid->getTag() == TYPE::NODE )
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
                _winLoseCnd = COND::L_BLEND;
            }
            else
            {
                node->_entity += flowEntity;
            }
        }
        else
        {
            auto pipe = static_cast<Pipe*>(grid);
            
            pipe->_entity = flowEntity;
            pipe->_color = flowColor;
        }
    }
}

void GameScene::flow( ColorNode* start )
{
    start->_visit = true;
    _adjacent.pushBack(start);
    _route.pushBack(start);
    
    // path finding : BFS
    while( !_adjacent.empty() )
    {
        int numAdjacent = 0;
        auto grid = _adjacent.front();
        
        // same as pop_back
        _adjacent.erase(_adjacent.begin());
        
        // if poped-one is node (not start node)
        // then stop flow
        if( grid->_coord != start->_coord &&
            grid->getTag() == TYPE::NODE )
        {
            continue;
        }
        
        findAdj(grid, numAdjacent);
        
        // check lose condition
        // 1. no connected grid
        if( !numAdjacent &&
           grid->getTag() != TYPE::NODE )
        {
            _winLoseCnd = COND::L_NOCONN;
            return;
        }
        // if no adjacent grid, continue
        else if( !numAdjacent ) continue;
        
        // flow to adjacent grid
        flowAdj(grid, numAdjacent);
    }
}

void GameScene::drawFlow( Node* sender, Grid* grid )
{
    if( grid->_color == Color3B::WHITE )
    {
        grid->setOpacity(255);
    }
    else
    {
        grid->setOpacity(grid->_entity);
    }
    grid->setColor(grid->_color);
}

void GameScene::drawColorNode( Node* sender, Grid* grid )
{
    // if   entity > 250 && carry something
    // then set entity to 255
    if( grid->_entity > 250 &&
       grid->_color != Color3B::WHITE )
    {
        grid->_entity = 255;
        grid->setColor(grid->_color);
        
        auto fto = FadeTo::create(0.1, grid->_entity);
        grid->runAction(fto);
    }
    
    // if   entity == 0 (not carry)
    // then set color to white
    if( grid->_entity == 0 )
    {
        grid->_color = Color3B::WHITE;
        grid->setColor(grid->_color);
        grid->setOpacity(255);
    }
}

void GameScene::clearToEmpty( Node* sender, Grid* grid )
{
    grid->_entity = 0;
    grid->_color = Color3B::WHITE;
    
    grid->setOpacity(255);
    grid->setColor(grid->_color);
}

void GameScene::flowAfter( ColorNode* start )
{
    Vector<FiniteTimeAction*> vfta;
    
    // pop target node
    // : only left pipe in _route
    auto target = _route.back();
    _route.popBack();
    
    // set pipe carries
    for( auto it : _route )
    {
        vfta.pushBack(CallFuncN::create(CC_CALLBACK_1(GameScene::drawFlow, this, &(*it))));
        vfta.pushBack(DelayTime::create(flow_speed));
    }
    
    // draw target node
    vfta.pushBack(CallFuncN::create(CC_CALLBACK_1(GameScene::drawColorNode, this, target)));
    
    vfta.pushBack(CallFunc::create(CC_CALLBACK_0(GameScene::updateText, this)));
    
    // clear to empty start node
    vfta.pushBack(CallFuncN::create(CC_CALLBACK_1(GameScene::clearToEmpty, this, start)));
    
    // set route pipe empty
    for( auto it : _route )
    {
        vfta.pushBack(CallFuncN::create(CC_CALLBACK_1(GameScene::clearToEmpty, this, &(*it))));
        vfta.pushBack(DelayTime::create(flow_speed));
    }
    
    // clear route for next touch
    vfta.pushBack(CallFunc::create([&]()
                                   {
                                       _route.clear();
                                       for( auto it : _grids )
                                       {
                                           (*it)._visit = false;
                                       }
                                   }));
    // check win/lose state
    vfta.pushBack(CallFunc::create(CC_CALLBACK_0(GameScene::checkWinLose, this)));
    
    auto seq = Sequence::create(vfta);
    runAction(seq);
}

void GameScene::checkWinLose()
{
    // check win/lose condition
    // 1. win  : all node's entity reach 255
    // 2. lose : total entity belows 250
    bool isFull = true;
    int totalEntity = 0;
    
    for( auto it : _grids )
    {
        if( (*it).getTag() != TYPE::NODE ) continue;
        auto node = static_cast<ColorNode*>(it);
        
        if( node->_color != Color3B::WHITE )
        {
            totalEntity += node->_entity;
            if( node->_entity < 250 )
            {
                isFull = false;
            }
        }
    }
    
    if( isFull )
    {
        _winLoseCnd = COND::WIN;
    }
    if( totalEntity < 250 )
    {
        _winLoseCnd = COND::L_TOTAL;
    }
    
    // check win/lose condition and jump
    switch( _winLoseCnd )
    {
        case COND::L_BLEND:
        {
            CCLOG("L_BLEND");
            stageOver();
            break;
        }
        case COND::L_NOCONN:
        {
            CCLOG("L_NOCONN");
            stageOver();
            break;
        }
        case COND::L_TOTAL:
        {
            CCLOG("L_TOTAL");
            stageOver();
            break;
        }
        case COND::WIN:
        {
            CCLOG("WIN");
            stageClear();
            break;
        }
    }
}

void GameScene::updateText()
{
    for( auto it : _grids )
    {
        if( (*it).getTag() != TYPE::NODE ) continue;
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