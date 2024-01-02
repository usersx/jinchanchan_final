#pragma once
#ifndef __PLAYER_MANAGER_H__
#define __PLAYER_MANAGER_H__

#include "cocos2d.h"
#include"Player/PlayerBase.h"
#include<string>

USING_NS_CC;

class PlayerManager {
public:
    // 获取单例实例
    static PlayerManager* getInstance();

private:
    // 单例实例
    static PlayerManager* instance;
    
    // 玩家信息
    CC_SYNTHESIZE(Vector<Player*>, playerList, PlayerList);
    CC_SYNTHESIZE(bool, isShowing, IsShowing);

    //创建玩家列表
    void initPlayerList(int PlayerNum);

    //创建单个玩家对象
    Player* createPlayer(int PlayerID, const  std::string& Playerfilename);

    //通过玩家ID获取Player指针
    Player* getPlayerByID(int playerID);

    //刷新玩家
    void updatePlayerList();

    // 私有构造函数，确保单例
    PlayerManager();

    

    // 禁用拷贝构造函数和赋值运算符
    PlayerManager(const PlayerManager&) = delete;
    PlayerManager& operator=(const PlayerManager&) = delete;
};

#endif // __PLAYER_MANAGER_H__
