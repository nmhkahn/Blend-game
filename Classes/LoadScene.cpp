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
    
    initUI();
    
    return true;
}

void LoadScene::onEnter()
{
    Node::onEnter();
    
    auto listner = EventListenerTouchOneByOne::create();
    listner->setSwallowTouches(true);
    
    listner->onTouchBegan = [&]( Touch* const touch, Event* const event ) -> bool
    {
        return true;
    };
    
    listner->onTouchMoved = [&]( Touch* const touch, Event* const event )
    {
    };
    
    listner->onTouchEnded = [&]( Touch* const touch, Event* const event )
    {
        Vec2 loc = touch->getLocation();
        
        // when touched buttons
        if( _prev != nullptr &&
           _prev->getBoundingBox().containsPoint(loc) )
        {
            if( _level > 1 ) _level--;
            changeScene();
        }
        
        if( _next != nullptr &&
           _next->getBoundingBox().containsPoint(loc) )
        {
            if( _level < max_level ) _level++;
            changeScene();
        }
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listner, 1);
    
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
    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
    
    Node::onExit();
}

void LoadScene::initUI()
{
    _indicator = Text::create();
    _indicator->setPosition(_center);
    _indicator->setString(int_to_string(_level));
    _indicator->setFontSize(50);
    _indicator->setOpacity(0);
    addChild(_indicator);
    
    auto foa = FadeIn::create(0.5);
    _indicator->runAction(foa);
    
    if( _level < max_level &&
        UserDefault::getInstance()->getIntegerForKey("level") > _level )
    {
        _prev = Sprite::create("res/arrow.png");
        _prev->setPosition(50, _size.height-50);
        _next = Sprite::create("res/arrow.png");
        _next->setRotation(180);
        _next->setPosition(_size.width-50, _size.height-50);
        
        addChild(_prev);
        addChild(_next);
    }
    else
    {
        _prev = Sprite::create("res/arrow.png");
        _prev->setPosition(50, _size.height-50);
        _next = nullptr;
        
        addChild(_prev);
    }
}

void LoadScene::hideIndicator()
{
    auto foa = FadeOut::create(0.5);
    _indicator->runAction(foa);
}

void LoadScene::transitScene()
{
    Director::getInstance()->replaceScene(GameScene::createScene(_level));
}

void LoadScene::changeScene()
{
    if( _level == 1 ) Director::getInstance()->replaceScene(GameScene::createScene(_level));
    else Director::getInstance()->replaceScene(LoadScene::createScene(_level));
}