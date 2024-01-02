#pragma once
#ifndef PLAYERBASE_H_
#define PLAYERBASE_H_

#include"cocos2d.h"
#include"BaseSprite/BaseSprite.h"
#include"HealthBar/HealthBar.h"
#include"Hero/HeroNew.h"
#include"Equipment/Equipment.h"
//#include "Scene/Battle.h"

USING_NS_CC;

struct PlayerHealthEventData{
	int playerId;
	int health;
};

class Player :public BaseSprite {
public:
	static Player* create(const std::string& filename);

	//static const char* HEALTH_CHANGED_EVENT;
	//static const char* PLAYER_ID_KEY; // 键名，用于存储玩家ID的键
	Player() = default;
	~Player() = default;
	bool init(const std::string& filename);//精灵初始化
	void initData();//角色数据初始化
	
	CC_SYNTHESIZE(int, blood, Blood);//血量
	CC_SYNTHESIZE(float, move_rate, MoveRate);//移动速度
	CC_SYNTHESIZE(bool, facingRight, IsFacingRight);// 记录朝向，true表示朝右，false表示朝左
	
	CC_SYNTHESIZE(bool, isDead, IsDead);//当前玩家是否阵亡
	CC_SYNTHESIZE(bool, isEnemy, IsEnemy);//判断是否为敌人，主要用于镜像
	CC_SYNTHESIZE(int, playerId, PlayerId);//玩家的id
	CC_SYNTHESIZE(int, money, Money);//金币数
	CC_SYNTHESIZE(int, level, Level);//玩家等级
	CC_SYNTHESIZE(Vector<Hero*>, heroList, HeroList);//英雄列表
	CC_SYNTHESIZE(bool, isRoundLose, IsRoundLose);//判断该玩家当前回合是否战败，每回合一开始均为false
	CC_SYNTHESIZE(bool, isAI, IsAI);//是否是AI玩家	
	CC_SYNTHESIZE(bool, isInaRound, IsInaRound);//是否已经匹配
	CC_SYNTHESIZE(bool, isInBattle, IsInBattle);//是否正在战斗，用于判断玩家是否可以移动场上英雄
	CC_SYNTHESIZE(int, round, Round);//每个玩家各自的回合数

	CC_SYNTHESIZE(EquipmentBarforGame*, _the_bar, TheBar);
	virtual bool onTouchBegan(Touch* touch, Event* event);

	Animation* getAnimation();//获取行走动作


	float getTimetoDest(Vec2 pt);//获取移动到目标的时间

	void needFlip(float mousePosX);//判断是否需要转向

	void flipDirection();//英雄转向

	void playWalkAnimationAndMove(const Vec2& destination, float duration);//发生行走动作和移动

	void setHealthBar(HealthBar* healthBar);//初始化血条

	void decreaseBlood(int decreaseNum);//减少血量

	void moneychange();//回合结算时的金币改变

	HealthBar* healthBar;

	void player_push_back_herolist(int key);
	

	//Vector<Hero*> heroList;

	//Vector<Equipment*> equipmentList;

	//Layer* HeroLayer;

	//Scene* myBattleField;

	/*static bool compareByHealth(const Player& player1, const Player& player2) {
		return player1.getBlood() < player2.getBlood();
	}*/

};





#endif // !PLAYERBASE_H_
