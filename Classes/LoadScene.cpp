#include "LoadScene.h"
#include "GameScene.h"

#include "Util.h"

Scene* LoadScene::createScene( const int& level )
{
    auto scene = Scene::create();
    auto layer = LoadScene::create(level);
    
    scene->addChild(layer);
    return scene;
}

LoadScene* LoadScene::create( const int& level )
{
    LoadScene *ret = new (std::nothrow) LoadScene();
    if (ret && ret->init(level))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool LoadScene::init( const int& level )
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    _size   = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();
    _center = _origin + _size / 2;
    
    _level = level;
    
    _background = Sprite::create("res/background.png");
    _background->setPosition(_center);
    this->addChild(_background);
    
    _indicator = Text::create();
    _indicator->setPosition(_center);
    _indicator->setString(int_to_string(_level));
    _indicator->setFontSize(50);
    this->addChild(_indicator);
    
    return true;
}

void LoadScene::onEnter()
{
    Node::onEnter();
    
    auto dta = DelayTime::create(1.0);
    auto seq = Sequence::create(dta,
                                CallFunc::create(CC_CALLBACK_0(LoadScene::hideIndicator, this)),
                                dta,
                                CallFunc::create(CC_CALLBACK_0(LoadScene::transitScene, this)),
                                NULL);
    runAction(seq);
}

void LoadScene::onExit()
{
    Node::onExit();
}

void LoadScene::hideIndicator()
{
    auto foa = FadeOut::create(1.0);
    _indicator->runAction(foa);
}

void LoadScene::transitScene()
{
    Director:: getInstance()->replaceScene(GameScene::createScene(_level));
}