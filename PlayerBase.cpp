#include"PlayerBase.h"


//const char* Player::HEALTH_CHANGED_EVENT = "HealthChangedEvent";
//const char* Player::PLAYER_ID_KEY = "PlayerID"; // ���������ڴ洢���ID�ļ�


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
    if (!Sprite::initWithSpriteFrameName(filename)) // ʹ�����plist�ж����֡��
    {
        
        return false;
    }
    //BaseSprite::init();
    //Ѫ����ʼ��
    HealthBar* healthBar = HealthBar::create("Display2.png", "Display1.png", 100.0f, 20.0f);
    
    //Ѫ����λ
    this->setHealthBar(healthBar);

    //��ɫ���ݳ�ʼ��
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
        //hero = AkaliHero::create();//Ӣ�۷Ž�heroList
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
    healthBar->setPercentage(getBlood());//����Ѫ����ʾ
    //if (getBlood() != lastblood) {

    //    auto customEvent = new cocos2d::EventCustom(Player::HEALTH_CHANGED_EVENT);

    //    PlayerHealthEventData* eventData = new PlayerHealthEventData();
    //    eventData->playerId = playerId;
    //    eventData->health = blood;
    //    customEvent->setUserData(eventData);

    //    // ����Ѫ���仯�¼�
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
}//������ʼ��


//�ƶ�����
bool Player::onTouchBegan(Touch* touch, Event* event) {

    //auto mousePos = convertToNodeSpace(Vec2(event->getCursorX(), event->getCursorY()));
    //��������д������
    CCLOG("listen mouse click!");
    if (this->getIsEnemy() || this->getIsAI()) {
        return false;  // ���� false ��ʾ���������¼�
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
    
    // ֹ֮ͣǰ�Ķ������ƶ�
    this->stopAllActions();
    //setIsMoving(true);
    // �������߶���
    this->runAction(RepeatForever::create(Animate::create(getAnimation())));

    // �����ƶ�����
    auto moveAction = MoveTo::create(duration, destination);

    // ͨ�� Spawn ͬʱִ����������
    auto spawnAction = Spawn::create(moveAction, nullptr);
    // ͨ�� RepeatForever ʹ��ѭ��ִ��
    auto repeatAction = RepeatForever::create(spawnAction);
    // ����һ���ص����������ƶ���ɺ�ִ��
    auto callback = CallFunc::create([this]() {
    // �ڻص���ֹͣ���߶���
    this->stopAllActions();
    //setIsMoving(false);
        });

    // ͨ�� Sequence ���ƶ��ͻص�������������
    auto sequence = Sequence::create(moveAction, callback, nullptr);

    // ͬʱִ�����߶������ƶ�����
    this->runAction(RepeatForever::create(sequence));
    

    // ͬʱִ�����߶������ƶ�����
    //this->runAction(repeatAction);
    
}

void Player::needFlip(float mousePosX)
{
    if (mousePosX < this->getBoundingBox().getMidX())
    {
        if (getIsFacingRight())
            this->flipDirection();// �������Player��ߣ���Ҫ��ת����
    }
    else {
        if (!getIsFacingRight())
            this->flipDirection();
    }

}

void Player::flipDirection()
{
    facingRight = !facingRight; // �л�����

    this->setFlippedX(!facingRight); // ���ݳ��������Ƿ�ˮƽ��ת
}

void Player::moneychange()
{
    if (this->getIsRoundLose() == true) {
        setMoney(getMoney() + 10);
    }
    else {
        setMoney(getMoney() + 20);
    }
    //updateMoneydisplay();//����Ҫ����������ʾ
}