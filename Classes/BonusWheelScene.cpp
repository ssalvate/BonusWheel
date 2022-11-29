#include "BonusWheelScene.h"
#include "RewardData.h"

USING_NS_CC;

Scene* BonusWheel::createScene()
{
    return BonusWheel::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool BonusWheel::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    mScreenMiddle = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 );
    LoadGameObjects();

    return true;

}

void BonusWheel::LoadGameObjects()
{
    mHasPlayed = false;

    // Create node in center of screen
    // Attach sprite images to node
    mWheelNode = Node::create();
    mWheelNode->setPosition(Vec2(mScreenMiddle.x, mScreenMiddle.y));
    mWheelNode->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(mWheelNode);

    // Inner part of wheel with divisions
    mWheelInner = Sprite::create("wheel_sections_8.png");
    mWheelInner->setScale(0.7f);
    mWheelInner->setAnchorPoint(Vec2(0.5, 0.5));
    mWheelNode->addChild(mWheelInner, 0);

    // Wheel Border
    // --add to scene so you can rotate wheel node and children
    mWheelBorder = Sprite::create("wheel_border.png");
    mWheelBorder->setAnchorPoint(Vec2(0.5, 0.5));
    mWheelBorder->setPosition(mWheelNode->getPosition());
    mWheelBorder->setScale(0.7f);
    this->addChild(mWheelBorder, 1);

    // Arrow Image
    mArrow = Sprite::create("wheel_arrow.png");
    mArrow->setPosition(
            Vec2(mWheelNode->getPosition().x,
                 mWheelNode->getPosition().y+ 250)
    );
    this->addChild(mArrow,2);

    // Play On Button
    mPlayButton = ui::Button::create("spin_button.png");
    mPlayButton->setScale(0.6f);
    mPlayButton->setAnchorPoint(Vec2(0.5, .5));
    mPlayButton->setPosition(
            Vec2(mWheelNode->getPosition().x,
                 mWheelNode->getPosition().y-360.0f)
    );
    mPlayButton->addClickEventListener(CC_CALLBACK_0(BonusWheel::OnPlayButton, this));
    this->addChild(mPlayButton);

    // Set special text label for looks
    SetButtonLabel("Play On", Color4B::BLACK);

    // Create reward sprites and align them to inner wheel sectors
    int radius = 180;
    for (const RewardElement& reward : RewardBoard)
    {
        auto s = Sprite::create(RewardImage[reward.type]);
        s->setScale(0.7f);
        s->setAnchorPoint(Vec2(0.5, 0.5));
        // Set Label before moving and rotating sprite
        SetRewardLabel(s, reward);
        // Angle = 360/8-rewards * which half step
        // Reward.Sector doubling as index in loop(-1)
        auto angle = (45 * (0.5 + reward.Sector - 1));
        s->setRotation(angle);
        // image offset + Radius * sin/cos(offset - angle)
        // x,y relative to center of node
        auto x = radius * cos(MATH_DEG_TO_RAD(90 - angle));
        auto y = radius * sin(MATH_DEG_TO_RAD(90 - angle));
        s->setPosition(x, y);

        mRewardIcons.push_back(s);
        mWheelNode->addChild(s);
    }
}

void BonusWheel::SetRewardLabel(Sprite* sp, const RewardElement& reward)
{
    if (reward.type == Life)
    {
        auto p = Vec2(sp->getPosition().x + sp->getContentSize().width / 2,
                      sp->getPosition().y);
        SetIconLabel(sp, std::to_string(reward.amount), p, 48.0f);
        auto subPos = Vec2(sp->getPosition().x + sp->getContentSize().width / 2 + 25,
                           sp->getPosition().y - 32);
        SetIconLabel(sp, "min" , subPos, 32.0f, 2);
    }
    else
    {
        auto p = Vec2(sp->getPosition().x + sp->getContentSize().width / 2,
                      sp->getPosition().y);
        SetIconLabel(sp, std::to_string(reward.amount) + "X", p, 48.0f);
    }
}

void BonusWheel::OnPlayButton()
{
    log("On button click");

    if (!mHasPlayed) {
        mHasPlayed = true;

        //DistributionCheck(1000);

        int sector = GetReward();
        // Since it spins counterclockwise flip degrees( -360)
        float goalAngle = (45 * (0.5 + sector - 1)) - 360;
        log("Goal sector: %d--Angle: %f", sector, goalAngle);

        // Actions for board
        int fullLoops = 3;
        //auto rotateBy = RotateBy::create(2.0f, 360.0f * 3);
        mWheelNode->runAction(Sequence::create(
                EaseSineOut::create(RotateBy::create(0.5f, -10)),
                EaseSineOut::create(RotateBy::create(3.0f, 360 * fullLoops + 10 - goalAngle)),
                DelayTime::create(0.25f),
                CallFunc::create([&, sector] { ShowReward(sector); }),
                nullptr
        ));

        // Actions for button
        auto scaleDown = ScaleBy::create(0.1, 0.1f);
        auto showBtn = CallFunc::create([&] {
            mPlayButton->setVisible(true);
        });
        auto hideBtn = CallFunc::create([&] {
            mPlayButton->setVisible(false);
        });
        auto delay = DelayTime::create(4.0f);

        auto s = Sequence::create(scaleDown, hideBtn, delay,
                                  scaleDown->reverse(), showBtn, nullptr);

        mPlayButton->runAction(s);
        SetButtonLabel("Claim");
    }
    else
    {
        RefreshScene();
    }
}

void BonusWheel::ShowReward(int sector) {

    // For actual reward type and value grab RewardBoard[sector-1]

    // Get the reward icon
    log("Reward Sector: %d", sector);
    auto rewardIcon = mRewardIcons[sector - 1];
    for (auto icon : mRewardIcons)
    {
        if (icon != rewardIcon)icon->setVisible(false);
    }

    // Reward Icon Actions
    auto ScaleUp = ScaleBy::create(0.5f, 2.0f);
    auto MoveTo = MoveTo::create(0.4f, Vec2(0.0f, 0.0f));
    auto easeMoveTo = EaseOut::create(MoveTo, .2f);

    // Hide other elements
    mWheelBorder->setVisible(false);
    mWheelInner->setVisible(false);
    mArrow->setVisible(false);

    auto s = Spawn::create(ScaleUp, easeMoveTo, nullptr);
    // Save position to reset
    mRR.pos = rewardIcon->getPosition();
    mRR.sp = rewardIcon;

    rewardIcon->runAction(s);
}

void BonusWheel::RefreshScene()
{
    mWheelNode->setRotation(0.0f);
    mWheelBorder->setVisible(true);
    mWheelInner->setVisible(true);
    mArrow->setVisible(true);
    for (auto icon : mRewardIcons) {
        if (icon == mRR.sp) {
            icon->setPosition(mRR.pos);
            icon->setScale(0.7f);
        }
        icon->setVisible(true);
    }
    SetButtonLabel("Play On");
    mHasPlayed = false;
}

