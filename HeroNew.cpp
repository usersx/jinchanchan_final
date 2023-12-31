#include"Hero/HeroNew.h"
#include"Player/PlayerManager.h"
#include<algorithm>

template <typename T>
T clamp(T value, T minValue, T maxValue) {
    return std::max(minValue, std::min(value, maxValue));
}

const Rect targetRect = Rect(180, 150, 664, 464);
Hero::Hero(const std::vector<float>& health, const std::vector<float>& physicalAttack,
    float armor, float magicResist, float critRate, float attackRange,
    int initialMana, int mana, float attackSpeed, Level level)
    : _health(health), _physicalAttack(physicalAttack), _armor(armor), _magicResist(magicResist),
    _critRate(critRate), _attackRange(attackRange), _initialMana(initialMana),
    _mana(mana), _attackSpeed(attackSpeed), _level(level) 
{
    int index = static_cast<int>(_level);
    // ��������ֵ����������
    _currentHealth = _health[index];
    _currentPhysicalAttack = _physicalAttack[index];
    _mana = _initialMana;
    setPosition(_originalPos);
    this->init();
}

Hero::~Hero() {
   ;
}
//Hero::Hero()
//{
//    _level = Level::Level1;
//}

//Hero* Hero::create(const std::string& filename)
//{
//    Hero* hero = new (std::nothrow) Hero();
//
//    if (hero && hero->init(filename))
//    {
//        hero->autorelease();
//
//        return hero;
//    }
//    else
//    {
//        CC_SAFE_DELETE(hero);
//
//        return nullptr;
//    }
//}

bool Hero::init()
{
    
    
    this->setBloodAndMana();//����Ѫ��ͼƬ

    this->setIsDead(false);//��ʼ��ʱ�����

    this->setIsOnTheField(false);//��ʼ��ʱ�����ڳ���

    this->setTeam(0);

    this->setenemyTeam(1);

    //�ж��ǲ���AI��Ӫ�� ����ǵĻ� ����create

    _bar = EquipmentBarforHero::create();
    this->addChild(_bar);

    //setLocalZOrder(2);//�˴�����Ⱦ˳�� ��Ԥ������Ӣ�۵���Ⱦ˳�� �������ӵĽ����ھɽڵ�֮ǰ����Ⱦ ���ܻ��ͼƬ��û
    
    return true;
}
//���������Ӣ��
void Hero::put_on_field(std::pair<int, int> pair_position) {
    //���㳡��λ��
    //Director::getInstance()->getRunningScene()->addChild(this);
    setIsOnTheField(true);
    Vec2 position = Vec2(220+square_width * (pair_position.first - 1), 180+ square_height * (pair_position.second - 1));
    this->setOriginalPos(position);

    const float gridWidth = 74.0f;
    const float gridHeight = 60.0f;
    float scaleX = gridWidth / getContentSize().width;
    float scaleY = gridHeight / getContentSize().height;
    this->setScale(scaleX, scaleY);
    setVisible(true);
}


bool Hero::take_equip(Equipment* equip) {
    //������С ע������ڵ�
   // int number = equip->getEquipNumber();
    //_power += equipment_power[number];
    int result = _bar->auto_equip(equip);
    return result;
}

bool Hero::unload_equip() {
    
    _bar->unload_all();
    return 1;
}


//Hero* Hero::create(const std::string& filename,
//    const std::vector<float>& health, const std::vector<float>& physicalAttack,
//    float armor, float magicResist, float critRate, float attackRange,
//    float initialMana, float mana, float attackSpeed, Level level) 
//{
//    Hero* hero = new (std::nothrow) Hero(health, physicalAttack, armor, magicResist,
//        critRate, attackRange, initialMana, mana, attackSpeed, level);
//    
//    
//    
//    if (hero && hero->initWithFile(filename)) {
//
//        hero->autorelease();
//
//        hero->setBloodAndMana();//����Ѫ��ͼƬ
//
//        hero->setIsDead(false);//��ʼ��ʱ�����
//
//        hero->setIsOnTheField(false);//��ʼ��ʱ�����ڳ���
//
//        return hero;
//    }
//    CC_SAFE_DELETE(hero);
//
//    return nullptr;
//}

void Hero::upgrade() 
{
    if (_level != Level::Level3) {
        _level = static_cast<Level>(static_cast<int>(_level) + 1);
        updateAttributes();
    }
}

void Hero::updateAttributes() {
    int index = static_cast<int>(_level);
    // ��������ֵ����������
    _currentHealth = _health[index];
    _currentPhysicalAttack = _physicalAttack[index];
    _mana = _initialMana;
    if (this->getIsOnTheField()) {
        this->setPosition(_battlePosition);
    }
    else {
        this->setPosition(_originalPos);
    }
    //setPosition(_originalPos);
    this->setIsDead(false);
    this->setOpacity(255);
    this->getBloodBar()->setPercentage(100);
    // �������Ա��ֲ���
}

