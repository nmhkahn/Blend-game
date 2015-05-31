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
            _level--;
            changeScene();
        }
        
        if( _next != nullptr &&
           _next->getBoundingBox().containsPoint(loc) )
        {
            _level++;
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
    addChild(_indicator);
    
    if( _level == 1 )
    {
        _prev = nullptr;
        _next = Sprite::create("res/node.png");
        _next->setPosition(_size.width-30, _size.height-30);
        
        addChild(_next);
    }
    else if( _level == max_level )
    {
        _prev = Sprite::create("res/node.png");
        _prev->setPosition(30, _size.height-30);
        _next = nullptr;
        
        addChild(_prev);
    }
    else
    {
        _prev = Sprite::create("res/node.png");
        _prev->setPosition(30, _size.height-30);
        _next = Sprite::create("res/node.png");
        _next->setPosition(_size.width-30, _size.height-30);
        
        addChild(_prev);
        addChild(_next);
    }
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

void LoadScene::changeScene()
{
    Director:: getInstance()->replaceScene(LoadScene::createScene(_level));
}