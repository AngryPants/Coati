// Include MK
#include "MKPlayer.h"
#include "MK/Gameplay/MKCollisionCategory.h"
#include "MK/GameData/MKGameDataLoader.h"
#include "MK/Audio/MKAudioManager.h"

// Include Game
#include "Game/GameData/MKPlayerData.h"
#include "Game/GameData/MKHeroData.h"

NS_MK_BEGIN

// Sprite
const mkString MKPlayer::SHIELD_SPRITE_FILE = "Textures/Gameplay/Powerups/Shield/Shield.png";

// Audio
const mkString MKPlayer::SHIELD_ADD_SOUND_NAME = "Shield_Add";
const mkString MKPlayer::SHIELD_REMOVE_SOUND_NAME = "Shield_Remove";

// Animation State Names
const mkString MKPlayer::RUN_ANIMATION_NAME = "Run";
const mkString MKPlayer::JUMP_START_ANIMATION_NAME = "Jump_Start";
const mkString MKPlayer::JUMP_LOOP_ANIMATION_NAME = "Jump_Loop";
const mkString MKPlayer::JUMP_END_ANIMATION_NAME = "Jump_End";
const mkString MKPlayer::SLIDE_ANIMATION_NAME = "Slide";
const mkString MKPlayer::DIE_ANIMATION_NAME = "Die";

// Constructor(s) & Destructor
MKPlayer::MKPlayer()
{
}

MKPlayer::~MKPlayer()
{
    ReleasePlayerActions();
    ReleasePhysicsShapes();
    DeleteShield();
    MK_CALL_DEINITIALISE_INPUT
    MK_CALL_DEINITIALISE_CONTACT_LISTENER
}

// Collision Callback
mkBool MKPlayer::OnContactBegin(cocos2d::PhysicsContact& _contact)
{
    PhysicsShape* physicsShapeA = _contact.getShapeA();
    PhysicsShape* physicsShapeB = _contact.getShapeB();
    // Ignore this collision if we're not involved.
    if (physicsShapeA->getBody() != _physicsBody &&
        physicsShapeB->getBody() != _physicsBody)
    {
        return false;
    }

    PhysicsBody* otherPhysicsBody = (physicsShapeA->getBody() == getPhysicsBody()) ? physicsShapeB->getBody() : physicsShapeA->getBody();
    if (!NS_MK::MKMathsHelper::CompareBitmasks<mkS32>(getPhysicsBody()->getContactTestBitmask(), otherPhysicsBody->getCategoryBitmask()))
    {
        return false;
    }

    switch (otherPhysicsBody->getCategoryBitmask())
    {
    case MK_COLLISION_CATEGORY_OBSTACLE:
        CollideWithObstacle();
        break;
    case MK_COLLISION_CATEGORY_SHIELD:
        PickUpShield();
        break;
    default:
        MK_ASSERT(false);
        break;
    }

    return true;
}

// Play Animation(s)
void MKPlayer::PlayRunAnimation()
{
    ClearAllTickets();
    AddTicketToQueue(MKSpriteAnimationTicket::Create(RUN_ANIMATION_NAME, 0.5f, MKSpriteAnimationTicket::INFINITE_LOOPS, false));
}

void MKPlayer::PlayJumpAnimation()
{
    ClearAllTickets();
    AddTicketToQueue(MKSpriteAnimationTicket::Create(JUMP_START_ANIMATION_NAME, m_JumpDuration * 0.2f, 1, false));
    AddTicketToQueue(MKSpriteAnimationTicket::Create(JUMP_LOOP_ANIMATION_NAME, m_JumpDuration * 0.6f, 1, false));
    AddTicketToQueue(MKSpriteAnimationTicket::Create(JUMP_END_ANIMATION_NAME, m_JumpDuration * 0.2f, MKSpriteAnimationTicket::INFINITE_LOOPS, false));
}

void MKPlayer::PlaySlideAnimation()
{
    ClearAllTickets();
    AddTicketToQueue(MKSpriteAnimationTicket::Create(SLIDE_ANIMATION_NAME, 0.2f, MKSpriteAnimationTicket::INFINITE_LOOPS, false));
}

