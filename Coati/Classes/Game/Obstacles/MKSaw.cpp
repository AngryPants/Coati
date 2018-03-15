// Include GT
#include "MKSaw.h"
#include "../../MK/Gameplay/MKCollisionCategory.h"
#include "../../MK/Actions/MKMoveBySinCurveAction.h"

NS_MK_BEGIN

// Sprite
const mkString MKSaw::SAW_SPRITE_FILE = "Textures/Gameplay/Obstacles/Saw/Saw.png";

// Audio
const mkString MKSaw::SAW_SPINNING_SOUND_NAME = "Saw_Spinning";
const mkString MKSaw::SAW_HIT_SOUND_NAME = "Saw_Hit";

// Constructor(s) & Destructor
MKSaw::MKSaw(MKScene* _scene)
    : MKObstacle(_scene)
{
}

MKSaw::~MKSaw()
{
    MK_CALL_DEINITIALISE_CONTACT_LISTENER
    MKAudioManager::GetInstance()->StopSound(m_SawSpinningSoundID);
}

// Static Function(s)
MKSaw* MKSaw::Create(MKScene* _scene, mkF32 _startOffset)
{
	MKSaw* obstacle = new (std::nothrow) MKSaw(_scene);
	if (obstacle && obstacle->init(_startOffset))
	{
		obstacle->autorelease();
		return obstacle;
	}

	CC_SAFE_DELETE(obstacle);
	return nullptr;
}

// Collision
mkBool MKSaw::OnContactBegin(cocos2d::PhysicsContact& _contact)
{
    if (!Super::OnContactBegin(_contact)) { return false; }

    // Stop listening or else this function will stil get called.
    MK_CALL_DEINITIALISE_CONTACT_LISTENER

    // Remove our PhysicsBody. We no longer need to collide.
    removeComponent(getPhysicsBody());

    // Play Saw Hit Audio
    MKAudioManager::GetInstance()->PlaySound(SAW_HIT_SOUND_NAME, MKSound::SoundType::SFX);

	return true;
}

// Virtual Override(s)
mkBool MKSaw::init(mkF32 _startOffset)
{
    if (!Super::init()) { return false; }

    // Create the MKSprite.
    m_Saw = MKSprite::CreateWithFile(SAW_SPRITE_FILE);
    m_Saw->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(m_Saw);

    // Set our Size
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setContentSize(m_Saw->getContentSize());
    m_Saw->setPosition(getContentSize() * 0.5f);

    // Create the PhysicsBody.
    cocos2d::PhysicsBody* physicsBody = PhysicsBody::createCircle(m_Saw->getContentSize().height * 0.5f, PHYSICSBODY_MATERIAL_DEFAULT);
    physicsBody->setDynamic(false);
    physicsBody->setCategoryBitmask(MK_COLLISION_CATEGORY_OBSTACLE);
    physicsBody->setContactTestBitmask(MK_COLLISION_CATEGORY_PLAYER);
    physicsBody->setCollisionBitmask(MK_COLLISION_CATEGORY_NONE);
    setPhysicsBody(physicsBody);

    // Set the saw scale.
    Size visibleSize = Director::getInstance()->getVisibleSize();
    mkF32 desiredSize = visibleSize.height * 0.2f;
    setScale(desiredSize / getContentSize().width, desiredSize / getContentSize().height);

    // Set Variable(s)
    m_StartOffset = _startOffset;
    m_RotationSpeed = 720.0f;
    m_CycleDuration = 8.0f;
    m_MoveDisplacement = visibleSize.height * 0.3f;

    // Run actions
    mkF32 phaseShift = m_StartOffset * MKMathsHelper::TWO_PI;
    auto moveAction = MKMoveBySinCurveAction::Create(m_CycleDuration, phaseShift, m_MoveDisplacement);
    auto rotateAction = RotateBy::create(m_CycleDuration, m_RotationSpeed * m_CycleDuration);
    runAction(
        RepeatForever::create(
            Spawn::createWithTwoActions(
                moveAction,
                rotateAction
            )
        )
    );

    MK_CALL_INITIALISE_CONTACT_LISTENER

    return true;
}

void MKSaw::update(mkF32 _deltaTime)
{
    // Play saw sound if the saw is on the screen.
    mkF32 obstacleLeft = getPosition().x - getScaledContentSize().width * 0.5f;
    mkF32 obstacleRight = getPosition().x + getScaledContentSize().width * 0.5f;
    mkF32 obstacleTop = getPosition().x + getScaledContentSize().height * 0.5f;
    mkF32 obstacleBottom = getPosition().x - getScaledContentSize().height * 0.5f;

    if (IsOnScreen(obstacleLeft, obstacleRight, obstacleTop, obstacleBottom))
    {
        if (m_SawSpinningSoundID == MKAudioManager::INVALID_SOUND_ID)
        {
            // m_SawSpinningSoundID = MKAudioManager::GetInstance()->PlaySound(SAW_SPINNING_SOUND_NAME, MKSound::SoundType::SFX, 1.0f, true);
        }
    }
    else
    {
        if (m_SawSpinningSoundID != MKAudioManager::INVALID_SOUND_ID)
        {
            MKAudioManager::GetInstance()->StopSound(m_SawSpinningSoundID);
            m_SawSpinningSoundID = MKAudioManager::INVALID_SOUND_ID;
        }
    }
}

void MKSaw::pause()
{
    Super::pause();

    // Pause Audio
    MKAudioManager::GetInstance()->PauseSound(m_SawSpinningSoundID);
}

void MKSaw::resume()
{
    Super::resume();

    // Resume Audio
    MKAudioManager::GetInstance()->ResumeSound(m_SawSpinningSoundID);
}

NS_MK_END