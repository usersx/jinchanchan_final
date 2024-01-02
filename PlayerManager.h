#pragma once
#ifndef __PLAYER_MANAGER_H__
#define __PLAYER_MANAGER_H__

#include "cocos2d.h"
#include"Player/PlayerBase.h"
#include<string>

USING_NS_CC;

class PlayerManager {
public:
    // ��ȡ����ʵ��
    static PlayerManager* getInstance();

private:
    // ����ʵ��
    static PlayerManager* instance;
    
    // �����Ϣ
    CC_SYNTHESIZE(Vector<Player*>, playerList, PlayerList);
    CC_SYNTHESIZE(bool, isShowing, IsShowing);

    //��������б�
    void initPlayerList(int PlayerNum);

    //����������Ҷ���
    Player* createPlayer(int PlayerID, const  std::string& Playerfilename);

    //ͨ�����ID��ȡPlayerָ��
    Player* getPlayerByID(int playerID);

    //ˢ�����
    void updatePlayerList();

    // ˽�й��캯����ȷ������
    PlayerManager();

    

    // ���ÿ������캯���͸�ֵ�����
    PlayerManager(const PlayerManager&) = delete;
    PlayerManager& operator=(const PlayerManager&) = delete;
};

#endif // __PLAYER_MANAGER_H__
