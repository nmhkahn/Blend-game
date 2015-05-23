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
    
    void flow( ColorNode* start );
    void findAdjacent( Grid* grid, int& numAdjacent );
    void flowAdjacent( Grid* grid, const int& numAdjacent );
    
    void draw( ColorNode* start );
    void drawAction( Node* sender, Grid* grid );
    void after( Node* sender, Grid* grid );
    
    void updateColor();
    void updateText();
    void clearGrid();
        
private:
    Size _size;
    Vec2 _origin;
    Vec2 _center;
    
    // container for sprite
    Sprite* _background;
    Vector<Grid*> _grids;
    
    // queue for store adjacent grid
    Vector<Grid*> _queue;
    // queue for store route to draw pipe
    Vector<Grid*> _route;
    
    // check win condition
    int _clearCond;
        
    int _level;
};

#endif