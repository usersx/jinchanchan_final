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
    // 更新生命值和物理攻击力
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
    
    
    this->setBloodAndMana();//设置血条图片

    this->setIsDead(false);//初始化时均存活

    this->setIsOnTheField(false);//初始化时均不在场上

    this->setTeam(0);

    this->setenemyTeam(1);

    //判断是不是AI阵营的 如果是的话 不用create

    _bar = EquipmentBarforHero::create();
    this->addChild(_bar);

    //setLocalZOrder(2);//此处是渲染顺序 请预设所有英雄的渲染顺序 否则后添加的结点会在旧节点之前被渲染 可能会把图片卡没
    
    return true;
}
//根据坐标放英雄
void Hero::put_on_field(std::pair<int, int> pair_position) {
    //计算场上位置
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
    //调整大小 注意更换节点
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
//        hero->setBloodAndMana();//设置血条图片
//
//        hero->setIsDead(false);//初始化时均存活
//
//        hero->setIsOnTheField(false);//初始化时均不在场上
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
    // 更新生命值和物理攻击力
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
    // 其他属性保持不变
}

void Hero::setBloodAndMana()
{
    //血条初始化
    bloodBar = HealthBar::create("Display2.png", "Display1.png", 100.0f, 20.0f);
    //蓝条初始化
    manaBar = HealthBar::create("Display4.png", "Display3.png", 100.0f, 20.0f);
    //血条定位
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
            //在场上并存活再搜索
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

    Hero* target = getNextEnemyHero(); // 获取最近的敌方单位
    if (target) {
        if (isInRange(target)) {
            this->_initialMana += 10;
            this->manaBar->setPercentage(1.0 * _initialMana / _mana * 100);
            if (this->_initialMana >= this->_mana)
                castSkill();
            else
                attackTarget(target); // 如果在攻击范围内，则攻击
        }
        else {
            moveToTarget(target); // 否则，移动向目标
        }
    }
}

