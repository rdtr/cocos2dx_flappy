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
    Sprite *skySprite0 = Sprite::create("flappy_background.png");
    Sprite *skySprite1 = Sprite::create("flappy_background.png");
    Sprite *skySprite2 = Sprite::create("flappy_background.png");
    this->addChild(skySprite0);
    this->addChild(skySprite1);
    this->addChild(skySprite2);
    skySprite0->setPosition(visibleSize.width/2, 168 + 200);
    skySprite1->setPosition(visibleSize.width/2 - skySprite1->getContentSize().width, 168 + 200);
    skySprite2->setPosition(visibleSize.width/2 + skySprite1->getContentSize().width, 168 + 200);
    
    // bird setup
    /*
    Sprite *birdSprite = Sprite::create("flappybird1.png");
    this->addChild(birdSprite);
    birdSprite->setPosition(visibleSize.width/2, visibleSize.height/2 + 120);
    */

    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("bird.plist");
    
    auto flyAnim = Animation::create();
    for (int i = 1; i < 4; i++) {
        SpriteFrame * frame = cache->getSpriteFrameByName("flappybird" + to_string(i) + ".png");
        flyAnim->addSpriteFrame(frame);
    }
    auto birdSprite = Sprite::createWithSpriteFrameName("flappybird1.png");

    flyAnim->setDelayPerUnit(0.2f);
    
    auto action = Animate::create(flyAnim);
    auto animation = RepeatForever::create(action);
    birdSprite->runAction(animation);
    
    birdSprite->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 + 80));
    this->addChild(birdSprite);
    
    return true;
}