void Hero::setBloodAndMana()
{
    //Ѫ����ʼ��
    bloodBar = HealthBar::create("Display2.png", "Display1.png", 100.0f, 20.0f);
    //������ʼ��
    manaBar = HealthBar::create("Display4.png", "Display3.png", 100.0f, 20.0f);
    //Ѫ����λ
    if (bloodBar) {
        bloodBar->setPosition(Vec2(0, this->getContentSize().height + 5.0f));
        this->addChild(bloodBar);
        bloodBar->setScale(2.0f);
        this->bloodBar->retain();
    }
    if (manaBar) {
        manaBar->setPosition(Vec2(0, this->getContentSize().height));
        this->addChild(manaBar);
        manaBar->setScale(2.0f);
        this->manaBar->retain();
        manaBar->setPercentage(1.0 * _initialMana / _mana * 100);
    }

}
Hero* Hero::getNextEnemyHero()
{
    auto heroList = PlayerManager::getInstance()->getPlayerByID(getenemyTeam())->getHeroList();
    
    if (!heroList.empty()) {
        Hero* p = nullptr;

        float minDistance = 1000000;

        for (Hero* enemyHero : heroList)
        {
            //�ڳ��ϲ����������
            if (this->getIsOnTheField()==true&& enemyHero->getIsOnTheField()==true && enemyHero->getIsDead() == false) {
                float newDistance = getDistance(enemyHero);

                if (minDistance > newDistance) {

                    minDistance = newDistance;

                    p = enemyHero;
                }
            }
        }
        return p;
    }
    
    return nullptr;

}

void Hero::update(float dt) {

    Hero* target = getNextEnemyHero(); // ��ȡ����ĵз���λ
    if (target) {
        if (isInRange(target)) {
            this->_initialMana += 10;
            this->manaBar->setPercentage(1.0 * _initialMana / _mana * 100);
            if (this->_initialMana >= this->_mana)
                castSkill();
            else
                attackTarget(target); // ����ڹ�����Χ�ڣ��򹥻�
        }
        else {
            moveToTarget(target); // �����ƶ���Ŀ��
        }
    }
}

