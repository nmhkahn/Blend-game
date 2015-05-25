#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
USING_NS_CC;

#include "cocosGUI.h"
using namespace cocos2d::ui;

#include "Materials.h"

class GameScene : public Layer
{
public:
    /* implement in GameScene.cpp */
    static Scene* createScene( const int& level );
    static GameScene* create( const int& level );
    
    virtual bool init( const int& level );
    virtual void onEnter();
    virtual void onExit();
    
    void initLevel();
    void parseJSON();
    
    void stageOver();
    void stageClear();
    void changeScene();
    
    /* implement in GameScene_1.cpp */
    void findAdj( Grid* grid, int& numAdjacent );
    void flowAdj( Grid* grid, const int& numAdjacent );
    void flow( ColorNode* start );
    
    void drawFlow( Node* sender, Grid* grid );
    void drawColorNode( Node* sender, Grid* grid );
    void clearToEmpty( Node* sender, Grid* grid );
    void flowAfter( ColorNode* start );
    
    void checkWinLose();
    
    void updateText();
    
private:
    Size _size;
    Vec2 _origin;
    Vec2 _center;
    
    // container for sprite
    Sprite* _background;
    Vector<Grid*> _grids;
    
    // store adjacent grid
    Vector<Grid*> _adjacent;
    // store route to draw pipe
    Vector<Grid*> _route;
    
    Vector<Text*> _textList;
    
    // check win condition
    int _winLoseCnd;
    // contain current level
    int _level;
    // contain # of color in current level
    int _numColor;
};

#endif