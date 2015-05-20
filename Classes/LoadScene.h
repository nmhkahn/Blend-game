#ifndef _LOAD_SCENE_H_
#define _LOAD_SCENE_H_

#include "cocos2d.h"
USING_NS_CC;

#include "cocosGUI.h"
using namespace cocos2d::ui;

class LoadScene : public Layer
{
public:
    static Scene* createScene( size_t level );
    static LoadScene* create( size_t level );
    
    virtual bool init( size_t level );
    virtual void onEnter();
    virtual void onExit();
    
    void hideIndicator();
    void transitScene();
    
private:
    Size _size;
    Vec2 _origin;
    Vec2 _center;
    
    Sprite* _background;
    Text* _level_indicator;
    
    size_t _level;
};

#endif