float Hero::getDistance(Hero* actor)
{//计算距离
    if (actor == nullptr) {
        return 0;
    }
    float deltax = actor->getPositionX() - this->getPositionX();
    float deltay = actor->getPositionY() - this->getPositionY();
    float distance = sqrt(deltax * deltax + deltay * deltay);
    return distance;
}
void Hero::receiveMagicDamage(float damage) {
    // 计算实际伤害，考虑魔抗等因素
    float actualDamage = damage * (1 - _magicResist / 100.0f);

    // 减少生命值
    _currentHealth -= actualDamage;
    if (_currentHealth <= 0) {
        onDeath(); // 处理死亡逻辑
    }
}
void Hero::moveToTarget(Hero* target) {
    //if (target != nullptr) {
    //    // 获取目标位置和当前位置
    //    Vec2 targetPosition = target->getPosition();

    //    //targetPosition.x=

    //    Vec2 currentPosition = this->getPosition();
    //    
    //    //// 计算方向向量
    //    //Vec2 direction = targetPosition - currentPosition;

    //    //// 如果英雄已经在目标附近，则无需移动
    //    //if (direction.lengthSquared() <= _attackRange * _attackRange) {
    //    //    return;
    //    //}

    //    //// 标准化方向向量并乘以移动速率
    //    //Vec2 normalizedDirection = direction.getNormalized();
    //    //Vec2 movement = normalizedDirection * 50;

    //    //// 计算新位置
    //    //Vec2 newPos = currentPosition + movement;

    //    //// 更新英雄位置

    //    //float distance = getDistance(target);
    //    //auto action = MoveTo::create(distance / 50, target->getPosition());
    //    //this->runAction(action);


    //    //Vec2 targetPosition = target->getPosition();
    //    //Vec2 currentPosition = this->getPosition();

    //    // 计算方向向量
    //    // 计算新的位置
    //    //Vec2 newPosition = currentPosition + delta;

    //    //// 使用 clamp 限制新位置在指定区域内
    //    //newPosition.x = clamp(newPosition.x, targetRect.origin.x, targetRect.origin.x + targetRect.size.width);
    //    //newPosition.y = clamp(newPosition.y, targetRect.origin.y, targetRect.origin.y + targetRect.size.height);
    //    Vec2 direction = targetPosition - currentPosition;

    //    // 如果英雄已经在目标附近，则无需移动
    //    if (direction.lengthSquared() <= _attackRange * _attackRange) {
    //        return;
    //    }

    //    // 标准化方向向量并乘以移动速率
    //    Vec2 normalizedDirection = direction.getNormalized();
    //    Vec2 movement = normalizedDirection * 50;

    //    // 计算新位置
    //    Vec2 newPos = currentPosition + movement;

    //    // 计算移动的时间
    //    float distance = getDistance(target);
    //    float moveTime = distance / 50;


    //    // 添加动作完成的回调函数
    //    auto callback = CallFunc::create([this]() {
    //        // 在回调函数中停止所有动作，即将速度归零
    //        this->stopAllActions();
    //        });
    //    // 使用MoveTo动作移动英雄
    //    auto moveToAction = MoveTo::create(moveTime, target->getPosition());

    //    // 创建一个动作序列，先执行MoveTo，然后执行回调
    //    auto sequence = Sequence::create(moveToAction, callback, nullptr);

    //    // 运行动作序列
    //    this->runAction(sequence);
    //    //this->runAction(moveToAction);
    //    //TO DO:有点bug
    //    //this->setPosition(newPos);
    //}
    
    if (target != nullptr) {
        // 获取目标位置和当前位置
        Vec2 targetPosition = target->getPosition();
        Vec2 currentPosition = this->getPosition();

        // 计算方向向量
        Vec2 direction = targetPosition - currentPosition;

        // 如果英雄已经在目标附近，则无需移动
        if (direction.lengthSquared() <= _attackRange * _attackRange) {
            return;
        }

        // 标准化方向向量并乘以移动速率
        Vec2 normalizedDirection = direction.getNormalized();
        Vec2 movement = normalizedDirection * 50;

        // 计算新位置
        Vec2 newPos = currentPosition + movement;

        // 使用 clamp 限制新位置在指定区域内
        newPos.x = clamp(newPos.x, targetRect.origin.x, targetRect.origin.x + targetRect.size.width);
        newPos.y = clamp(newPos.y, targetRect.origin.y, targetRect.origin.y + targetRect.size.height);

        // 计算移动的时间
        float distance = getDistance(target);
        float moveTime = distance / 50;

        // 添加动作完成的回调函数
        auto callback = CallFunc::create([this]() {
            // 在回调函数中停止所有动作，即将速度归零
            this->stopAllActions();
            });

        // 使用 MoveTo 动作移动英雄到新位置
        auto moveToAction = MoveTo::create(moveTime, newPos);

        // 创建一个动作序列，先执行 MoveTo，然后执行回调
        auto sequence = Sequence::create(moveToAction, callback, nullptr);

        // 运行动作序列
        this->runAction(sequence);
    }

    }


//攻击动画
void Hero::playAttackAnimation() {
    // 设置旋转角度和动画持续时间
    float rotateAngle = 10.0f; // 旋转角度，可以调整为更适合的值
    float duration = 0.1f; // 动画持续时间

    // 创建旋转动作
    auto rotateAction = RotateBy::create(duration, rotateAngle);
    auto rotateBackAction = rotateAction->reverse();

    // 创建动画序列：旋转出去然后旋转回来
    auto attackAnimation = Sequence::create(rotateAction, rotateBackAction, nullptr);

    // 执行动画
    this->runAction(attackAnimation);
}
void Hero::performRangedAttackAnimation(Hero* target) {
    if (_attackRange > 200) { // 检查是否为远程攻击型英雄
        // 创建子弹精灵
        auto bullet = Sprite::create("bullet.png");
        bullet->setPosition(this->getPosition()); // 初始位置设置为英雄的位置
        bullet->setScale(2.0f);
        this->addChild(bullet); // 确保子弹被添加到与英雄相同的层级
        
        // 创建一个动画，将子弹移动到目标位置
        auto moveAction = MoveTo::create(0.5f, target->getPosition()); // 调整时间以匹配子弹的飞行速度
        auto removeAction = CallFunc::create([bullet]() {
            bullet->removeFromParent(); // 动画结束后移除子弹
            });

        // 运行动画
        bullet->runAction(Sequence::create(moveAction, removeAction, nullptr));
    }
}