float Hero::getDistance(Hero* actor)
{//�������
    if (actor == nullptr) {
        return 0;
    }
    float deltax = actor->getPositionX() - this->getPositionX();
    float deltay = actor->getPositionY() - this->getPositionY();
    float distance = sqrt(deltax * deltax + deltay * deltay);
    return distance;
}
void Hero::receiveMagicDamage(float damage) {
    // ����ʵ���˺�������ħ��������
    float actualDamage = damage * (1 - _magicResist / 100.0f);

    // ��������ֵ
    _currentHealth -= actualDamage;
    if (_currentHealth <= 0) {
        onDeath(); // ���������߼�
    }
}
void Hero::moveToTarget(Hero* target) {
    //if (target != nullptr) {
    //    // ��ȡĿ��λ�ú͵�ǰλ��
    //    Vec2 targetPosition = target->getPosition();

    //    //targetPosition.x=

    //    Vec2 currentPosition = this->getPosition();
    //    
    //    //// ���㷽������
    //    //Vec2 direction = targetPosition - currentPosition;

    //    //// ���Ӣ���Ѿ���Ŀ�긽�����������ƶ�
    //    //if (direction.lengthSquared() <= _attackRange * _attackRange) {
    //    //    return;
    //    //}

    //    //// ��׼�����������������ƶ�����
    //    //Vec2 normalizedDirection = direction.getNormalized();
    //    //Vec2 movement = normalizedDirection * 50;

    //    //// ������λ��
    //    //Vec2 newPos = currentPosition + movement;

    //    //// ����Ӣ��λ��

    //    //float distance = getDistance(target);
    //    //auto action = MoveTo::create(distance / 50, target->getPosition());
    //    //this->runAction(action);


    //    //Vec2 targetPosition = target->getPosition();
    //    //Vec2 currentPosition = this->getPosition();

    //    // ���㷽������
    //    // �����µ�λ��
    //    //Vec2 newPosition = currentPosition + delta;

    //    //// ʹ�� clamp ������λ����ָ��������
    //    //newPosition.x = clamp(newPosition.x, targetRect.origin.x, targetRect.origin.x + targetRect.size.width);
    //    //newPosition.y = clamp(newPosition.y, targetRect.origin.y, targetRect.origin.y + targetRect.size.height);
    //    Vec2 direction = targetPosition - currentPosition;

    //    // ���Ӣ���Ѿ���Ŀ�긽�����������ƶ�
    //    if (direction.lengthSquared() <= _attackRange * _attackRange) {
    //        return;
    //    }

    //    // ��׼�����������������ƶ�����
    //    Vec2 normalizedDirection = direction.getNormalized();
    //    Vec2 movement = normalizedDirection * 50;

    //    // ������λ��
    //    Vec2 newPos = currentPosition + movement;

    //    // �����ƶ���ʱ��
    //    float distance = getDistance(target);
    //    float moveTime = distance / 50;


    //    // ��Ӷ�����ɵĻص�����
    //    auto callback = CallFunc::create([this]() {
    //        // �ڻص�������ֹͣ���ж����������ٶȹ���
    //        this->stopAllActions();
    //        });
    //    // ʹ��MoveTo�����ƶ�Ӣ��
    //    auto moveToAction = MoveTo::create(moveTime, target->getPosition());

    //    // ����һ���������У���ִ��MoveTo��Ȼ��ִ�лص�
    //    auto sequence = Sequence::create(moveToAction, callback, nullptr);

    //    // ���ж�������
    //    this->runAction(sequence);
    //    //this->runAction(moveToAction);
    //    //TO DO:�е�bug
    //    //this->setPosition(newPos);
    //}
    
    if (target != nullptr) {
        // ��ȡĿ��λ�ú͵�ǰλ��
        Vec2 targetPosition = target->getPosition();
        Vec2 currentPosition = this->getPosition();

        // ���㷽������
        Vec2 direction = targetPosition - currentPosition;

        // ���Ӣ���Ѿ���Ŀ�긽�����������ƶ�
        if (direction.lengthSquared() <= _attackRange * _attackRange) {
            return;
        }

        // ��׼�����������������ƶ�����
        Vec2 normalizedDirection = direction.getNormalized();
        Vec2 movement = normalizedDirection * 50;

        // ������λ��
        Vec2 newPos = currentPosition + movement;

        // ʹ�� clamp ������λ����ָ��������
        newPos.x = clamp(newPos.x, targetRect.origin.x, targetRect.origin.x + targetRect.size.width);
        newPos.y = clamp(newPos.y, targetRect.origin.y, targetRect.origin.y + targetRect.size.height);

        // �����ƶ���ʱ��
        float distance = getDistance(target);
        float moveTime = distance / 50;

        // ��Ӷ�����ɵĻص�����
        auto callback = CallFunc::create([this]() {
            // �ڻص�������ֹͣ���ж����������ٶȹ���
            this->stopAllActions();
            });

        // ʹ�� MoveTo �����ƶ�Ӣ�۵���λ��
        auto moveToAction = MoveTo::create(moveTime, newPos);

        // ����һ���������У���ִ�� MoveTo��Ȼ��ִ�лص�
        auto sequence = Sequence::create(moveToAction, callback, nullptr);

        // ���ж�������
        this->runAction(sequence);
    }

    }


//��������
void Hero::playAttackAnimation() {
    // ������ת�ǶȺͶ�������ʱ��
    float rotateAngle = 10.0f; // ��ת�Ƕȣ����Ե���Ϊ���ʺϵ�ֵ
    float duration = 0.1f; // ��������ʱ��

    // ������ת����
    auto rotateAction = RotateBy::create(duration, rotateAngle);
    auto rotateBackAction = rotateAction->reverse();

    // �����������У���ת��ȥȻ����ת����
    auto attackAnimation = Sequence::create(rotateAction, rotateBackAction, nullptr);

    // ִ�ж���
    this->runAction(attackAnimation);
}
void Hero::performRangedAttackAnimation(Hero* target) {
    if (_attackRange > 200) { // ����Ƿ�ΪԶ�̹�����Ӣ��
        // �����ӵ�����
        auto bullet = Sprite::create("bullet.png");
        bullet->setPosition(this->getPosition()); // ��ʼλ������ΪӢ�۵�λ��
        bullet->setScale(2.0f);
        this->addChild(bullet); // ȷ���ӵ�����ӵ���Ӣ����ͬ�Ĳ㼶
        
        // ����һ�����������ӵ��ƶ���Ŀ��λ��
        auto moveAction = MoveTo::create(0.5f, target->getPosition()); // ����ʱ����ƥ���ӵ��ķ����ٶ�
        auto removeAction = CallFunc::create([bullet]() {
            bullet->removeFromParent(); // �����������Ƴ��ӵ�
            });

        // ���ж���
        bullet->runAction(Sequence::create(moveAction, removeAction, nullptr));
    }
}

