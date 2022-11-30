
#ifndef  BONUSWHEEL_CLASSES_REWARDDATA_H_
#define  BONUSWHEEL_CLASSES_REWARDDATA_H_

#include "cocos2d.h"
#include <random>

// Return a float value from [0,max)
float getRandomNumber(float max ){
    // Static prevents new seeds each call
    static std::mt19937_64 mt_engine{ (unsigned)time(0) };
    static std::uniform_real_distribution<float> range{ 0, max };
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
    float dropChance;
};

// Actual reward board, change this to change chances and elements
std::vector<RewardElement> RewardBoard{
        {1, Life  ,  30, 22.5f},
        {2, Brush ,   3, 10.0f},
        {3, Gem   ,  35, 10.0f},
        {4, Hammer,   3, 10.0f},
        {5, Coin  , 750,  5.0f},
        {6, Brush ,   1, 20.0f},
        {7, Gem   ,  75,  5.0f},
        {8, Hammer,   1, 18.5f}
};

// Returns the ID for element in RewardBoard
int GetReward() {

    std::vector<float> tmp;

    // Accumulate drop chances
    float max_chance = RewardBoard[0].dropChance;
    tmp.push_back(max_chance);
    for (int i = 1; i < RewardBoard.size(); i++) {
        max_chance += RewardBoard[i].dropChance;
        tmp.push_back(max_chance);
    }
    // Get first element that encapsulates randNum
    float ranNum = getRandomNumber(max_chance);
    for (int index = 0; index < tmp.size(); index++ ){
        // +1 to index to align with Id values/sectors
        if (tmp[index] > ranNum) return index+1;
    }
    return tmp.size();
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

#endif // BONUSWHEEL_CLASSES_REWARDDATA_H_