//攻击
void Hero::attackTarget(Hero* target) {
    // 强制转换目标为 Hero 类型
    auto enemyHero = dynamic_cast<Hero*>(target);
    if (enemyHero) {
        // 计算造成的伤害
        float damage = _currentPhysicalAttack; // 这里简化了伤害计算，实际可能更复杂

        // 应用伤害到目标英雄
        enemyHero->receiveDamage(damage);

        // 播放攻击动画
        playAttackAnimation();
        // 对于远程攻击型英雄，执行额外的动画
        performRangedAttackAnimation(target);
    }
}
//受伤
void Hero::receiveDamage(float damage) {
    this->_initialMana += 10;
    if(_currentHealth - damage<0){
        _currentHealth = 0;
        onDeath(); // 处理死亡逻辑
    }
    else {
        _currentHealth -= damage; // 减少生命值
    }
    int index = static_cast<int>(_level);
    CCLOG("the health:%f", 1.0 * _currentHealth / _health[index]*100);
    this->getBloodBar()->setPercentage(1.0*_currentHealth / _health[index]*100);
    
}

void Hero::onDeath() {
    // 播放死亡动画
    auto deathAnimation = Sequence::create(
        FadeOut::create(1.0f), // 1秒内逐渐消失
        CallFunc::create([this]() {
            this->removeFromParent(); // 从父节点移除
            this->setIsDead(true);
            }),
        nullptr
                );

    this->runAction(deathAnimation);

    // 可以在这里执行其他死亡相关逻辑，比如更新分数、触发事件等
    // 例如，通知游戏管理器英雄死亡
    // GameManager::getInstance()->onHeroDeath(this);
}

//判断是否在攻击距离
bool Hero::isInRange(Hero* target) {
    float distance = getDistance(target);
    return distance <= _attackRange; // 判断距离是否在攻击范围内
}
bool Hero::isEnemy(Hero* hero) {
    // 这里的逻辑取决于您的游戏设计。
    // 以下是一个基于团队不同来判断敌人的示例：

    // 检查英雄是否存在以及是否属于不同的团队
    if (hero && this->_team != hero->_team) {
        return true; // 不同团队的英雄被视为敌人
    }

    return false; // 同一团队或无效英雄不是敌人
}
bool Hero::isEnd()
{//需要修改，只判断了对方是否死光，不过可以在我写的Player类中判断
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
    // 初始化 Akali 特有的状态或其他逻辑
}

AkaliHero* AkaliHero::create() {
    AkaliHero* akali = new (std::nothrow) AkaliHero();
    if (akali && akali->initWithFile("heroes/1.png")) { // 假设有 akali.png 文件
        akali->autorelease();
        return akali;
    }
    CC_SAFE_DELETE(akali);
    return nullptr;
}

void AkaliHero::castSkill() {
    CCLOG("Akali casts Unending Melody.");

    // 伤害值根据等级变化
    float initialDamage[3] = { 114, 171, 513 };

    // 找到最远的敌人并对其造成伤害
    Hero* farthestEnemy = getFarthestEnemy();
    if (farthestEnemy) {
        // 模拟攻击最远的敌人
        float originalAttack = _currentPhysicalAttack;
        _currentPhysicalAttack = initialDamage[static_cast<int>(_level)];
        attackTarget(farthestEnemy);
        _currentPhysicalAttack = originalAttack; // 恢复原始攻击力
    }
}

Hero* AkaliHero::getFarthestEnemy() {
    // 获取场景中所有的敌人节点
    Hero* target = getNextEnemyHero(); // 获取最近的敌方单位
    
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
    // 初始化 Annie 特有的状态或其他逻辑
}

AnnieHero* AnnieHero::create() {
    AnnieHero* annie = new (std::nothrow) AnnieHero();
    if (annie && annie->initWithFile("heroes/2.png")) { // 假设有 annie.png 文件
        annie->autorelease();
        return annie;
    }
    CC_SAFE_DELETE(annie);
    return nullptr;
}

void AnnieHero::castSkill() {
    CCLOG("Annie casts Dark Fracture.");

    float activeDamage[3] = { 195, 295, 440 };
    // 对当前攻击目标造成魔法伤害
    int levelIndex = static_cast<int>(_level); // 将枚举转换为整数索引
    Hero* target = getNextEnemyHero(); // 获取最近的敌方单位
    target->receiveDamage(activeDamage[levelIndex]);
    

    // 处理被动技能效果
    applyPassive();
}

