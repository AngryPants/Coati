// Include MK
#include "MKMissileWarning.h"
#include "../../MK/Actions/MKFollowNodeAction.h"
#include "../../MK/Actions/MKScaleUpAndDownAction.h"
#include "../../MK/Actions/MKRemoveFromParentAction.h"

NS_MK_BEGIN

// Sprite
const mkString MKMissileWarning::MISSILE_WARNING_SPRITE_FILE = "Textures/Gameplay/Obstacles/Missile/Missile_Warning.png";

// Audio
const mkString MKMissileWarning::MISSILE_WARNING_SOUND_NAME = "Missile_Warning";

// Constructor(s) & Destructor
MKMissileWarning::MKMissileWarning()
{
}

MKMissileWarning::~MKMissileWarning()
{
    if (m_FinishCallback)
    {
        m_FinishCallback(this);
    }
}

// Static Function(s)
MKMissileWarning* MKMissileWarning::create(mkF32 _warningDuration, cocos2d::Node* _followNode, const Vec2& _offset, const std::function<void(cocos2d::Node*)>& _finishCallback)
{
    MKMissileWarning* missileWarning = new (std::nothrow) MKMissileWarning();
    if (missileWarning && missileWarning->init(_warningDuration, _followNode, _offset, _finishCallback))
    {
        missileWarning->autorelease();
        return missileWarning;
    }

    CC_SAFE_DELETE(missileWarning);
    return nullptr;
}

// Virtual Override(s)
mkBool MKMissileWarning::init(mkF32 _warningDuration, cocos2d::Node* _followNode, const Vec2& _offset, const std::function<void(cocos2d::Node*)>& _finishCallback)
{
    if (!Super::initSpriteWithFile(MISSILE_WARNING_SPRITE_FILE)) { return false; }

    setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    m_WarningDuration = _warningDuration;
    m_FinishCallback = _finishCallback;

    // Run Action(s)
    // Scale
    auto visibleSize = Director::getInstance()->getVisibleSize();
    mkF32 maxSize = visibleSize.height * 0.06f;
    mkF32 minSize = visibleSize.height * 0.04f;
    MKScaleUpAndDownAction* scaleAction = MKScaleUpAndDownAction::Create(m_WarningDuration, minSize / getContentSize().height, maxSize / getContentSize().height, 0.0f, 10.0f);
    
    // Play Sound
    CallFunc* playSoundCallback = CallFunc::create(
        []() -> void
        {
            // Play Warning Sound
            MKAudioManager::GetInstance()->PlaySound(MISSILE_WARNING_SOUND_NAME, MKSound::SoundType::SFX);
        }
    );

    // Follow
    MKFollowNodeAction* followAction = MKFollowNodeAction::Create(m_WarningDuration, _followNode, MKFollowNodeAction::ALL, _offset);

    runAction(
        Spawn::createWithTwoActions(
            Sequence::create(scaleAction, playSoundCallback, MKRemoveFromParentAction::Create(), nullptr),
            followAction
        )
    );

    return true;
}

NS_MK_END