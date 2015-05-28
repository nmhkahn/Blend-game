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
    
    bool init( const int& level );
    virtual void onEnter() override;
    virtual void onExit() override;
    
    virtual void update( float dt ) override;
    
    void initLevel();
    void parseJSON();
    
    void stageOver();
    void stageClear();
    void changeScene();
    
    /* implement in GameScene_1.cpp */
    void findAdj( Grid* curr, int& numAdjacent );
    void flowAdj( Grid* curr, const int& numAdjacent );
    void flow( ColorNode* start );
    
    void drawFlow( Node* sender, Grid* curr );
    void drawColorNode( Node* sender, Grid* curr );
    void clearToEmpty( Node* sender, Grid* curr );
    void flowAfter( ColorNode* start );
    
    void checkWinLose();
    
    void updateText();
    
private:
    Size _size;
    Vec2 _origin;
    Vec2 _center;
    
    Sprite* _background;     // background sprite
    Vector<Grid*> _grids;    // contain entire grid
    
    Vector<Grid*> _adjacent; // store adjacent grid
    Vector<Grid*> _route;    // store route to draw pipe
    
    Vector<Text*> _textList; // test : show entity
    
    int _winLoseCnd;         // check win condition
    int _level;              // contain current level
    int _numColor;           // contain # of color in current level
};

#endif