void MKPlayer::PlayDieAnimation()
{
    ClearAllTickets();
    AddTicketToQueue(MKSpriteAnimationTicket::Create(DIE_ANIMATION_NAME, 0.2f, 1, false));
}

// Set PhysicsShapes
void MKPlayer::SetRunPhysicsShape()
{
    if (m_RunPhysicsShape == nullptr)
    {
        m_RunPhysicsShape = PhysicsShapeBox::create(Size(getContentSize().width * 0.5f, getContentSize().height * 0.8f), PHYSICSBODY_MATERIAL_DEFAULT, Vec2::ZERO);
        m_RunPhysicsShape->setCategoryBitmask(MK_COLLISION_CATEGORY_PLAYER);
        m_RunPhysicsShape->setCollisionBitmask(MK_COLLISION_CATEGORY_NONE);
        m_RunPhysicsShape->setContactTestBitmask(MK_COLLISION_CATEGORY_OBSTACLE | MK_COLLISION_CATEGORY_SHIELD);
        m_RunPhysicsShape->retain();
    }
    getPhysicsBody()->removeAllShapes(true);
    getPhysicsBody()->addShape(m_RunPhysicsShape, true);
}

void MKPlayer::SetJumpPhysicsShape()
{
    if (m_JumpPhysicsShape == nullptr)
    {
        m_JumpPhysicsShape = PhysicsShapeBox::create(Size(getContentSize().width * 0.5f, getContentSize().height * 0.8f), PHYSICSBODY_MATERIAL_DEFAULT, Vec2::ZERO);
        m_JumpPhysicsShape->setCategoryBitmask(MK_COLLISION_CATEGORY_PLAYER);
        m_JumpPhysicsShape->setCollisionBitmask(MK_COLLISION_CATEGORY_NONE);
        m_JumpPhysicsShape->setContactTestBitmask(MK_COLLISION_CATEGORY_OBSTACLE | MK_COLLISION_CATEGORY_SHIELD);
        m_JumpPhysicsShape->retain();
    }
    getPhysicsBody()->removeAllShapes(true);
    getPhysicsBody()->addShape(m_JumpPhysicsShape, true);
}

void MKPlayer::SetSlidePhysicsShape()
{
    if (m_SlidePhysicsShape == nullptr)
    {
        m_SlidePhysicsShape = PhysicsShapeBox::create(Size(getContentSize().width * 0.5f, getContentSize().height * 0.4f), PHYSICSBODY_MATERIAL_DEFAULT, Vec2(0.0f, -getContentSize().height * 0.2f));
        m_SlidePhysicsShape->setCategoryBitmask(MK_COLLISION_CATEGORY_PLAYER);
        m_SlidePhysicsShape->setCollisionBitmask(MK_COLLISION_CATEGORY_NONE);
        m_SlidePhysicsShape->setContactTestBitmask(MK_COLLISION_CATEGORY_OBSTACLE | MK_COLLISION_CATEGORY_SHIELD);
        m_SlidePhysicsShape->retain();
    }
    getPhysicsBody()->removeAllShapes(true);
    getPhysicsBody()->addShape(m_SlidePhysicsShape, true);
}

// Callback(s)
void MKPlayer::OnJumpFinished()
{
    BoundsCheck();
    PlayRunAnimation();
    SetRunPhysicsShape();
}

void MKPlayer::OnSlideFinished()
{
    BoundsCheck();
    PlayRunAnimation();
    SetRunPhysicsShape();
}

void MKPlayer::OnTouchGround()
{
    m_OnGround = true;
}

// Internal Function(s)
void MKPlayer::Run()
{
    setPositionY(m_RunHeight);

    // Run Action(s)
    if (m_RunAction == nullptr)
    {
        MoveBy* run = MoveBy::create(1.0f, cocos2d::Vec3(m_HorizontalVelocity, 0.0f, 0.0f));
        m_RunAction = RepeatForever::create(run);
        m_RunAction->retain();
    }

    runAction(m_RunAction);

    PlayRunAnimation();
    SetRunPhysicsShape();
}

