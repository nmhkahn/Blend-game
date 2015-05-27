#ifndef _LOAD_SCENE_H_
#define _LOAD_SCENE_H_

#include "cocos2d.h"
USING_NS_CC;

#include "cocosGUI.h"
using namespace cocos2d::ui;

class LoadScene : public Layer
{
public:
    static Scene* createScene( const int& level );
    static LoadScene* create( const int& level );
    
    bool init( const int& level );
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void hideIndicator();
    void transitScene();
    
private:
    Size _size;
    Vec2 _origin;
    Vec2 _center;
    
    Sprite* _background;
    Text* _indicator;
    
    int _level;
};

#endif