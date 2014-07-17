#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // Ground setup
    Sprite *groundSprite = Sprite::create("ground.png");
    this->addChild(groundSprite);
    groundSprite->setPosition(Vec2(308, 84)); //half of image size
    
    // SkyGround setup
    Sprite *skySprite = Sprite::create("flappy_background.png");
    this->addChild(skySprite);
    skySprite->setPosition(visibleSize.width/2, 168 + 200);
    
    // bird setup
    Sprite *birdSprite = Sprite::create("flappybird1.png");
    this->addChild(birdSprite);
    birdSprite->setPosition(visibleSize.width/2, visibleSize.height/2 + 120);
    return true;
}

