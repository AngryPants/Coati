#ifndef MK_PLAYER_H
#define MK_PLAYER_H

// Include MK
#include "../../MK/Common/MKMacros.h"
#include "../../MK/Sprite/SpriteAnimation/MKSpriteAnimation.h"
#include "../../MK/Input/MKInputMacros.h"
#include "../../MK/Actions/MKMoveBySinCurveAction.h"
#include "../../MK/Gameplay/MKGameplayMacros.h"
#include "../../MK/SceneManagement/MKScene.h"
#include "../Powerups/MKShield.h"

// Include STL
#include <functional>

NS_MK_BEGIN

// Forward Declaration(s)
class MKHeroShopItem;

class MKPlayer : public MKSpriteAnimation
{
    typedef MKSpriteAnimation Super;

protected:
    // Action(s) (Hold a pointer to the actions so that we can stop the actions halfway through its excecution.)
    cocos2d::Action* m_JumpAction = nullptr;
    cocos2d::Action* m_SlideAction = nullptr;
    cocos2d::Action* m_RunAction = nullptr;
    cocos2d::Action* m_DieAction = nullptr;

    // Physics Shapes
    PhysicsShape* m_RunPhysicsShape = nullptr;
    PhysicsShape* m_JumpPhysicsShape = nullptr;
    PhysicsShape* m_SlidePhysicsShape = nullptr;

    // Variable(s)
    mkF32 m_JumpHeight = 0.0f;
    mkF32 m_FloorHeight = 0.0f;
    mkF32 m_RunHeight = 0.0f;
    mkF32 m_HorizontalVelocity = 0.0f;
    mkF32 m_DownwardsVelocity = 0.0f;
    MKScene* m_Scene = nullptr;
    mkF32 m_Score = 0;
    mkBool m_Alive = true;
    mkBool m_TriggerDie = false;

    const mkF32 m_SlideDuration = 0.75f;
    const mkF32 m_JumpDuration = 0.75f;
    mkBool m_OnGround = false;

    std::function<void()> m_DieCallback = nullptr;

    // Powerup(s)
    mkBool m_HasShield = false;
    MKSprite* m_Shield = nullptr;

    void AddShield();
    void RemoveShield();
    void DeleteShield();

    // Collision
    EventListenerPhysicsContact* m_ContactListener = nullptr;
    MK_DECL_INITIALISE_CONTACT_LISTENER(MKPlayer);
    MK_DECL_DEINITIALISE_CONTACT_LISTENER(MKPlayer);
    virtual mkBool OnContactBegin(cocos2d::PhysicsContact& _contact);

    // Play Animation(s)
    void PlayRunAnimation();
    void PlayJumpAnimation();
    void PlaySlideAnimation();
    void PlayDieAnimation();

    // Set PhysicsShapes
    void SetRunPhysicsShape();
    void SetJumpPhysicsShape();
    void SetSlidePhysicsShape();

    // Callback(s)
    void OnJumpFinished();
    void OnSlideFinished();
    void OnTouchGround();

    // Internal Function(s)
    void Run();
    void Jump();
    void Slide();
    void Die();

    void CollideWithObstacle();
    void PickUpShield();

    void BoundsCheck();
    void ReleasePlayerActions();
    void ReleasePhysicsShapes();

    void UpdateScore(mkF32 _deltaTime);

    // Input Event Listeners
    EventListenerCustom * m_ButtonListener = nullptr;
    EventListenerCustom* m_ClickListener = nullptr;
    EventListenerCustom* m_AxisListener = nullptr;

    // Input Callbacks
    virtual void OnButton(EventCustom * _event);
    virtual void OnClick(EventCustom * _event);
    virtual void OnAxis(EventCustom * _event) {}

    // Input Initialisation
    MK_DECL_INITIALISE_INPUT(MKPlayer);
    MK_DECL_DEINITIALISE_INPUT(MKPlayer);

    // Player Controls
    mkF32 m_SwipeDeadZone = 0.3f;

    MKCursorPosition m_SlideClickStartPosition;
    MKCursorPosition m_SlideClickCurrentPosition;
    void SlideTouchInput(const MKInputClick* _input);

    MKCursorPosition m_JumpClickStartPosition;
    MKCursorPosition m_JumpClickCurrentPosition;
    void JumpTouchInput(const MKInputClick* _input);


public:
    // Sprite
    static const mkString SHIELD_SPRITE_FILE;

    // Audio
    static const mkString SHIELD_ADD_SOUND_NAME;
    static const mkString SHIELD_REMOVE_SOUND_NAME;

    // Animation State Names
    static const mkString RUN_ANIMATION_NAME;
    static const mkString JUMP_START_ANIMATION_NAME;
    static const mkString JUMP_LOOP_ANIMATION_NAME;
    static const mkString JUMP_END_ANIMATION_NAME;
    static const mkString SLIDE_ANIMATION_NAME;
    static const mkString DIE_ANIMATION_NAME;

    // Static Function(s)
    static MKPlayer* Create(mkF32 _floorHeight, MKScene* _scene);

    // Interface Function(s)
    void SetDieCallback(const std::function<void()>& _callback) { m_DieCallback = _callback; }
    mkF32 GetFloorHeight() const { return m_FloorHeight; }
    mkF32 GetJumpHeight() const { return m_JumpHeight; }
    mkF32 GetHorizontalVelocity() const { return m_HorizontalVelocity; }
    mkF32 GetDownwardsVelocity() const { return m_DownwardsVelocity; }
    MKScene* GetScene() { return m_Scene; }
    const MKScene* GetScene() const { return m_Scene; }
    mkU32 GetScore() const { return static_cast<mkU32>(m_Score); }

CC_CONSTRUCTOR_ACCESS:
    // Constructor(s) & Destructor
    MKPlayer();
    virtual ~MKPlayer();

    // Virtual Function(s)
    virtual mkBool initPlayer(mkF32 _floorHeight, MKScene* _scene);
    virtual void update(mkF32 _deltaTime) override;
};

NS_MK_END

#endif // MK_PLAYER_H