//����
void Hero::attackTarget(Hero* target) {
    // ǿ��ת��Ŀ��Ϊ Hero ����
    auto enemyHero = dynamic_cast<Hero*>(target);
    if (enemyHero) {
        // ������ɵ��˺�
        float damage = _currentPhysicalAttack; // ��������˺����㣬ʵ�ʿ��ܸ�����

        // Ӧ���˺���Ŀ��Ӣ��
        enemyHero->receiveDamage(damage);

        // ���Ź�������
        playAttackAnimation();
        // ����Զ�̹�����Ӣ�ۣ�ִ�ж���Ķ���
        performRangedAttackAnimation(target);
    }
}
//����
void Hero::receiveDamage(float damage) {
    this->_initialMana += 10;
    if(_currentHealth - damage<0){
        _currentHealth = 0;
        onDeath(); // ���������߼�
    }
    else {
        _currentHealth -= damage; // ��������ֵ
    }
    int index = static_cast<int>(_level);
    CCLOG("the health:%f", 1.0 * _currentHealth / _health[index]*100);
    this->getBloodBar()->setPercentage(1.0*_currentHealth / _health[index]*100);
    
}

void Hero::onDeath() {
    // ������������
    auto deathAnimation = Sequence::create(
        FadeOut::create(1.0f), // 1��������ʧ
        CallFunc::create([this]() {
            this->removeFromParent(); // �Ӹ��ڵ��Ƴ�
            this->setIsDead(true);
            }),
        nullptr
                );

    this->runAction(deathAnimation);

    // ����������ִ��������������߼���������·����������¼���
    // ���磬֪ͨ��Ϸ������Ӣ������
    // GameManager::getInstance()->onHeroDeath(this);
}

//�ж��Ƿ��ڹ�������
bool Hero::isInRange(Hero* target) {
    float distance = getDistance(target);
    return distance <= _attackRange; // �жϾ����Ƿ��ڹ�����Χ��
}
bool Hero::isEnemy(Hero* hero) {
    // ������߼�ȡ����������Ϸ��ơ�
    // ������һ�������ŶӲ�ͬ���жϵ��˵�ʾ����

    // ���Ӣ���Ƿ�����Լ��Ƿ����ڲ�ͬ���Ŷ�
    if (hero && this->_team != hero->_team) {
        return true; // ��ͬ�Ŷӵ�Ӣ�۱���Ϊ����
    }

    return false; // ͬһ�Ŷӻ���ЧӢ�۲��ǵ���
}
bool Hero::isEnd()
{//��Ҫ�޸ģ�ֻ�ж��˶Է��Ƿ����⣬������������д��Player�����ж�
    auto children = this->getScene()->getChildren();
    if (children.empty()) {
        return 1;
    }
    return 0;
}


AkaliHero::AkaliHero()
    : Hero({ 900, 1620, 2916 }, { 60, 90, 135 }, 50, 50, 0.25, 50, 10, 50, 0.85, Level::Level1) {
    _hero_number = 0;
    _star = 4;
    _career = 1;
    hero_type = 1;
    _power = _star + int(_level);
    // ��ʼ�� Akali ���е�״̬�������߼�
}

AkaliHero* AkaliHero::create() {
    AkaliHero* akali = new (std::nothrow) AkaliHero();
    if (akali && akali->initWithFile("heroes/1.png")) { // ������ akali.png �ļ�
        akali->autorelease();
        return akali;
    }
    CC_SAFE_DELETE(akali);
    return nullptr;
}

void AkaliHero::castSkill() {
    CCLOG("Akali casts Unending Melody.");

    // �˺�ֵ���ݵȼ��仯
    float initialDamage[3] = { 114, 171, 513 };

    // �ҵ���Զ�ĵ��˲���������˺�
    Hero* farthestEnemy = getFarthestEnemy();
    if (farthestEnemy) {
        // ģ�⹥����Զ�ĵ���
        float originalAttack = _currentPhysicalAttack;
        _currentPhysicalAttack = initialDamage[static_cast<int>(_level)];
        attackTarget(farthestEnemy);
        _currentPhysicalAttack = originalAttack; // �ָ�ԭʼ������
    }
}

Hero* AkaliHero::getFarthestEnemy() {
    // ��ȡ���������еĵ��˽ڵ�
    Hero* target = getNextEnemyHero(); // ��ȡ����ĵз���λ
    
    return target;
}
AnnieHero::AnnieHero()
    : Hero({ 500, 900, 1620 }, { 40, 60, 90 }, 20, 20, 0.25, 300, 0, 50, 0.7, Level::Level1),
    _skillCastCount(0) {
    _hero_number = 1;
    _star = 1;
    _career = 3;
    hero_type = 2;
    _power = _star + int(_level);
    // ��ʼ�� Annie ���е�״̬�������߼�
}

AnnieHero* AnnieHero::create() {
    AnnieHero* annie = new (std::nothrow) AnnieHero();
    if (annie && annie->initWithFile("heroes/2.png")) { // ������ annie.png �ļ�
        annie->autorelease();
        return annie;
    }
    CC_SAFE_DELETE(annie);
    return nullptr;
}

