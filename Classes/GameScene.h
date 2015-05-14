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
    
    static Scene* createScene( size_t level );
    static GameScene* create( size_t level );
    
    virtual bool init( size_t level );
    virtual void onEnter();
    virtual void onExit();
    virtual void update( float delta );
    
    void initLevel();
    void stageOver();
    void stageClear();
    void changeScene();
    
    void flow( ColorNode* start );
    
    void updateColor();
    void updateText();
    void clearGrid();
    
    void rotatePipe( Pipe* pipe );
    
private:
    Vec2 _origin;
    Size _size;
    Vec2 _center;
    
    // container for sprite
    Sprite* _background;
    Vector<Grid*> _grids;
    
    // ui
    Vector<Text*> _textList;
    Text* _stageText;
    
    Vector<Grid*> _finds;
    
    size_t _level;
};

#endif