
#ifndef  PROJ_ANDROID_REWARDDATA_H
#define  PROJ_ANDROID_REWARDDATA_H

#include "cocos2d.h"
#include <random>

// Return a number from 0-100
int getRandomNumber( ){
    // Static prevents new seeds each call
    static std::mt19937_64 mt_engine{ (unsigned)time(0) };
    static std::uniform_int_distribution<int> range{ 0, 99 };
    return range(mt_engine);
}

void initRandomEngine(){
    static std::mt19937_64 mt_engine{ (unsigned)time(0) };
}


// Data for Bonus Wheel rewards
enum RewardType
{
    Brush,
    Coin,
    Gem,
    Hammer,
    Life
};

std::map<RewardType, std::string> RewardImage{
        {Brush, "brush.png"},
        {Coin, "coin.png"},
        {Gem, "gem.png"},
        {Hammer, "hammer.png"},
        {Life, "heart.png"}
};

struct RewardElement{
    int Sector;
    RewardType type;
    int amount;
    int dropChance;
};

// Actual reward board, change this to change chances and elements
std::vector<RewardElement> RewardBoard{
        {1, Life  ,  30, 20},
        {2, Brush ,   3, 10},
        {3, Gem   ,  35, 10},
        {4, Hammer,   3, 10},
        {5, Coin  , 750,  5},
        {6, Brush ,   1, 20},
        {7, Gem   ,  75,  5},
        {8, Hammer,   1, 20}
};

// Returns the ID for element in RewardBoard
int GetReward() {

    std::vector<int> tmp;

    // Use raffle to pick random element
    for (int i = 0; i < RewardBoard.size(); i++) {
        for (int d = 0; d < RewardBoard[i].dropChance; d++) {
            tmp.push_back(RewardBoard[i].Sector);
        }
    }
    auto index = getRandomNumber() % tmp.size();
    return tmp[index];

}

// Log how many times each sector is selected based
// on input 'amount' a.ka. times GetReward() is called
void DistributionCheck(int amount)
{
    std::map<int, int> distribution;

    for (int i = 0; i < amount; i++)
    {
        int rewardId = GetReward();
        distribution[rewardId] += 1;
    }

    log("DistributionCheck() : You ran GetReward() %d times", amount);
    for (auto elem : distribution) {
        log("Sector : %i , Amount : %i", elem.first, elem.second);
    }
}

#endif //PROJ_ANDROID_REWARDDATA_H