void AnnieHero::castSkill() {
    CCLOG("Annie casts Dark Fracture.");

    float activeDamage[3] = { 195, 295, 440 };
    // �Ե�ǰ����Ŀ�����ħ���˺�
    int levelIndex = static_cast<int>(_level); // ��ö��ת��Ϊ��������
    Hero* target = getNextEnemyHero(); // ��ȡ����ĵз���λ
    target->receiveDamage(activeDamage[levelIndex]);
    

    // ����������Ч��
    applyPassive();
}

void AnnieHero::applyPassive() {
    _skillCastCount++;
    if (_skillCastCount >= 4) {
        _attackSpeed *= 1.5f; // ����50%�����ٶ�
        _skillCastCount = 0; // ���ü����ͷż���
    }
}

AriHero::AriHero()
    : Hero({ 700, 1260, 2268 }, { 50, 75, 113 }, 30, 30, 0.25, 300, 0, 30, 0.65) {
    // ��ʼ�� Ari ���е�״̬�������߼�
    _hero_number = 2;
    _star = 4;
    _career = 3;
    hero_type = 3;
    _power = _star + int(_level);
}

AriHero* AriHero::create() {
    AriHero* ari = new (std::nothrow) AriHero();
    if (ari && ari->initWithFile("heroes/3.png")) { // ������ ari.png �ļ�
        ari->autorelease();
        return ari;
    }
    CC_SAFE_DELETE(ari);
    return nullptr;
}

void AriHero::castSkill() {
    CCLOG("Ari casts Idol Charm.");

    // ���ݵȼ�ȷ���˺�ֵ
    float normalDamage[3] = { 275, 410, 820 };
    float enhancedDamage[3] = { 480, 720, 1440 };
    int levelIndex = static_cast<int>(_level); // ��ö��ת��Ϊ��������

    // ��ȡ��ǰ����Ŀ��
    Hero* target = getNextEnemyHero(); // ��ȡ����ĵз���λ
    if (target) {
        // ��Ŀ�������ͨ�˺�
        dealMagicDamageToTarget(target, normalDamage[levelIndex]);

        // ��Ŀ�������ǿ�˺�
        dealMagicDamageToTarget(target, enhancedDamage[levelIndex]);
    }
}

void AriHero::dealMagicDamageToTarget(Hero* target, float damage) {
    // ǿ��ת��Ŀ��Ϊ Hero ����
    auto enemyHero = dynamic_cast<Hero*>(target);
    if (enemyHero) {
        // Ӧ���˺���Ŀ��Ӣ��
        enemyHero->receiveDamage(damage);

        // ���Ź�������
        playAttackAnimation();
    }
}

CatalinaHero::CatalinaHero()
    : Hero({ 650, 1170, 2106 }, { 40, 60, 90 }, 35, 35, 0.25, 100, 20, 60, 0.7, Level::Level1) {
    // ��ʼ�� Catalina ���е�״̬�������߼�
    _hero_number = 3;
    _star = 2;
    _career = 1;
    hero_type = 4;
    _power = _star + int(_level);
}

CatalinaHero* CatalinaHero::create() {
    CatalinaHero* catalina = new (std::nothrow) CatalinaHero();
    if (catalina && catalina->initWithFile("heroes/4.png")) { // ������ catalina.png �ļ�
        catalina->autorelease();
        return catalina;
    }
    CC_SAFE_DELETE(catalina);
    return nullptr;
}

void CatalinaHero::castSkill() {
    CCLOG("Catalina casts Ricochet Blade.");

    float damage[3] = { 140, 210, 315 };
    int levelIndex = static_cast<int>(_level); // ��ö��ת��Ϊ��������

    // ��ȡ��ǰ����Ŀ��
    Hero* target = dynamic_cast<Hero*>(getNextEnemyHero());
    if (target) {
        // ��Ŀ������˺�
        target->receiveDamage(damage[levelIndex]);

        // �����Ҫ��ʵ�����е����߼�
        // �������Ҫ������߼���ѡ����һ��Ŀ�겢Ӧ���˺�
        // ...
    }
    performRangedAttackAnimation(target);
}

EvelynnHero::EvelynnHero()
    : Hero({ 700, 1260, 2268 }, { 50, 75, 113 }, 30, 30, 0.25, 100, 0, 40, 0.85, Level::Level1),
    _skillCastCount(0) {
    // ��ʼ�� Evelynn ���е�״̬�������߼�
    _hero_number = 4;
    _star = 1;
    _career = 1;
    hero_type = 5;
    _power = _star + int(_level);
}

EvelynnHero* EvelynnHero::create() {
    EvelynnHero* evelynn = new (std::nothrow) EvelynnHero();
    if (evelynn && evelynn->initWithFile("heroes/5.png")) { // ������ evelynn.png �ļ�
        evelynn->autorelease();
        return evelynn;
    }
    CC_SAFE_DELETE(evelynn);
    return nullptr;
}