void MKPlayer::Jump()
{
    // We can only jump when we are on the ground.
    if (m_OnGround == false) { return; }
    m_OnGround = false;

    // If we are currently sliding or jumping, stop that action.
    stopAction(m_SlideAction);
    stopAction(m_JumpAction);

    // Run Action(s)
    if (m_JumpAction == nullptr)
    {
        MKMoveBySinCurveAction* jump = MKMoveBySinCurveAction::Create(m_JumpDuration, 0.0f, m_JumpHeight - m_RunHeight, 0.5f);
        cocos2d::CallFunc* touchGroundCallback = CallFunc::create(CC_CALLBACK_0(MKPlayer::OnTouchGround, this));
        cocos2d::CallFunc* finishCallback = CallFunc::create(CC_CALLBACK_0(MKPlayer::OnJumpFinished, this));

        m_JumpAction = Sequence::create(jump, touchGroundCallback, finishCallback, nullptr);
        m_JumpAction->retain();
    }
    
    runAction(m_JumpAction);

    PlayJumpAnimation();
    SetJumpPhysicsShape();
}

void MKPlayer::Slide()
{
    // We can slide anytime, even in the air.

    // If we are currently sliding or jumping, stop that action.
    stopAction(m_JumpAction);
    stopAction(m_SlideAction);

    // Run Action(s)
    if (m_SlideAction != nullptr)
    {
        m_SlideAction->release();
        m_SlideAction = nullptr;
    }

    mkF32 moveDistance = m_RunHeight - getPosition().y;
    mkF32 moveTime = MKMathsHelper::Abs<mkF32>(moveDistance / m_DownwardsVelocity);

    cocos2d::MoveBy* moveToGround = cocos2d::MoveBy::create(moveTime, cocos2d::Vec3(0.0f, moveDistance, 0.0f)); // Move the character m_RunHeight.
    cocos2d::CallFunc* touchGroundCallback = CallFunc::create(CC_CALLBACK_0(MKPlayer::OnTouchGround, this));
    cocos2d::DelayTime* delayTime = cocos2d::DelayTime::create(m_SlideDuration);
    cocos2d::CallFunc* finishCallback = CallFunc::create(CC_CALLBACK_0(MKPlayer::OnSlideFinished, this));

    m_SlideAction = Sequence::create(moveToGround, touchGroundCallback, delayTime, finishCallback, nullptr);
    m_SlideAction->retain();
    
    runAction(m_SlideAction);

    PlaySlideAnimation();
    SetSlidePhysicsShape();
}

void MKPlayer::Die()
{
    // Stop all other actions
    stopAction(m_RunAction);
    stopAction(m_JumpAction);
    stopAction(m_SlideAction);

    // Run Action(s)
    if (m_DieAction == nullptr)
    {
        mkF32 moveDistance = m_RunHeight - getPosition().y;
        mkF32 moveTime = MKMathsHelper::Abs<mkF32>(moveDistance / m_DownwardsVelocity);
        m_DieAction = cocos2d::MoveBy::create(moveTime, cocos2d::Vec3(0.0f, moveDistance, 0.0f)); // Move the character m_RunHeight.
        m_DieAction->retain();
    }

    runAction(m_DieAction);

    PlayDieAnimation();
    getPhysicsBody()->removeAllShapes(true);
    m_Alive = false;

    if (m_DieCallback != nullptr) { m_DieCallback(); }
}

void MKPlayer::CollideWithObstacle()
{
    if (m_HasShield)
    {
        RemoveShield();
    }
    else
    {
        // Stop listening or else this function will stil get called.
        MK_CALL_DEINITIALISE_CONTACT_LISTENER;
        m_TriggerDie = true;
    }
}

void MKPlayer::PickUpShield()
{
    AddShield();
}

void MKPlayer::BoundsCheck()
{
    if (getPositionY() > m_JumpHeight)
    {
        setPositionY(m_JumpHeight);
    }

    if (getPositionY() < m_RunHeight)
    {
        setPositionY(m_RunHeight);
    }
}

