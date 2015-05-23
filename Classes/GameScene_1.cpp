//
//  Implement Game Logic and 
//

#include <iostream>
using namespace std;

#include "GameScene.h"
#include "Util.h"

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
               (*it)._coord == it2 )
            {
                // for-all connected-grid of target's
                for( auto it3 : (*it)._connect )
                {
                    // if connect to target -> start
                    if( grid->_coord == it3 )
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
    if( grid->getTag() == TYPE::NODE )
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
        
        flowEntity = pipe->_entity / numAdjacent;
        flowColor = pipe->_color;
        
        //pipe->_entity = 0;
        //pipe->_color = Color3B::WHITE;
    }
    
    for( int i = 0; i < numAdjacent; i++ )
    {
        // set color of lastest grid
        auto grid = _queue.at(_queue.size()-1-i);
        
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
            
            pipe->_entity = flowEntity;
            pipe->_color = flowColor;
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
        if( grid->_coord != start->_coord &&
           grid->getTag() == TYPE::NODE )
        {
            continue;
        }
        
        findAdjacent(grid, numAdjacent);
        
        // check lose condition
        // 1. no connected grid
        if( !numAdjacent &&
           grid->getTag() != TYPE::NODE )
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
    
    if( grid->getTag() == TYPE::NODE )
    {
        auto node = static_cast<ColorNode*>(grid);
        
        opacity = node->_entity;
        color = node->_color;
    }
    else
    {
        auto pipe = static_cast<Pipe*>(grid);
        
        opacity = pipe->_entity;
        color = pipe->_color;
    }
    
    grid->setOpacity(opacity);
    grid->setColor(color);
}

void GameScene::after( Node* sender, Grid* grid )
{
    if( grid->getTag() != TYPE::NODE )
    {
        auto pipe = static_cast<Pipe*>(grid);
        
        pipe->_entity = 255;
        pipe->_color = Color3B::WHITE;
        
        pipe->setOpacity(pipe->_entity);
        pipe->setColor(pipe->_color);
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
                                           if( (*it).getTag() != TYPE::NODE ) continue;
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
        if( (*it).getTag() != TYPE::NODE ) continue;
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
        
        auto fto = FadeTo::create(0.1, node->_entity);
        node->runAction(fto);
    }
}

void GameScene::updateText()
{
    /*
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
    */
}

void GameScene::clearGrid()
{
    for( auto it : _grids )
    {
        // reset visit
        (*it)._visit = false;
        
        // clear carry in pipe
        if( (*it).getTag() == TYPE::NODE )
        {
        }
        else
        {
            auto pipe = static_cast<Pipe*>(it);
            
            pipe->_entity = 0;
            pipe->_color = Color3B::WHITE;
            
            pipe->setOpacity(255);
            pipe->setColor(pipe->_color);
        }
    }
    
    _route.clear();
}