void EvelynnHero::castSkill() {
    CCLOG("Evelynn casts Party Crash.");

    float lineDamage[3] = { 325, 488, 1463 }; // ֱ���˺�
    int levelIndex = static_cast<int>(_level); // ��ö��ת��Ϊ��������

    Hero* target = getNextEnemyHero(); // ��ȡ����ĵз���λ
    if (target) {
        if (isInRange(target)) {
            auto enemyHero = dynamic_cast<Hero*>(target);
            if (enemyHero) {
                // ������ɵ��˺�
                float damage = lineDamage[levelIndex]; // ��������˺����㣬ʵ�ʿ��ܸ�����

                // Ӧ���˺���Ŀ��Ӣ��
                enemyHero->receiveDamage(damage);// ����ڹ�����Χ�ڣ��򹥻�
            }
        }
    }

}

EzrealHero :: EzrealHero()
    : Hero({ 990, 1460, 2350 }, { 60, 85, 133 }, 30, 30, 0.25, 600, 0, 40, 1.2, Level::Level1),
    _skillCastCount(0) {
    // ��ʼ�� Evelynn ���е�״̬�������߼�
    _hero_number = 5;
    _star = 4;
    _career = 2;
    hero_type = 6;
    _power = _star + int(_level);
}

EzrealHero* EzrealHero::create() {
    EzrealHero* evelynn = new (std::nothrow) EzrealHero();
    if (evelynn && evelynn->initWithFile("heroes/6.png")) { // ������ evelynn.png �ļ�
        evelynn->autorelease();
        return evelynn;
    }
    CC_SAFE_DELETE(evelynn);
    return nullptr;
}

void EzrealHero::castSkill() {
    CCLOG("Evelynn casts Party Crash.");

    float lineDamage[3] = { 325, 488, 1463 }; // ֱ���˺�
    int levelIndex = static_cast<int>(_level); // ��ö��ת��Ϊ��������
    
    Hero* target = getNextEnemyHero(); // ��ȡ����ĵз���λ
    if (target) {
        if (isInRange(target)) {
            
                attackTarget(target); // ����ڹ�����Χ�ڣ��򹥻�
        }
    }
}

GalenHero::GalenHero()
    : Hero({ 750, 1350, 2430 }, { 55, 83, 124 }, 45, 45, 0.25, 100, 50, 100, 0.7, Level::Level1) {
    // ��ʼ�� Galen ���е�״̬�������߼�
    _hero_number = 6;
    _star = 2;
    _career = 0;
    hero_type = 7;
    _power = _star + int(_level);
}

GalenHero* GalenHero::create() {
    GalenHero* galen = new (std::nothrow) GalenHero();
    if (galen && galen->initWithFile("heroes/7.png")) { // ������ galen.png �ļ�
        galen->autorelease();
        return galen;
    }
    CC_SAFE_DELETE(galen);
    return nullptr;
}

void GalenHero::castSkill() {
    CCLOG("Galen activates Power Surge!");

    // �����������ֵ
    float additionalHealth[3] = { 200, 215, 230 };
    int levelIndex = static_cast<int>(_level); // ��ö��ת��Ϊ��������
    _currentHealth += additionalHealth[levelIndex];
    _health[levelIndex] += additionalHealth[levelIndex]; // �����������ֵ

    // �´ι�����ɶ����˺�
    float additionalDamage[3] = { 163, 267, 441 };
    float _extraAttackDamage = additionalDamage[levelIndex]; // �洢�����˺�ֵ

    // �����һ�ι�����Ӧ�ö����˺� 
    Hero* target = dynamic_cast<Hero*>(getNextEnemyHero());
    auto enemyHero = dynamic_cast<Hero*>(target);
    if (enemyHero) {
        // ������ɵ��˺�
        float damage = _currentPhysicalAttack + _extraAttackDamage;// ��������˺����㣬ʵ�ʿ��ܸ�����

        // Ӧ���˺���Ŀ��Ӣ��
        enemyHero->receiveDamage(damage);

        // ���Ź�������
        playAttackAnimation();
    }
}

GiggsHero::GiggsHero()
    : Hero({ 800, 1440, 2592 }, { 65, 98, 146 }, 40, 40, 0.25, 400, 30, 75, 0.8, Level::Level1) {
    // ��ʼ�� Giggs ���е�״̬�������߼�
    _hero_number = 7;
    _star = 5;
    _career = 3;
    hero_type = 8;
    _power = _star + int(_level);
}

GiggsHero* GiggsHero::create() {
    GiggsHero* giggs = new (std::nothrow) GiggsHero();
    if (giggs && giggs->initWithFile("heroes/8.png")) { // ������ giggs.png �ļ�
        giggs->autorelease();
        return giggs;
    }
    CC_SAFE_DELETE(giggs);
    return nullptr;
}