void MKPlayer::ReleasePlayerActions()
{
    if (m_RunAction)
    {
        stopAction(m_RunAction);
        m_RunAction->release();
        m_RunAction = nullptr;
    }
    if (m_JumpAction)
    {
        stopAction(m_JumpAction);
        m_JumpAction->release();
        m_JumpAction = nullptr;
    }
    if (m_SlideAction)
    {
        stopAction(m_SlideAction);
        m_SlideAction->release();
        m_SlideAction = nullptr;
    }
    if (m_DieAction)
    {
        stopAction(m_DieAction);
        m_DieAction->release();
        m_DieAction = nullptr;
    }

    ClearAllTickets();
}

void MKPlayer::ReleasePhysicsShapes()
{
    if (m_RunPhysicsShape)
    {
        m_RunPhysicsShape->release();
        m_RunPhysicsShape = nullptr;
    }
    if (m_JumpPhysicsShape)
    {
        m_JumpPhysicsShape->release();
        m_JumpPhysicsShape = nullptr;
    }
    if (m_SlidePhysicsShape)
    {
        m_SlidePhysicsShape->release();
        m_SlidePhysicsShape = nullptr;
    }

    if (getPhysicsBody())
    {
        getPhysicsBody()->removeAllShapes(true);
    }
}

void MKPlayer::UpdateScore(mkF32 _deltaTime)
{
    if (m_Alive)
    {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        m_Score += (GetHorizontalVelocity() * _deltaTime * 10.0f) / visibleSize.height;
    }
}

// Powerup(s)
void MKPlayer::AddShield()
{
    m_HasShield = true;

    if (m_Shield == nullptr)
    {
        m_Shield = MKSprite::CreateWithFile(SHIELD_SPRITE_FILE);
        m_Shield->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_Shield->retain();

        auto visibleSize = Director::getInstance()->getVisibleSize();

        // Set Scale
        mkF32 desiredSize = visibleSize.height * 0.3f;
        m_Shield->setScaleX(desiredSize / (m_Shield->getContentSize().width * getScaleX()));
        m_Shield->setScaleY(desiredSize / (m_Shield->getContentSize().height * getScaleY()));
        m_Shield->setPosition(getContentSize().width * 0.5f, getContentSize().height * 0.5f);
    }

    if (m_Shield->getParent() == nullptr)
    {
        addChild(m_Shield);
        MKAudioManager::GetInstance()->PlaySound(SHIELD_ADD_SOUND_NAME, MKSound::SoundType::SFX);
    }
}

void MKPlayer::RemoveShield()
{
    m_HasShield = false;

    if (m_Shield && m_Shield->getParent() == this)
    {
        removeChild(m_Shield, true);
        MKAudioManager::GetInstance()->PlaySound(SHIELD_REMOVE_SOUND_NAME, MKSound::SoundType::SFX);
    }
}

void MKPlayer::DeleteShield()
{
    RemoveShield();

    if (m_Shield != nullptr)
    {
        m_Shield->release();
        m_Shield = nullptr;
    }
}

// Input Callbacks
void MKPlayer::OnButton(EventCustom * _event)
{
    MKInputButton* input = static_cast<MKInputButton*>(_event->getUserData());

    if (!m_Alive)
    {
        return;
    }

    if (input->m_ButtonState != MKInputButton::ButtonState::PRESS)
    {
        return;
    }

    switch (input->m_InputName)
    {
    case MKInputName::JUMP:
        Jump();
        break;
    case MKInputName::SLIDE:
        Slide();
        break;
    default:
        break;
    }
}

void MKPlayer::OnClick(EventCustom * _event)
{
    MKInputClick* input = static_cast<MKInputClick*>(_event->getUserData());
    
    if (!m_Alive)
    {
        return;
    }

    if (input->m_InputName == MinamiKotori::MKInputName::JUMP)
    {
        JumpTouchInput(input);
    }

    if (input->m_InputName == MinamiKotori::MKInputName::SLIDE)
    {
        SlideTouchInput(input);
    }
}

