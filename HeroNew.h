#pragma once
#ifndef __HERO_H_
#define __HERO_H_
#include"cocos2d.h"
#include"HealthBar/HealthBar.h"
#include<vector>
#include"BaseSprite/BaseSprite.h"

#include"Equipment/Equipment.h"


USING_NS_CC;

const int square_width = 74.0f;//方格大小
const int square_height = 60.0f;//方格大小

const std::vector<int> equipment_power = {
    1,1,1,1,1,
    2,2,2,2,2,
    2,2,2,2,2,
    2,2,2,2,2,
    2,2,2,2,2
};



//std::unordered_map<int, Hero*>match;
//void initializeMatch(std::unordered_map<int, Hero*>& match) {
//    match[1] = AkaliHero::create();
//    match[2] = AnnieHero::create();
//    match[3] = AriHero::create();
//    match[4] = CatalinaHero::create();
//    match[5] = EvelynnHero::create();
//    match[6] = EzrealHero::create();
//    match[7] = GalenHero::create();
//    match[8] = GiggsHero::create();
//    match[9] = JinxHero::create();
//    match[10] = KaiSaHero::create();
//    match[11] = KennenHero::create();
//    match[12] = KSanteHero::create();
//    match[13] = LucianHero::create();
//    // 继续添加其他英雄...
//}


class Hero : public BaseSprite 
{
public:
	enum class Level { Level1, Level2, Level3 };
    // 构造函数
    Hero(const std::vector<float>& health, const std::vector<float>& physicalAttack,
        float armor, float magicResist, float critRate, float attackRange,
        int initialMana, int mana, float attackSpeed,
        Level level = Level::Level1);
    Hero();
    bool init();
    // 创建英雄的静态工厂方法
    //static Hero* Hero::create(const std::string& filename);
    static Hero* create(const std::string& filename,
        const std::vector<float>& health, const std::vector<float>& physicalAttack,
        float armor, float magicResist, float critRate, float attackRange,
        float initialMana, float mana, float attackSpeed,
        Level level = Level::Level1);
    // 英雄的基础行为和技能
    virtual void castSkill() = 0;

    // 升级英雄
    void upgrade();
    void setBloodAndMana();

    void update(float dt); // 每帧更新英雄状态
    void moveToTarget(Hero* target); // 移动到目标
    void playAttackAnimation();
    void performRangedAttackAnimation(Hero* target);
    void attackTarget(Hero* target); // 攻击目标
    void receiveDamage(float damage);//受伤
    void onDeath();
    bool isInRange(Hero* target); // 判断目标是否在攻击范围内
    bool isEnemy(Hero* hero);
    //寻找下一个目标
    Hero* getNextEnemyHero();
    //virtual void moveToNextActor(Hero* actor);
    float getDistance(Hero* actor);
    bool isEnd();

    //更新英雄状态
    void updateAttributes();
    void receiveMagicDamage(float damage);

    void put_on_field(std::pair<int, int> pair_position);//按照棋盘坐标放置

    bool take_equip(Equipment* equip);
    bool unload_equip();

    ~Hero();

protected:
    std::vector<float> _health;
    std::vector<float> _physicalAttack;
    CC_SYNTHESIZE(int, _hero_number, HeroNumber);//编号
    CC_SYNTHESIZE(int, _star, Star);//星级
    CC_SYNTHESIZE(int, _career, Career);//职业
    CC_SYNTHESIZE(int, _power, Power);//权值
    CC_SYNTHESIZE(int, _currentHealth, CurrentHealth);//目前血量
    CC_SYNTHESIZE(int, _currentPhysicalAttack, CurrentPhysicalAttack);//当前物理攻击
    CC_SYNTHESIZE(float, _armor, Armor);
    CC_SYNTHESIZE(float, _magicResist, MagicResist);//法抗
    CC_SYNTHESIZE(float, _critRate, CritRate);//
    CC_SYNTHESIZE(float, _attackRange, AttackRange);//攻击范围
    CC_SYNTHESIZE(int, _initialMana, InitialMana);//最初蓝量
    CC_SYNTHESIZE(int, _mana, Mana);//蓝量
    CC_SYNTHESIZE(float, _attackSpeed, AttackSpeed);//攻击速度
    CC_SYNTHESIZE(Level, _level, Level);//等级
    CC_SYNTHESIZE(float, _moveRate, MoveRate);//移速
    CC_SYNTHESIZE(Vec2, _originalPos, OriginalPos);//位置
    CC_SYNTHESIZE(Vec2, _battlePosition, BattlePosition);//战斗原位置