void GiggsHero::castSkill() {
    CCLOG("Giggs casts Chaos Theory.");

    // ħ���˺�ֵ���ݵȼ��仯
    float magicDamage[3] = { 360, 540, 4000 };
    int levelIndex = static_cast<int>(_level); // ��ö��ת��Ϊ��������

    // ѡ��ǰĿ�겢��������˺�
    Hero* target = getNextEnemyHero(); // ��ȡ����ĵз���λ
    if (target) {
        auto enemyHero = dynamic_cast<Hero*>(target);
        if (enemyHero) {
            // ��Ŀ�����ħ���˺�
            enemyHero->receiveMagicDamage(magicDamage[levelIndex]);
            // �����������Ч������ը�����ѡ�ħ�������
            // ...
        }
    }
}

JinxHero::JinxHero()
    : Hero({ 450, 810, 1458 }, { 45, 68, 101 }, 15, 15, 0.25, 500, 0, 40, 0.7, Level::Level1) {
    // ��ʼ�� Jinx ���е�״̬�������߼�
    _hero_number = 8;
    _star = 1;
    _career = 2;
    hero_type = 9;
    _power = _star + int(_level);
}

JinxHero* JinxHero::create() {
    JinxHero* jinx = new (std::nothrow) JinxHero();
    if (jinx && jinx->initWithFile("heroes/9.png")) {
        jinx->autorelease();
        return jinx;
    }
    CC_SAFE_DELETE(jinx);
    return nullptr;
}

void JinxHero::castSkill() {
    // ʵ�� Jinx �ļ����߼���ɱ���󹥻��ٶ�����
    CCLOG("Jinx activates her skill.");

    increaseAttackSpeed();

    // TODO: ������Ϸ�ľ������������������Ч�������緢����ͻ��
}

void JinxHero::increaseAttackSpeed() {
    // ���輼��Ч��������50%�Ĺ����ٶȣ�����5��
    float originalAttackSpeed = _attackSpeed;
    _attackSpeed *= 1.5; // ���ӹ����ٶ�

    // ����һ����ʱ������5���ָ�ԭ���Ĺ����ٶ�
    auto delay = DelayTime::create(5.0f);
    auto resetSpeed = CallFunc::create([this, originalAttackSpeed]() {
        _attackSpeed = originalAttackSpeed;
        });

    this->runAction(Sequence::create(delay, resetSpeed, nullptr));
}

KaiSaHero::KaiSaHero()
    : Hero({ 550, 990, 1782 }, { 50, 75, 113 }, 20, 20, 0.25, 500, 0, 60, 0.7, Level::Level1) {
    // ��ʼ�� KaiSa ���е�״̬�������߼�
    _hero_number = 9;
    _star = 2;
    _career = 2;
    hero_type = 10;
    _power = _star + int(_level);
}

KaiSaHero* KaiSaHero::create() {
    KaiSaHero* kaisa = new (std::nothrow) KaiSaHero();
    if (kaisa && kaisa->initWithFile("heroes/10.png")) { // ������ kaisa.png �ļ�
        kaisa->autorelease();
        return kaisa;
    }
    CC_SAFE_DELETE(kaisa);
    return nullptr;
}

void KaiSaHero::castSkill() {
    CCLOG("KaiSa casts Icathian Rain.");

    // �����˺�ֵ���ݵȼ��仯
    float damagePercent[3] = { 280, 280, 285 };
    int levelIndex = static_cast<int>(_level); // ��ö��ת��Ϊ��������

    // ��ȡ��ǰ����Ŀ��
    Hero* target = getNextEnemyHero(); // ��ȡ����ĵз���λ
    if (target) {
        // �����Ŀ�꣨������볬��2�����ƶ�2����룩
        dashToTarget(target, 2);

        // ��Ŀ�귢�䵯�岢����˺�
        auto enemyHero = dynamic_cast<Hero*>(target);
        if (enemyHero) {
            float damage = _currentPhysicalAttack * (damagePercent[levelIndex] / 100.0f);
            enemyHero->receiveDamage(damage);
        }
    }
}

void KaiSaHero::dashToTarget(Hero* target, float maxDashDistance) {
    Vec2 targetPosition = target->getPosition();
    Vec2 currentPosition = this->getPosition();

    // ���㷽��;���
    Vec2 direction = targetPosition - currentPosition;
    float distance = direction.length();

    // ������볬������̾��룬����������
    if (distance > maxDashDistance) {
        direction.normalize();
        Vec2 dashPosition = currentPosition + direction * maxDashDistance;
        this->setPosition(dashPosition);
    }
    else {
        // ����ֱ�ӳ����Ŀ��
        this->setPosition(targetPosition);
    }
}

