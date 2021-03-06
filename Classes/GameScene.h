#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
USING_NS_CC;

#include "ui/CocosGUI.h"
using namespace cocos2d::ui;

#include "Materials.h"

class GameScene : public Layer
{
public:
    /* implement in GameScene.cpp */
    static Scene* createScene( const int& level );
    static GameScene* create( const int& level );
    
    bool init( const int& level );
    virtual void onEnter() override;
    virtual void onExit() override;
    
    virtual void update( float dt ) override;
    
    void initLevel();
    void initUI();
    void parseJSON();
    
    void stageOver();
    void stageClear();
    void changeScene();
    
    /* implement in GameScene_1.cpp */
    void connectToOther( Grid *from, Grid *to, int& numAdjacent );
    void findAdj( Grid* curr, int& numAdjacent );
    void flowAdj( Grid* curr, const int& numAdjacent );
    void flow( ColorNode* start );
    
    void rollbackFlow( Grid* start, Grid* curr );
    void drawFlow( Node* sender, Grid* curr );
    void drawColorNode( Node* sender, Grid* curr );
    void clearToEmpty( Node* sender, Grid* curr );
    void flowAfter( ColorNode* start );
    
    void checkWinLose();
        
private:
    Size _size;
    Vec2 _origin;
    Vec2 _center;
    
    Sprite* _background;     // background sprite
    Vector<Grid*> _grids;    // contain entire grid
    
    Vector<Grid*> _adjacent; // store adjacent grid
    Vector<Grid*> _route;    // store route to draw pipe
        
    Sprite* _prev, *_next;   // next, prev button
    
    int _winLoseCnd;         // check win condition
    int _level;              // contain current level
    int _numColor;           // contain # of color in current level
    
    bool _touchEnable;
};

#endif