void AnnieHero::applyPassive() {
    _skillCastCount++;
    if (_skillCastCount >= 4) {
        _attackSpeed *= 1.5f; // 增加50%攻击速度
        _skillCastCount = 0; // 重置技能释放计数
    }
}

AriHero::AriHero()
    : Hero({ 700, 1260, 2268 }, { 50, 75, 113 }, 30, 30, 0.25, 300, 0, 30, 0.65) {
    // 初始化 Ari 特有的状态或其他逻辑
    _hero_number = 2;
    _star = 4;
    _career = 3;
    hero_type = 3;
    _power = _star + int(_level);
}

AriHero* AriHero::create() {
    AriHero* ari = new (std::nothrow) AriHero();
    if (ari && ari->initWithFile("heroes/3.png")) { // 假设有 ari.png 文件
        ari->autorelease();
        return ari;
    }
    CC_SAFE_DELETE(ari);
    return nullptr;
}

void AriHero::castSkill() {
    CCLOG("Ari casts Idol Charm.");

    // 根据等级确定伤害值
    float normalDamage[3] = { 275, 410, 820 };
    float enhancedDamage[3] = { 480, 720, 1440 };
    int levelIndex = static_cast<int>(_level); // 将枚举转换为整数索引

    // 获取当前攻击目标
    Hero* target = getNextEnemyHero(); // 获取最近的敌方单位
    if (target) {
        // 对目标造成普通伤害
        dealMagicDamageToTarget(target, normalDamage[levelIndex]);

        // 对目标造成增强伤害
        dealMagicDamageToTarget(target, enhancedDamage[levelIndex]);
    }
}

void AriHero::dealMagicDamageToTarget(Hero* target, float damage) {
    // 强制转换目标为 Hero 类型
    auto enemyHero = dynamic_cast<Hero*>(target);
    if (enemyHero) {
        // 应用伤害到目标英雄
        enemyHero->receiveDamage(damage);

        // 播放攻击动画
        playAttackAnimation();
    }
}

CatalinaHero::CatalinaHero()
    : Hero({ 650, 1170, 2106 }, { 40, 60, 90 }, 35, 35, 0.25, 100, 20, 60, 0.7, Level::Level1) {
    // 初始化 Catalina 特有的状态或其他逻辑
    _hero_number = 3;
    _star = 2;
    _career = 1;
    hero_type = 4;
    _power = _star + int(_level);
}

CatalinaHero* CatalinaHero::create() {
    CatalinaHero* catalina = new (std::nothrow) CatalinaHero();
    if (catalina && catalina->initWithFile("heroes/4.png")) { // 假设有 catalina.png 文件
        catalina->autorelease();
        return catalina;
    }
    CC_SAFE_DELETE(catalina);
    return nullptr;
}

void CatalinaHero::castSkill() {
    CCLOG("Catalina casts Ricochet Blade.");

    float damage[3] = { 140, 210, 315 };
    int levelIndex = static_cast<int>(_level); // 将枚举转换为整数索引

    // 获取当前攻击目标
    Hero* target = dynamic_cast<Hero*>(getNextEnemyHero());
    if (target) {
        // 对目标造成伤害
        target->receiveDamage(damage[levelIndex]);

        // 如果需要，实现利刃弹跳逻辑
        // 这可能需要额外的逻辑来选择下一个目标并应用伤害
        // ...
    }
    performRangedAttackAnimation(target);
}

EvelynnHero::EvelynnHero()
    : Hero({ 700, 1260, 2268 }, { 50, 75, 113 }, 30, 30, 0.25, 100, 0, 40, 0.85, Level::Level1),
    _skillCastCount(0) {
    // 初始化 Evelynn 特有的状态或其他逻辑
    _hero_number = 4;
    _star = 1;
    _career = 1;
    hero_type = 5;
    _power = _star + int(_level);
}

EvelynnHero* EvelynnHero::create() {
    EvelynnHero* evelynn = new (std::nothrow) EvelynnHero();
    if (evelynn && evelynn->initWithFile("heroes/5.png")) { // 假设有 evelynn.png 文件
        evelynn->autorelease();
        return evelynn;
    }
    CC_SAFE_DELETE(evelynn);
    return nullptr;
}

