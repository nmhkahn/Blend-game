using namespace std;

#include "LoadScene.h"
#include "GameScene.h"

Scene* LoadScene::createScene( size_t level )
{
    auto scene = Scene::create();
    auto layer = LoadScene::create(level);
    
    scene->addChild(layer);
    return scene;
}

LoadScene* LoadScene::create( size_t level )
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

bool LoadScene::init( size_t level )
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
    
    _level_indicator = Text::create();
    _level_indicator->setPosition(_center);
    _level_indicator->setString(to_string(_level));
    _level_indicator->setFontSize(50);
    this->addChild(_level_indicator);
    
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
    _level_indicator->runAction(foa);
}

void LoadScene::transitScene()
{
    Director:: getInstance()->replaceScene(GameScene::createScene(_level));
}