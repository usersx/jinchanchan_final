#include "PlayerManager.h"
#include"Player/AIsystem_clever.h"

PlayerManager* PlayerManager::instance = nullptr;

PlayerManager* PlayerManager::getInstance() {
    if (!instance) {
        instance = new PlayerManager();
    }
    return instance;
}

PlayerManager::PlayerManager() {
    // 初始化玩家列表
#ifdef eightbattle
    initPlayerList(8);
#endif // eightbattle

#define doublebattle
#ifdef doublebattle
    initPlayerList(2);
#endif // doublebattle

    
    setIsShowing(false);
}

Player* PlayerManager::getPlayerByID(int playerID)
{
    for (Player* player : playerList)
    {
        if (player->getPlayerId() == playerID)
        {
            CCLOG("finded!");
            return player;
        }
    }
    CCLOG("none finded!");
    return nullptr;
}

void PlayerManager::initPlayerList(int PlayerNum)
{
    for (int i = 0; i < PlayerNum; i++) {
        playerList.pushBack(createPlayer(i,"PlayerWalk4.png"));
        CCLOG("玩家%d已初始化", i);
    }
}

Player* PlayerManager::createPlayer(int PlayerID,const std::string& Playerfilename)
{
    if (PlayerID == 0) {
        Player* player = Player::create(Playerfilename);

        player->setPlayerId(PlayerID);

        return player;
    }
    else {
        AI_clever* AIPl = AI_clever::create(Playerfilename);

        AIPl->setPlayerId(PlayerID);

        AIPl->setIsAI(true);

        return AIPl;
    }
}

void PlayerManager::updatePlayerList()
{
    for (Player* player : playerList)
    {
        player->setBlood(100);

        player->getHeroList().clear();

        player->setMoney(20);

        
    }
}