    CC_SYNTHESIZE(int, _team, Team);//阵营
    CC_SYNTHESIZE(int, _enemyTeam, enemyTeam);//敌方阵营

    CC_SYNTHESIZE(HealthBar*, bloodBar, BloodBar);//血条显示
    CC_SYNTHESIZE(HealthBar*, manaBar, ManaBar);//蓝条显示

    CC_SYNTHESIZE(bool, isOnTheField, IsOnTheField);
    CC_SYNTHESIZE(bool, isDead, IsDead);
    CC_SYNTHESIZE(int, hero_type, HeroType);//英雄类型,基类不初始化
    CC_SYNTHESIZE(EquipmentBarforHero*, _bar, Bar);

};

class AkaliHero : public Hero {
public:
    AkaliHero();
    
    static AkaliHero* create();

    virtual void castSkill() /*override*/; // 实现 Akali 的特殊技能

    Hero* getFarthestEnemy();
};
class AnnieHero : public Hero {
public:
    AnnieHero();

    static AnnieHero* create();

    virtual void castSkill() override; // 实现 Annie 的特殊技能
private:
    int _skillCastCount; // 记录技能释放次数
    void applyPassive(); // 应用被动效果
};
class AriHero : public Hero {
public:
    AriHero();

    static AriHero* create();

    virtual void castSkill() override; // 实现 Ari 的特殊技能
    void dealMagicDamageToTarget(Hero* target, float damage);
};
class CatalinaHero : public Hero {
public:
    CatalinaHero();

    static CatalinaHero* create();

    virtual void castSkill() override; // 实现 Catalina 的特殊技能
};
class EvelynnHero : public Hero {
public:
    EvelynnHero();

    static EvelynnHero* create();

    virtual void castSkill() override; // 实现 Evelynn 的特殊技能

private:
    int _skillCastCount; // 记录技能释放次数
};
class EzrealHero : public Hero {
public:
    EzrealHero();

    static EzrealHero* create();

    virtual void castSkill() override; // 实现 Ezreal 的特殊技能

private:
    int _skillCastCount; // 记录技能释放次数
};
class GalenHero : public Hero {
public:
    GalenHero();

    static GalenHero* create();

    virtual void castSkill() override; // 实现 Galen 的特殊技能
};
class GiggsHero : public Hero {
public:
    GiggsHero();

    static GiggsHero* create();

    virtual void castSkill() override; // 实现 Giggs 的特殊技能
};
class JinxHero : public Hero {
public:
    JinxHero();

    static JinxHero* create();

    virtual void castSkill() override;// 重写基类的技能方法
    void JinxHero::increaseAttackSpeed();
};
class KaiSaHero : public Hero {
public:
    KaiSaHero();

    static KaiSaHero* create();

    virtual void castSkill() override; // 实现 KaiSa 的特殊技能
    void dashToTarget(Hero* target, float maxDashDistance);
};
class KennenHero : public Hero {
public:
    KennenHero();

    static KennenHero* create();

    virtual void castSkill() override; // 实现 Kennen 的特殊技能
};
class KSanteHero : public Hero {
public:
    KSanteHero();

    static KSanteHero* create();

    virtual void castSkill() override; // 实现 KSante 的特殊技能
};
class LucianHero : public Hero {
public:
    LucianHero();

    static LucianHero* create();

    virtual void castSkill() override; // 实现 Lucian 的特殊技能
};

#endif // __HERO_H_
