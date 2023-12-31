#pragma once
#ifndef __HERO_H_
#define __HERO_H_
#include"cocos2d.h"
#include"HealthBar/HealthBar.h"
#include<vector>
#include"BaseSprite/BaseSprite.h"

#include"Equipment/Equipment.h"


USING_NS_CC;

const int square_width = 74.0f;//�����С
const int square_height = 60.0f;//�����С

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
//    // �����������Ӣ��...
//}


class Hero : public BaseSprite 
{
public:
	enum class Level { Level1, Level2, Level3 };
    // ���캯��
    Hero(const std::vector<float>& health, const std::vector<float>& physicalAttack,
        float armor, float magicResist, float critRate, float attackRange,
        int initialMana, int mana, float attackSpeed,
        Level level = Level::Level1);
    Hero();
    bool init();
    // ����Ӣ�۵ľ�̬��������
    //static Hero* Hero::create(const std::string& filename);
    static Hero* create(const std::string& filename,
        const std::vector<float>& health, const std::vector<float>& physicalAttack,
        float armor, float magicResist, float critRate, float attackRange,
        float initialMana, float mana, float attackSpeed,
        Level level = Level::Level1);
    // Ӣ�۵Ļ�����Ϊ�ͼ���
    virtual void castSkill() = 0;

    // ����Ӣ��
    void upgrade();
    void setBloodAndMana();

    void update(float dt); // ÿ֡����Ӣ��״̬
    void moveToTarget(Hero* target); // �ƶ���Ŀ��
    void playAttackAnimation();
    void performRangedAttackAnimation(Hero* target);
    void attackTarget(Hero* target); // ����Ŀ��
    void receiveDamage(float damage);//����
    void onDeath();
    bool isInRange(Hero* target); // �ж�Ŀ���Ƿ��ڹ�����Χ��
    bool isEnemy(Hero* hero);
    //Ѱ����һ��Ŀ��
    Hero* getNextEnemyHero();
    //virtual void moveToNextActor(Hero* actor);
    float getDistance(Hero* actor);
    bool isEnd();

    //����Ӣ��״̬
    void updateAttributes();
    void receiveMagicDamage(float damage);

    void put_on_field(std::pair<int, int> pair_position);//���������������

    bool take_equip(Equipment* equip);
    bool unload_equip();

    ~Hero();

protected:
    std::vector<float> _health;
    std::vector<float> _physicalAttack;
    CC_SYNTHESIZE(int, _hero_number, HeroNumber);//���
    CC_SYNTHESIZE(int, _star, Star);//�Ǽ�
    CC_SYNTHESIZE(int, _career, Career);//ְҵ
    CC_SYNTHESIZE(int, _power, Power);//Ȩֵ
    CC_SYNTHESIZE(int, _currentHealth, CurrentHealth);//ĿǰѪ��
    CC_SYNTHESIZE(int, _currentPhysicalAttack, CurrentPhysicalAttack);//��ǰ������
    CC_SYNTHESIZE(float, _armor, Armor);
    CC_SYNTHESIZE(float, _magicResist, MagicResist);//����
    CC_SYNTHESIZE(float, _critRate, CritRate);//
    CC_SYNTHESIZE(float, _attackRange, AttackRange);//������Χ
    CC_SYNTHESIZE(int, _initialMana, InitialMana);//�������
    CC_SYNTHESIZE(int, _mana, Mana);//����
    CC_SYNTHESIZE(float, _attackSpeed, AttackSpeed);//�����ٶ�
    CC_SYNTHESIZE(Level, _level, Level);//�ȼ�
    CC_SYNTHESIZE(float, _moveRate, MoveRate);//����
    CC_SYNTHESIZE(Vec2, _originalPos, OriginalPos);//λ��
    CC_SYNTHESIZE(Vec2, _battlePosition, BattlePosition);//ս��ԭλ��

    CC_SYNTHESIZE(int, _team, Team);//��Ӫ
    CC_SYNTHESIZE(int, _enemyTeam, enemyTeam);//�з���Ӫ

    CC_SYNTHESIZE(HealthBar*, bloodBar, BloodBar);//Ѫ����ʾ
    CC_SYNTHESIZE(HealthBar*, manaBar, ManaBar);//������ʾ

    CC_SYNTHESIZE(bool, isOnTheField, IsOnTheField);
    CC_SYNTHESIZE(bool, isDead, IsDead);
    CC_SYNTHESIZE(int, hero_type, HeroType);//Ӣ������,���಻��ʼ��
    CC_SYNTHESIZE(EquipmentBarforHero*, _bar, Bar);

};

class AkaliHero : public Hero {
public:
    AkaliHero();
    
    static AkaliHero* create();

    virtual void castSkill() /*override*/; // ʵ�� Akali �����⼼��

    Hero* getFarthestEnemy();
};
class AnnieHero : public Hero {
public:
    AnnieHero();

    static AnnieHero* create();

    virtual void castSkill() override; // ʵ�� Annie �����⼼��
private:
    int _skillCastCount; // ��¼�����ͷŴ���
    void applyPassive(); // Ӧ�ñ���Ч��
};
class AriHero : public Hero {
public:
    AriHero();

    static AriHero* create();

    virtual void castSkill() override; // ʵ�� Ari �����⼼��
    void dealMagicDamageToTarget(Hero* target, float damage);
};
class CatalinaHero : public Hero {
public:
    CatalinaHero();

    static CatalinaHero* create();

    virtual void castSkill() override; // ʵ�� Catalina �����⼼��
};
class EvelynnHero : public Hero {
public:
    EvelynnHero();

    static EvelynnHero* create();

    virtual void castSkill() override; // ʵ�� Evelynn �����⼼��

private:
    int _skillCastCount; // ��¼�����ͷŴ���
};
class EzrealHero : public Hero {
public:
    EzrealHero();

    static EzrealHero* create();

    virtual void castSkill() override; // ʵ�� Ezreal �����⼼��

private:
    int _skillCastCount; // ��¼�����ͷŴ���
};
class GalenHero : public Hero {
public:
    GalenHero();

    static GalenHero* create();

    virtual void castSkill() override; // ʵ�� Galen �����⼼��
};
class GiggsHero : public Hero {
public:
    GiggsHero();

    static GiggsHero* create();

    virtual void castSkill() override; // ʵ�� Giggs �����⼼��
};
class JinxHero : public Hero {
public:
    JinxHero();

    static JinxHero* create();

    virtual void castSkill() override;// ��д����ļ��ܷ���
    void JinxHero::increaseAttackSpeed();
};
class KaiSaHero : public Hero {
public:
    KaiSaHero();

    static KaiSaHero* create();

    virtual void castSkill() override; // ʵ�� KaiSa �����⼼��
    void dashToTarget(Hero* target, float maxDashDistance);
};
class KennenHero : public Hero {
public:
    KennenHero();

    static KennenHero* create();

    virtual void castSkill() override; // ʵ�� Kennen �����⼼��
};
class KSanteHero : public Hero {
public:
    KSanteHero();

    static KSanteHero* create();

    virtual void castSkill() override; // ʵ�� KSante �����⼼��
};
class LucianHero : public Hero {
public:
    LucianHero();

    static LucianHero* create();

    virtual void castSkill() override; // ʵ�� Lucian �����⼼��
};

#endif // __HERO_H_
