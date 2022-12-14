

#ifndef  BONUSWHEEL_CLASSES_BONUSWHEELSCENE_H_
#define  BONUSWHEEL_CLASSES_BONUSWHEELSCENE_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class BonusWheel : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void OnPlayButton();

    void LoadGameObjects();

    void SetRewardLabel(Sprite* sp, const class RewardElement& reward);

    void ShowReward(int sector);

    void RefreshScene();

    void SetButtonLabel(std::string text, Color4B color = Color4B::BLACK) {
        mPlayButton->removeAllChildren();
        Label* textLabel = Label::createWithTTF(text, "fonts/arial.ttf", 64.0f);
        textLabel->enableOutline(color, 2);
        textLabel->setAnchorPoint(Vec2(0.5, 0.5));
        textLabel->setPosition(
                Vec2(mPlayButton->getContentSize().width / 2,
                     mPlayButton->getContentSize().height / 2
                )
        );
        mPlayButton->addChild(textLabel);
    }

    void SetIconLabel(Sprite* sp, std::string text, Vec2 pos, float size, int b_width = 3) {
        Label* textLabel = Label::createWithTTF(text, "fonts/arial.ttf", size);
        textLabel->enableOutline(Color4B(139, 0, 0,255), b_width);
        textLabel->setPosition(pos);
        sp->addChild(textLabel);
    }

    // implement the "static create()" method manually
    CREATE_FUNC(BonusWheel);

    // Use this to reset reward icon that has actions done to it
    struct ResetReward
    {
        Vec2 pos{};
        Sprite* sp = nullptr;
    };

private:
    // Member vars with in-class initialization
    std::vector<Sprite*> mRewardIcons{};
    Vec2 mScreenMiddle{};
    bool mHasPlayed = false;
    Node* mWheelNode = nullptr;
    Node* mButtonNode = nullptr;
    Sprite* mWheelInner = nullptr;
    Sprite* mWheelBorder = nullptr;
    Sprite* mArrow = nullptr;
    ui::Button* mPlayButton = nullptr;

    ResetReward mRR{}; //For reset purposes
};

#endif // BONUSWHEEL_CLASSES_BONUSWHEELSCENE_H_