// Player Controls
void MKPlayer::JumpTouchInput(const MKInputClick*_input)
{
    switch (_input->m_ButtonState)
    {
    case MinamiKotori::MKInputButton::ButtonState::PRESS:
        m_JumpClickStartPosition = _input->m_CursorPosition;
        m_JumpClickCurrentPosition = _input->m_CursorPosition;
        break;
    case MinamiKotori::MKInputButton::ButtonState::HOLD:
        // Do nothing.
        break;
    case MinamiKotori::MKInputButton::ButtonState::RELEASE:
        {
            m_JumpClickCurrentPosition = _input->m_CursorPosition;
            // Ensure that the delta movement of the click is greater than the deadzone.
            // We only care about the upwards vertical movement.
            mkF32 movementDelta = m_JumpClickCurrentPosition.GetY() - m_JumpClickStartPosition.GetY();
            if (movementDelta > 0.0f && MKMathsHelper::Abs(movementDelta) >= m_SwipeDeadZone)
            {
                Jump();
            }
        }
        break;
    default:
        // Do nothing.
        break;
    }
}

void MKPlayer::SlideTouchInput(const MKInputClick*_input)
{
    switch (_input->m_ButtonState)
    {
    case MinamiKotori::MKInputButton::ButtonState::PRESS:
        m_SlideClickStartPosition = _input->m_CursorPosition;
        m_SlideClickCurrentPosition = _input->m_CursorPosition;
        break;
    case MinamiKotori::MKInputButton::ButtonState::HOLD:
        // Do nothing.
        break;
    case MinamiKotori::MKInputButton::ButtonState::RELEASE:
        {
            m_SlideClickCurrentPosition = _input->m_CursorPosition;
            // Ensure that the delta movement of the click is greater than the deadzone.
            // We only care about the downwards vertical movement.
            mkF32 movementDelta = m_SlideClickCurrentPosition.GetY() - m_SlideClickStartPosition.GetY();
            if (movementDelta < 0.0f && MKMathsHelper::Abs(movementDelta) > m_SwipeDeadZone)
            {
                Slide();
            }
        }
        break;
    default:
        // Do nothing.
        break;
    }
}

// Static Function(s)
MKPlayer* MKPlayer::Create(mkF32 _floorHeight, MKScene* _scene)
{
    MKPlayer* player = new (std::nothrow) MKPlayer();
    if (player && player->initPlayer(_floorHeight, _scene))
    {
        player->autorelease();
        return player;
    }

    CC_SAFE_DELETE(player);
    return nullptr;
}

// Virtual Function(s)
mkBool MKPlayer::initPlayer(mkF32 _floorHeight, MKScene* _scene)
{
    MKHeroData* heroData = MKGameDataLoader::GetInstance()->GetGameData<MKHeroData>();
    MKPlayerData* playerData = MKGameDataLoader::GetInstance()->GetGameData<MKPlayerData>();
    const MKHeroShopItem* heroItem = heroData->GetHero(playerData->GetEquippedHero());
    if (heroItem == nullptr)
    {
        heroItem = heroData->GetHero(MKHeroData::DEFAULT_HERO_NAME);
    }
    MK_ASSERT(heroItem != nullptr);

    if (!Super::initSpriteAnimation(heroItem->GetPListFile(), heroItem->GetJSONFile())) { return false; }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Set Scale
    mkF32 desiredSize = visibleSize.height * 0.3f;
    setScaleX(desiredSize / getContentSize().width);
    setScaleY(desiredSize / getContentSize().height);

    // Set Variable(s)
    m_OnGround = true;
    m_FloorHeight = _floorHeight;
    m_JumpHeight = visibleSize.height * 0.8f;
    m_HorizontalVelocity = visibleSize.height;
    m_DownwardsVelocity = visibleSize.height * -2.0f;
    m_RunHeight = m_FloorHeight + getScaledContentSize().height * 0.5f;
    m_Scene = _scene;
    m_Score = 0;
    m_Alive = true;
    m_TriggerDie = false;

    // Create PhysicsBody
    auto physicsBody = PhysicsBody::create();
    physicsBody->setDynamic(false);
    setPhysicsBody(physicsBody);

    // Initialise Input
    MK_CALL_INITIALISE_INPUT
    MK_CALL_INITIALISE_CONTACT_LISTENER

    scheduleUpdate();

    Run();

    return true;
}

void MKPlayer::update(mkF32 _deltaTime)
{
    if (m_TriggerDie)
    {
        Die();
        m_TriggerDie = false;
    }

    UpdateScore(_deltaTime);
}

NS_MK_END