void EvelynnHero::castSkill() {
    CCLOG("Evelynn casts Party Crash.");

    float lineDamage[3] = { 325, 488, 1463 }; // 直线伤害
    int levelIndex = static_cast<int>(_level); // 将枚举转换为整数索引

    Hero* target = getNextEnemyHero(); // 获取最近的敌方单位
    if (target) {
        if (isInRange(target)) {
            auto enemyHero = dynamic_cast<Hero*>(target);
            if (enemyHero) {
                // 计算造成的伤害
                float damage = lineDamage[levelIndex]; // 这里简化了伤害计算，实际可能更复杂

                // 应用伤害到目标英雄
                enemyHero->receiveDamage(damage);// 如果在攻击范围内，则攻击
            }
        }
    }

}

EzrealHero :: EzrealHero()
    : Hero({ 990, 1460, 2350 }, { 60, 85, 133 }, 30, 30, 0.25, 600, 0, 40, 1.2, Level::Level1),
    _skillCastCount(0) {
    // 初始化 Evelynn 特有的状态或其他逻辑
    _hero_number = 5;
    _star = 4;
    _career = 2;
    hero_type = 6;
    _power = _star + int(_level);
}

EzrealHero* EzrealHero::create() {
    EzrealHero* evelynn = new (std::nothrow) EzrealHero();
    if (evelynn && evelynn->initWithFile("heroes/6.png")) { // 假设有 evelynn.png 文件
        evelynn->autorelease();
        return evelynn;
    }
    CC_SAFE_DELETE(evelynn);
    return nullptr;
}

void EzrealHero::castSkill() {
    CCLOG("Evelynn casts Party Crash.");

    float lineDamage[3] = { 325, 488, 1463 }; // 直线伤害
    int levelIndex = static_cast<int>(_level); // 将枚举转换为整数索引
    
    Hero* target = getNextEnemyHero(); // 获取最近的敌方单位
    if (target) {
        if (isInRange(target)) {
            
                attackTarget(target); // 如果在攻击范围内，则攻击
        }
    }
}

GalenHero::GalenHero()
    : Hero({ 750, 1350, 2430 }, { 55, 83, 124 }, 45, 45, 0.25, 100, 50, 100, 0.7, Level::Level1) {
    // 初始化 Galen 特有的状态或其他逻辑
    _hero_number = 6;
    _star = 2;
    _career = 0;
    hero_type = 7;
    _power = _star + int(_level);
}

GalenHero* GalenHero::create() {
    GalenHero* galen = new (std::nothrow) GalenHero();
    if (galen && galen->initWithFile("heroes/7.png")) { // 假设有 galen.png 文件
        galen->autorelease();
        return galen;
    }
    CC_SAFE_DELETE(galen);
    return nullptr;
}

void GalenHero::castSkill() {
    CCLOG("Galen activates Power Surge!");

    // 增加最大生命值
    float additionalHealth[3] = { 200, 215, 230 };
    int levelIndex = static_cast<int>(_level); // 将枚举转换为整数索引
    _currentHealth += additionalHealth[levelIndex];
    _health[levelIndex] += additionalHealth[levelIndex]; // 更新最大生命值

    // 下次攻击造成额外伤害
    float additionalDamage[3] = { 163, 267, 441 };
    float _extraAttackDamage = additionalDamage[levelIndex]; // 存储额外伤害值

    // 标记下一次攻击将应用额外伤害 
    Hero* target = dynamic_cast<Hero*>(getNextEnemyHero());
    auto enemyHero = dynamic_cast<Hero*>(target);
    if (enemyHero) {
        // 计算造成的伤害
        float damage = _currentPhysicalAttack + _extraAttackDamage;// 这里简化了伤害计算，实际可能更复杂

        // 应用伤害到目标英雄
        enemyHero->receiveDamage(damage);

        // 播放攻击动画
        playAttackAnimation();
    }
}

GiggsHero::GiggsHero()
    : Hero({ 800, 1440, 2592 }, { 65, 98, 146 }, 40, 40, 0.25, 400, 30, 75, 0.8, Level::Level1) {
    // 初始化 Giggs 特有的状态或其他逻辑
    _hero_number = 7;
    _star = 5;
    _career = 3;
    hero_type = 8;
    _power = _star + int(_level);
}

