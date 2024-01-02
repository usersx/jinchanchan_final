#include"PlayerBase.h"


//const char* Player::HEALTH_CHANGED_EVENT = "HealthChangedEvent";
//const char* Player::PLAYER_ID_KEY = "PlayerID"; // 键名，用于存储玩家ID的键


Player* Player::create(const std::string& filename)
{
    
    Player* player = new (std::nothrow) Player();
    
    if (player && player->init(filename))
    {
        player->autorelease();

        return player;
    }
    else
    {
        CC_SAFE_DELETE(player);

        return nullptr;
    }
}

bool Player::init(const std::string& filename)
{
    if (!Sprite::initWithSpriteFrameName(filename)) // 使用你的plist中定义的帧名
    {
        
        return false;
    }
    //BaseSprite::init();
    //血条初始化
    HealthBar* healthBar = HealthBar::create("Display2.png", "Display1.png", 100.0f, 20.0f);
    
    //血条定位
    this->setHealthBar(healthBar);

    //角色数据初始化
    initData();
    
    return true;
}

void Player::setHealthBar(HealthBar* healthBar) {
    if (healthBar) {
        this->healthBar = healthBar;
        this->healthBar->setPosition(Vec2(0, this->getContentSize().height));
        this->addChild(healthBar);
        this->healthBar->retain();
    }
}

void Player::player_push_back_herolist(int key) {
    switch (key) {
    case 1:
        //hero = AkaliHero::create();//英雄放进heroList
        heroList.pushBack(AkaliHero::create());
        break;
    case 2:
        //hero = AnnieHero::create();
        heroList.pushBack(AnnieHero::create());
        break;
    case 3:
        //hero = AriHero::create();
        heroList.pushBack(AriHero::create());
        break;
    case 4:
        //hero = CatalinaHero::create();
        heroList.pushBack(CatalinaHero::create());
        break;
    case 5:
        //hero = EvelynnHero::create();
        heroList.pushBack(EvelynnHero::create());
        break;
    case 6:
        //hero = EzrealHero::create();
        heroList.pushBack(EzrealHero::create());
        break;
    case 7:
        //hero = GalenHero::create();
        heroList.pushBack(GalenHero::create());
        break;
    case 8:
        //hero = GiggsHero::create();
        heroList.pushBack(GiggsHero::create());
        break;
    case 9:
        //hero = JinxHero::create();
        heroList.pushBack(JinxHero::create());
        break;
    case 10:
        //hero = KaiSaHero::create();
        heroList.pushBack(KaiSaHero::create());
        break;
    case 11:
        //hero = KennenHero::create();
        heroList.pushBack(KennenHero::create());
        break;
    case 12:
        //hero = KSanteHero::create();
        heroList.pushBack(KSanteHero::create());
        break;
    case 13:
        //hero = LucianHero::create();
        heroList.pushBack(LucianHero::create());
        break;
    }
}

void Player::decreaseBlood(int decreaseNum)
{
    int lastblood = getBlood();
    setBlood(lastblood - decreaseNum);
    healthBar->setPercentage(getBlood());//更新血条显示
    //if (getBlood() != lastblood) {

    //    auto customEvent = new cocos2d::EventCustom(Player::HEALTH_CHANGED_EVENT);

    //    PlayerHealthEventData* eventData = new PlayerHealthEventData();
    //    eventData->playerId = playerId;
    //    eventData->health = blood;
    //    customEvent->setUserData(eventData);

    //    // 发送血量变化事件
    //    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(customEvent);

    //}
}

void Player::initData() {

    setBlood(100);

    setMoveRate(100.0f);

    this->setScale(1.0f);

    this->setPosition(Vec2(150.0f, 150.0f));

    setIsFacingRight(true);

    //setIsMoving(false);

    setIsDead(false);

    setIsEnemy(false);

    setIsRoundLose(false);

    setMoney(20);

    setLevel(0);

    setIsAI(false);

    setIsInaRound(false);

    setIsInBattle(false);

    CCLOG("Player Data Init Successful!");
}//变量初始化


//移动函数
bool Player::onTouchBegan(Touch* touch, Event* event) {

    //auto mousePos = convertToNodeSpace(Vec2(event->getCursorX(), event->getCursorY()));
    //上面这种写法错误
    CCLOG("listen mouse click!");
    if (this->getIsEnemy() || this->getIsAI()) {
        return false;  // 返回 false 表示不处理触摸事件
    }
    
    //Vec2 pt = Vec2(event->getCursorX(), event->getCursorY());
    
    Vec2 touchPos = touch->getLocation();
    //Vec2 mousePos = Director::getInstance()->convertToGL(event->getLocationInView());
    Vec2 worldTouchPos = Director::getInstance()->convertToGL(touchPos);

    //CCLOG("(%f,%f)-(%f,%f)", touchPos.x, touchPos.y,this->getPosition().x, this->getPosition().y);

    float time = getTimetoDest(touchPos);

    needFlip(touchPos.x);

    playWalkAnimationAndMove(touchPos, time);
    
    
}

float Player::getTimetoDest(Vec2 pt)
{
    float deltax = this->getPosition().x - pt.x;

    float deltay = this->getPosition().y - pt.y;

    float time = static_cast<float>(sqrt(pow(deltax, 2) + pow(deltay, 2))) / getMoveRate();

    return time;
}

Animation* Player::getAnimation()
{
    Vector<SpriteFrame*> walkFrames;

    char frameName[100];

    for (int i = 1; i <= 4; ++i)
    {
        sprintf(frameName, "PlayerWalk%d.png", i);

        SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);

        walkFrames.pushBack(frame);
    }
    auto walkAnimation = Animation::createWithSpriteFrames(walkFrames, 0.2f);

    return walkAnimation;

}

void Player::playWalkAnimationAndMove(const Vec2& destination, float duration)
{
    
    // 停止之前的动画和移动
    this->stopAllActions();
    //setIsMoving(true);
    // 播放行走动画
    this->runAction(RepeatForever::create(Animate::create(getAnimation())));

    // 创建移动动作
    auto moveAction = MoveTo::create(duration, destination);

    // 通过 Spawn 同时执行两个动作
    auto spawnAction = Spawn::create(moveAction, nullptr);
    // 通过 RepeatForever 使其循环执行
    auto repeatAction = RepeatForever::create(spawnAction);
    // 创建一个回调动作，在移动完成后执行
    auto callback = CallFunc::create([this]() {
    // 在回调中停止行走动画
    this->stopAllActions();
    //setIsMoving(false);
        });

    // 通过 Sequence 将移动和回调动作串联起来
    auto sequence = Sequence::create(moveAction, callback, nullptr);

    // 同时执行行走动画和移动动作
    this->runAction(RepeatForever::create(sequence));
    

    // 同时执行行走动画和移动动作
    //this->runAction(repeatAction);
    
}

void Player::needFlip(float mousePosX)
{
    if (mousePosX < this->getBoundingBox().getMidX())
    {
        if (getIsFacingRight())
            this->flipDirection();// 鼠标点击在Player左边，需要翻转朝向
    }
    else {
        if (!getIsFacingRight())
            this->flipDirection();
    }

}

void Player::flipDirection()
{
    facingRight = !facingRight; // 切换朝向

    this->setFlippedX(!facingRight); // 根据朝向设置是否水平翻转
}

void Player::moneychange()
{
    if (this->getIsRoundLose() == true) {
        setMoney(getMoney() + 10);
    }
    else {
        setMoney(getMoney() + 20);
    }
    //updateMoneydisplay();//这里要补，更改显示
}