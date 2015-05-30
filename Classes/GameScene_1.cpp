//
//  Implement Flow
//

#include <iostream>
using namespace std;

#include "GameScene.h"
#include "Util.h"

void GameScene::findAdj( Grid* curr, int& numAdjacent )
{
    // find adjacent grid of pop_back one's
    // for-all grids
    for( auto it : _grids )
    {
        // if two nodes are adjacent, ignore them
        if( curr->getTag() == TYPE::NODE &&
            it->getTag() == TYPE::NODE ) continue;
        
        // for-all connected-grid of start's
        for( auto it2 : curr->_connect )
        {
            // if target is not visited &&
            // is connect to start -> target
            if( it != curr->_before &&
                it->_coord == it2 )
            {
                // for-all connected-grid of target's
                for( auto it3 : it->_connect )
                {
                    // if connect to target -> start
                    if( curr->_coord == it3 )
                    {
                        it->_before = curr;
                        _adjacent.pushBack(it);
                        _route.pushBack(it);
                        numAdjacent++;
                    }
                }
            }
        }
    }
}

void GameScene::flowAdj( Grid* curr, const int& numAdjacent )
{
    // variable for next flow
    int flowEntity;
    Color3B flowColor;
    
    // set color for next flow
    flowEntity = curr->_entity / numAdjacent;
    flowColor = curr->_color;
    
    for( int i = 0; i < numAdjacent; i++ )
    {
        // set color of lastest grid
        auto near = _adjacent.at(_adjacent.size()-1-i);
        
        if( near->_color == Color3B::WHITE )
        {
            near->_color = flowColor;
        }
        
        // check lose condition
        // 1. blend with other color
        if( near->_color != flowColor )
        {
            _winLoseCnd = COND::L_BLEND;
        }
        else
        {
            near->_entity += flowEntity;
        }
    }
}

void GameScene::flow( ColorNode* start )
{
    _adjacent.pushBack(start);
    
    // path finding : BFS
    while( !_adjacent.empty() )
    {
        int numAdjacent = 0;
        auto curr = _adjacent.front();
        
        // same as pop_front
        _adjacent.erase(_adjacent.begin());
        
        // if poped-one is node (not start node)
        // then stop flow
        if( curr->_coord != start->_coord &&
           curr->getTag() == TYPE::NODE ) continue;
        
        findAdj(curr, numAdjacent);
        
        // check lose condition
        // 1. no connected grid
        if( !numAdjacent &&
            curr->getTag() != TYPE::NODE )
        {
            _winLoseCnd = COND::L_NOCONN;
            return;
        }
        // if no adjacent node, continue
        else if( !numAdjacent ) continue;
        
        // flow to adjacent grid
        flowAdj(curr, numAdjacent);
    }
}

void GameScene::drawFlow( Node* sender, Grid* curr )
{
    // 1. set color and set opacity to 0
    // 2. set opacity to grid->_entity by action
    if( curr->getTag() == TYPE::NODE )
    {
        drawColorNode(sender, curr);
    }
    else
    {
        curr->setColor(curr->_color);
        
        auto fto = FadeTo::create(0.4, curr->_entity);
        curr->runAction(fto);
    }
}

void GameScene::drawColorNode( Node* sender, Grid* curr )
{
    // if   entity > 250 && carry something
    // then set entity to 255
    if( curr->_entity > 250 &&
        curr->_color != Color3B::WHITE )
    {
        curr->_entity = 255;
        curr->setColor(curr->_color);
        
        auto fto = FadeTo::create(0.4, curr->_entity);
        curr->runAction(fto);
    }
    // if   entity == 0 (not carry)
    // then set color to white
    else if( curr->_entity == 0 )
    {
        curr->_color = Color3B::WHITE;
        curr->setColor(curr->_color);
        curr->setOpacity(0);
    }
    else
    {
        curr->setColor(curr->_color);
        
        auto fto = FadeTo::create(0.4, curr->_entity);
        curr->runAction(fto);
    }
}

void GameScene::clearToEmpty( Node* sender, Grid* curr )
{
    curr->_entity = 0;
    curr->_color = Color3B::WHITE;
    
    auto fto = FadeTo::create(0.3, curr->_entity);
    curr->runAction(fto);
}

void GameScene::flowAfter( ColorNode* start )
{
    Vector<FiniteTimeAction*> vfta;
    Vector<Grid*> ends;
    
    // popback end node
    // : easy to implement clearToEmpty action
    for( auto it = _route.begin(); it != _route.end(); )
    {
        if( (*it)->getTag() == TYPE::NODE )
        {
            ends.pushBack((*it));
            it = _route.erase(it);
        }
        else
        {
            it++;
        }
    }
    
    // draw pipe carries
    for( auto it : _route )
    {
        vfta.pushBack(CallFuncN::create(CC_CALLBACK_1(GameScene::drawFlow, this, it)));
        vfta.pushBack(DelayTime::create(flow_speed));
    }
    // draw end grid
    for( auto it : ends )
    {
        vfta.pushBack(CallFuncN::create(CC_CALLBACK_1(GameScene::drawFlow, this, it)));
    }
    vfta.pushBack(DelayTime::create(0.2));
    
    // clear to empty start node
    vfta.pushBack(CallFuncN::create(CC_CALLBACK_1(GameScene::clearToEmpty, this, start)));
    // draw route pipe empty
    for( auto it : _route )
    {
        vfta.pushBack(CallFuncN::create(CC_CALLBACK_1(GameScene::clearToEmpty, this, it)));
        vfta.pushBack(DelayTime::create(0.05));
    }
    
    //vfta.pushBack(CallFunc::create(CC_CALLBACK_0(GameScene::updateText, this)));
    
    // clear route for next touch
    vfta.pushBack(CallFunc::create([&]()
                                   {
                                       _route.clear();
                                       for( auto it : _grids )
                                       {
                                           it->_before = nullptr;
                                           //it->_visit = false;
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
        if( it->getTag() != TYPE::NODE ) continue;
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
        if( it->getTag() != TYPE::NODE ) continue;
        auto node = static_cast<ColorNode*>(it);
        
        string entity = int_to_string(node->_entity);
        
        for( auto it2 : _textList )
        {
            if( (*it2).getPosition() == node->getPosition() )
            {
                (*it2).setString(entity);
            }
        }
    }
}