GiggsHero* GiggsHero::create() {
    GiggsHero* giggs = new (std::nothrow) GiggsHero();
    if (giggs && giggs->initWithFile("heroes/8.png")) { // 假设有 giggs.png 文件
        giggs->autorelease();
        return giggs;
    }
    CC_SAFE_DELETE(giggs);
    return nullptr;
}

void GiggsHero::castSkill() {
    CCLOG("Giggs casts Chaos Theory.");

    // 魔法伤害值根据等级变化
    float magicDamage[3] = { 360, 540, 4000 };
    int levelIndex = static_cast<int>(_level); // 将枚举转换为整数索引

    // 选择当前目标并对其造成伤害
    Hero* target = getNextEnemyHero(); // 获取最近的敌方单位
    if (target) {
        auto enemyHero = dynamic_cast<Hero*>(target);
        if (enemyHero) {
            // 对目标造成魔法伤害
            enemyHero->receiveMagicDamage(magicDamage[levelIndex]);
            // 可以添加其他效果，如炸弹分裂、魔抗击碎等
            // ...
        }
    }
}

JinxHero::JinxHero()
    : Hero({ 450, 810, 1458 }, { 45, 68, 101 }, 15, 15, 0.25, 500, 0, 40, 0.7, Level::Level1) {
    // 初始化 Jinx 特有的状态或其他逻辑
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
    // 实现 Jinx 的技能逻辑：杀死后攻击速度增加
    CCLOG("Jinx activates her skill.");

    increaseAttackSpeed();

    // TODO: 根据游戏的具体需求添加其他技能效果，例如发射大型火箭
}

void JinxHero::increaseAttackSpeed() {
    // 假设技能效果是增加50%的攻击速度，持续5秒
    float originalAttackSpeed = _attackSpeed;
    _attackSpeed *= 1.5; // 增加攻击速度

    // 设置一个延时动作，5秒后恢复原来的攻击速度
    auto delay = DelayTime::create(5.0f);
    auto resetSpeed = CallFunc::create([this, originalAttackSpeed]() {
        _attackSpeed = originalAttackSpeed;
        });

    this->runAction(Sequence::create(delay, resetSpeed, nullptr));
}

KaiSaHero::KaiSaHero()
    : Hero({ 550, 990, 1782 }, { 50, 75, 113 }, 20, 20, 0.25, 500, 0, 60, 0.7, Level::Level1) {
    // 初始化 KaiSa 特有的状态或其他逻辑
    _hero_number = 9;
    _star = 2;
    _career = 2;
    hero_type = 10;
    _power = _star + int(_level);
}

KaiSaHero* KaiSaHero::create() {
    KaiSaHero* kaisa = new (std::nothrow) KaiSaHero();
    if (kaisa && kaisa->initWithFile("heroes/10.png")) { // 假设有 kaisa.png 文件
        kaisa->autorelease();
        return kaisa;
    }
    CC_SAFE_DELETE(kaisa);
    return nullptr;
}

