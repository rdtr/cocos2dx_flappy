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
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    // Ground setup
    groundSprite0 = Sprite::create("ground.png");
    this->addChild(groundSprite0);
    groundSprite0->setPosition(Vec2(groundSprite0->getContentSize().width/2.0 , groundSprite0->getContentSize().height/2));
    
    groundSprite1 = Sprite::create("ground.png");
    this->addChild(groundSprite1);
    groundSprite1->setPosition(Vec2(visibleSize.width + groundSprite1->getContentSize().width/2.0 -10, groundSprite1->getContentSize().height/2));
    
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
    
    //pipe setup
    topPipeSprite = Sprite::create("top_pipe.png");
    bottomPipeSprite = Sprite::create("bottom_pipe.png");
    topPipeSprite->setPosition(visibleSize.width/2, visibleSize.height - 50);

    
    
    this->scheduleUpdate();
    
    return true;
}

void HelloWorld::update(float delata) {
    Vec2 curPos0 = groundSprite0->getPosition();
    Vec2 curPos1 = groundSprite1->getPosition();
    if (curPos0.x < -groundSprite0->getContentSize().width/2) {
        groundSprite0->setPosition(Vec2(curPos1.x + groundSprite0->getContentSize().width, curPos0.y));
        return;
    }
    if (curPos1.x <= -groundSprite1->getContentSize().width/2) {
        groundSprite1->setPosition(Vec2(curPos0.x + groundSprite1->getContentSize().width, curPos1.y));
        return;
    }
    groundSprite0->setPosition(Vec2(curPos0.x - 1, curPos0.y));
    groundSprite1->setPosition(Vec2(curPos1.x - 1, curPos1.y));
}

void HelloWorld::positionBottomPipe(){
    //bottomPiPeSprite->setPosition(Vec2(topPipeSprite->getPosition);
}
