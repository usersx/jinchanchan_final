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
	//static const char* PLAYER_ID_KEY; // ���������ڴ洢���ID�ļ�
	Player() = default;
	~Player() = default;
	bool init(const std::string& filename);//�����ʼ��
	void initData();//��ɫ���ݳ�ʼ��
	
	CC_SYNTHESIZE(int, blood, Blood);//Ѫ��
	CC_SYNTHESIZE(float, move_rate, MoveRate);//�ƶ��ٶ�
	CC_SYNTHESIZE(bool, facingRight, IsFacingRight);// ��¼����true��ʾ���ң�false��ʾ����
	
	CC_SYNTHESIZE(bool, isDead, IsDead);//��ǰ����Ƿ�����
	CC_SYNTHESIZE(bool, isEnemy, IsEnemy);//�ж��Ƿ�Ϊ���ˣ���Ҫ���ھ���
	CC_SYNTHESIZE(int, playerId, PlayerId);//��ҵ�id
	CC_SYNTHESIZE(int, money, Money);//�����
	CC_SYNTHESIZE(int, level, Level);//��ҵȼ�
	CC_SYNTHESIZE(Vector<Hero*>, heroList, HeroList);//Ӣ���б�
	CC_SYNTHESIZE(bool, isRoundLose, IsRoundLose);//�жϸ���ҵ�ǰ�غ��Ƿ�ս�ܣ�ÿ�غ�һ��ʼ��Ϊfalse
	CC_SYNTHESIZE(bool, isAI, IsAI);//�Ƿ���AI���	
	CC_SYNTHESIZE(bool, isInaRound, IsInaRound);//�Ƿ��Ѿ�ƥ��
	CC_SYNTHESIZE(bool, isInBattle, IsInBattle);//�Ƿ�����ս���������ж�����Ƿ�����ƶ�����Ӣ��
	CC_SYNTHESIZE(int, round, Round);//ÿ����Ҹ��ԵĻغ���

	CC_SYNTHESIZE(EquipmentBarforGame*, _the_bar, TheBar);
	virtual bool onTouchBegan(Touch* touch, Event* event);

	Animation* getAnimation();//��ȡ���߶���


	float getTimetoDest(Vec2 pt);//��ȡ�ƶ���Ŀ���ʱ��

	void needFlip(float mousePosX);//�ж��Ƿ���Ҫת��

	void flipDirection();//Ӣ��ת��

	void playWalkAnimationAndMove(const Vec2& destination, float duration);//�������߶������ƶ�

	void setHealthBar(HealthBar* healthBar);//��ʼ��Ѫ��

	void decreaseBlood(int decreaseNum);//����Ѫ��

	void moneychange();//�غϽ���ʱ�Ľ�Ҹı�

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