void KaiSaHero::castSkill() {
    CCLOG("KaiSa casts Icathian Rain.");

    // 技能伤害值根据等级变化
    float damagePercent[3] = { 280, 280, 285 };
    int levelIndex = static_cast<int>(_level); // 将枚举转换为整数索引

    // 获取当前攻击目标
    Hero* target = getNextEnemyHero(); // 获取最近的敌方单位
    if (target) {
        // 冲刺至目标（如果距离超过2格，则移动2格距离）
        dashToTarget(target, 2);

        // 对目标发射弹体并造成伤害
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

    // 计算方向和距离
    Vec2 direction = targetPosition - currentPosition;
    float distance = direction.length();

    // 如果距离超过最大冲刺距离，则冲刺最大距离
    if (distance > maxDashDistance) {
        direction.normalize();
        Vec2 dashPosition = currentPosition + direction * maxDashDistance;
        this->setPosition(dashPosition);
    }
    else {
        // 否则，直接冲刺至目标
        this->setPosition(targetPosition);
    }
}

KennenHero::KennenHero() : Hero({ 650, 1170, 2106 }, { 40, 60, 90 }, 40, 40, 0.25,200, 30, 90, 0.6, Level::Level1) {
    // 初始化 Kennen 特有的属性或状态
    _hero_number = 10;
    _star = 1;
    _career = 3;
    hero_type = 11;
    _power = _star + int(_level);
}

KennenHero* KennenHero::create() {
    KennenHero* kennen = new (std::nothrow) KennenHero();
    if (kennen && kennen->initWithFile("heroes/11.png")) { // 假设有 kennen.png 文件
        kennen->autorelease();
        return kennen;
    }
    CC_SAFE_DELETE(kennen);
    return nullptr;
}

void KennenHero::castSkill() {
    // 实现 Kennen 的技能逻辑：震击和震慑
    CCLOG("Kennen casts Shock and Stun.");
    float magicDamage[3] = { 360, 540, 4000 };
    int levelIndex = static_cast<int>(_level); // 将枚举转换为整数索引

    // 选择当前目标并对其造成伤害
    Hero* target = getNextEnemyHero(); // 获取最近的敌方单位
    if (target) {
        auto enemyHero = dynamic_cast<Hero*>(target);
        if (enemyHero) {
            // 对目标造成魔法伤害
            enemyHero->receiveMagicDamage(magicDamage[levelIndex]);
        }
    }
}

KSanteHero::KSanteHero() : Hero({ 550, 990, 1782 }, { 50, 75, 113 }, 25, 25, 0.25, 60, 0, 40, 0.75, Level::Level1) {
    // 初始化 KSante 特有的属性或状态
    _hero_number = 11;
    _star = 1;
    _career = 0;
    hero_type = 12;
    _power = _star + int(_level);
}

KSanteHero* KSanteHero::create() {
    KSanteHero* ksante = new (std::nothrow) KSanteHero();
    if (ksante && ksante->initWithFile("heroes/12.png")) { // 假设有 ksante.png 文件
        ksante->autorelease();
        return ksante;
    }
    CC_SAFE_DELETE(ksante);
    return nullptr;
}

void KSanteHero::castSkill() {
    // 技能开始：进入防御姿态
    CCLOG("KSante enters a defensive stance.");

    // 根据等级设置防御姿态的伤害减免效果
    float damageReduction = (_level == Level::Level1 || _level == Level::Level2) ? 0.50f : 0.55f;

    _armor *= (1 + damageReduction);
    // 技能结束：对当前目标造成大量伤害
    CCLOG("KSante strikes with a powerful blow.");
    Hero* target = getNextEnemyHero(); // 获取最近的敌方单位
    if (target) {
        // 对当前目标造成800%物理伤害
        auto enemyHero = dynamic_cast<Hero*>(target);
        if (enemyHero) {
            enemyHero->receiveDamage(_currentPhysicalAttack * 8.0f);
        }
    };

}
LucianHero::LucianHero()
    : Hero({ 750, 1350, 2430 }, { 75, 113, 169 }, 30, 30, 0.25, 400, 50, 125, 0.9, Level::Level1) {
    // 初始化 Lucian 特有的状态或其他逻辑
    _hero_number = 12;
    _star = 5;
    _career = 2;
    hero_type = 13;
    _power = _star + int(_level);
}

LucianHero* LucianHero::create() {
    LucianHero* lucian = new (std::nothrow) LucianHero();
    if (lucian && lucian->initWithFile("heroes/13.png")) { // 假设有 lucian.png 文件
        lucian->autorelease();
        return lucian;
    }
    CC_SAFE_DELETE(lucian);
    return nullptr;
}

void LucianHero::castSkill() {
    // 实现 Lucian 的技能逻辑：琶音
    CCLOG("Lucian casts The Culling.");

    // 计算发射的子弹数量
    int bulletCount = 14 + static_cast<int>(_attackSpeed / 0.2); // 每20%额外攻速增加1发子弹

    // 计算伤害值
    float damagePercent[3] = { 0.55, 0.55, 10.0 }; // 百分比物理伤害
    int levelIndex = static_cast<int>(_level);
    Hero* target = getNextEnemyHero(); // 获取最近的敌方单位
    if (target) {
        if (isInRange(target)) {
            auto enemyHero = dynamic_cast<Hero*>(target);
            if (enemyHero) {
                // 计算造成的伤害
                float damage = _currentPhysicalAttack * damagePercent[levelIndex]; // 这里简化了伤害计算，实际可能更复杂

                // 应用伤害到目标英雄
                enemyHero->receiveDamage(damage);// 如果在攻击范围内，则攻击
                performRangedAttackAnimation(target);
            }
        }
    }
    
}