#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0, -50));
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setSpeed(3.0);
    
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
    this->isGameOver = false;
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    // Ground setup
    groundSprite0 = Sprite::create("ground.png");
    this->addChild(groundSprite0);
    groundSprite0->setPosition(Vec2(groundSprite0->getContentSize().width/2.0 , groundSprite0->getContentSize().height/2));
    
    groundSprite1 = Sprite::create("ground.png");
    this->addChild(groundSprite1);
    groundSprite1->setPosition(Vec2(visibleSize.width + groundSprite1->getContentSize().width/2.0 -10, groundSprite1->getContentSize().height/2));
    
    auto groundbody0 = PhysicsBody::createBox(groundSprite0->getContentSize());
    groundbody0->setDynamic(false);
    groundbody0->setContactTestBitmask(true);
    groundSprite0->setPhysicsBody(groundbody0);
    auto groundbody1 = PhysicsBody::createBox(groundSprite1->getContentSize());
    groundbody1->setDynamic(false);
    groundbody1->setContactTestBitmask(true);
    groundSprite1->setPhysicsBody(groundbody1);
    
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

    auto birdBody = PhysicsBody::createCircle(17.0);
    birdBody->setDynamic(true);
    birdBody->setMass(1.0f);
    birdBody->setVelocity(Vec2(4.0f, 2.0f));
    birdBody->setVelocityLimit(50);
    birdBody->setContactTestBitmask(true);
    birdSprite->setPhysicsBody(birdBody);
    
    //pipe setup
    topPipeSprite = Sprite::create("top_pipe.png");
    bottomPipeSprite = Sprite::create("bottom_pipe.png");
    topPipeSprite->setPosition(visibleSize.width + topPipeSprite->getContentSize().width/2, 600);
    
    auto pipebody0 = PhysicsBody::createBox(topPipeSprite->getContentSize());
    pipebody0->setDynamic(false);
    topPipeSprite->setPhysicsBody(pipebody0);
    pipebody0->setContactTestBitmask(true);
    auto pipebody1 = PhysicsBody::createBox(bottomPipeSprite->getContentSize());
    pipebody1->setDynamic(false);
    pipebody1->setContactTestBitmask(true);
    bottomPipeSprite->setPhysicsBody(pipebody1);
    
    this->positionBottomPipe();
    this->addChild(topPipeSprite);
    this->addChild(bottomPipeSprite);
    
    //setup touch listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [=](Touch *touch, Event *event){
        if (!this->isGameOver) {
            birdBody->applyImpulse(Vec2(0, 90.0f));
        }
        log("touch detected!");
        return true;
    };
    
    //setup collision listener
    auto plistener = EventListenerPhysicsContact::create();
    plistener->onContactBegin = [=](PhysicsContact &contact){
        log("collision detected!");
        auto gameOverSprite = Sprite::create("game_over.png");
        gameOverSprite->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
        this->addChild(gameOverSprite);
        this->isGameOver = true;
        
        auto restartListner = EventListenerTouchOneByOne::create();
        restartListner->setSwallowTouches(true);
        restartListner->onTouchBegan = [](Touch *touch, Event* event){
            Director::getInstance()->replaceScene(HelloWorld::createScene());
            return true;
        };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(restartListner, gameOverSprite);
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(plistener, this);
    this->scheduleUpdate();
    
    return true;
}

void HelloWorld::update(float delata) {
    Vec2 curPos0 = groundSprite0->getPosition();
    Vec2 curPos1 = groundSprite1->getPosition();
    Vec2 pipePos = topPipeSprite->getPosition();
    if (curPos0.x < -groundSprite0->getContentSize().width/2) {
        groundSprite0->setPosition(Vec2(curPos1.x + groundSprite0->getContentSize().width, curPos0.y));
        return;
    }
    if (curPos1.x <= -groundSprite1->getContentSize().width/2) {
        groundSprite1->setPosition(Vec2(curPos0.x + groundSprite1->getContentSize().width, curPos1.y));
        return;
    }
    if (pipePos.x < -topPipeSprite->getContentSize().width / 2) {
        topPipeSprite->setPosition(Vec2(visibleSize.width + topPipeSprite->getContentSize().width/2, 480 + arc4random() % 300));
        return;
    }
    if (!this->isGameOver) {
        groundSprite0->setPosition(Vec2(curPos0.x - 2, curPos0.y));
        groundSprite1->setPosition(Vec2(curPos1.x - 2, curPos1.y));
        topPipeSprite->setPosition(Vec2(pipePos.x - 2, pipePos.y));
        this->positionBottomPipe();
    }
}

void HelloWorld::positionBottomPipe(){
    bottomPipeSprite->setPosition(Vec2(topPipeSprite->getPosition().x, topPipeSprite->getPosition().y - 660));
}