KennenHero::KennenHero() : Hero({ 650, 1170, 2106 }, { 40, 60, 90 }, 40, 40, 0.25,200, 30, 90, 0.6, Level::Level1) {
    // ��ʼ�� Kennen ���е����Ի�״̬
    _hero_number = 10;
    _star = 1;
    _career = 3;
    hero_type = 11;
    _power = _star + int(_level);
}

KennenHero* KennenHero::create() {
    KennenHero* kennen = new (std::nothrow) KennenHero();
    if (kennen && kennen->initWithFile("heroes/11.png")) { // ������ kennen.png �ļ�
        kennen->autorelease();
        return kennen;
    }
    CC_SAFE_DELETE(kennen);
    return nullptr;
}

void KennenHero::castSkill() {
    // ʵ�� Kennen �ļ����߼������������
    CCLOG("Kennen casts Shock and Stun.");
    float magicDamage[3] = { 360, 540, 4000 };
    int levelIndex = static_cast<int>(_level); // ��ö��ת��Ϊ��������

    // ѡ��ǰĿ�겢��������˺�
    Hero* target = getNextEnemyHero(); // ��ȡ����ĵз���λ
    if (target) {
        auto enemyHero = dynamic_cast<Hero*>(target);
        if (enemyHero) {
            // ��Ŀ�����ħ���˺�
            enemyHero->receiveMagicDamage(magicDamage[levelIndex]);
        }
    }
}

KSanteHero::KSanteHero() : Hero({ 550, 990, 1782 }, { 50, 75, 113 }, 25, 25, 0.25, 60, 0, 40, 0.75, Level::Level1) {
    // ��ʼ�� KSante ���е����Ի�״̬
    _hero_number = 11;
    _star = 1;
    _career = 0;
    hero_type = 12;
    _power = _star + int(_level);
}

KSanteHero* KSanteHero::create() {
    KSanteHero* ksante = new (std::nothrow) KSanteHero();
    if (ksante && ksante->initWithFile("heroes/12.png")) { // ������ ksante.png �ļ�
        ksante->autorelease();
        return ksante;
    }
    CC_SAFE_DELETE(ksante);
    return nullptr;
}

void KSanteHero::castSkill() {
    // ���ܿ�ʼ�����������̬
    CCLOG("KSante enters a defensive stance.");

    // ���ݵȼ����÷�����̬���˺�����Ч��
    float damageReduction = (_level == Level::Level1 || _level == Level::Level2) ? 0.50f : 0.55f;

    _armor *= (1 + damageReduction);
    // ���ܽ������Ե�ǰĿ����ɴ����˺�
    CCLOG("KSante strikes with a powerful blow.");
    Hero* target = getNextEnemyHero(); // ��ȡ����ĵз���λ
    if (target) {
        // �Ե�ǰĿ�����800%�����˺�
        auto enemyHero = dynamic_cast<Hero*>(target);
        if (enemyHero) {
            enemyHero->receiveDamage(_currentPhysicalAttack * 8.0f);
        }
    };

}
LucianHero::LucianHero()
    : Hero({ 750, 1350, 2430 }, { 75, 113, 169 }, 30, 30, 0.25, 400, 50, 125, 0.9, Level::Level1) {
    // ��ʼ�� Lucian ���е�״̬�������߼�
    _hero_number = 12;
    _star = 5;
    _career = 2;
    hero_type = 13;
    _power = _star + int(_level);
}

LucianHero* LucianHero::create() {
    LucianHero* lucian = new (std::nothrow) LucianHero();
    if (lucian && lucian->initWithFile("heroes/13.png")) { // ������ lucian.png �ļ�
        lucian->autorelease();
        return lucian;
    }
    CC_SAFE_DELETE(lucian);
    return nullptr;
}

void LucianHero::castSkill() {
    // ʵ�� Lucian �ļ����߼�������
    CCLOG("Lucian casts The Culling.");

    // ���㷢����ӵ�����
    int bulletCount = 14 + static_cast<int>(_attackSpeed / 0.2); // ÿ20%���⹥������1���ӵ�

    // �����˺�ֵ
    float damagePercent[3] = { 0.55, 0.55, 10.0 }; // �ٷֱ������˺�
    int levelIndex = static_cast<int>(_level);
    Hero* target = getNextEnemyHero(); // ��ȡ����ĵз���λ
    if (target) {
        if (isInRange(target)) {
            auto enemyHero = dynamic_cast<Hero*>(target);
            if (enemyHero) {
                // ������ɵ��˺�
                float damage = _currentPhysicalAttack * damagePercent[levelIndex]; // ��������˺����㣬ʵ�ʿ��ܸ�����

                // Ӧ���˺���Ŀ��Ӣ��
                enemyHero->receiveDamage(damage);// ����ڹ�����Χ�ڣ��򹥻�
                performRangedAttackAnimation(target);
            }
        }
    }
    
}