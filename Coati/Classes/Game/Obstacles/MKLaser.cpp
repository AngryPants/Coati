// Include GT
#include "MKLaser.h"
#include "MK/Actions/MKFollowNodeAction.h"
#include "MK/Gameplay/MKCollisionCategory.h"
#include "MK/Sprite/SpriteAnimation/MKSpriteAnimation.h"

NS_MK_BEGIN

const mkF32 MKLaser::LASER_MOVE_DOWN_DURATION = 2.0f;
const mkF32 MKLaser::LASER_MOVE_UP_DURATION = 1.0f;
const mkF32 MKLaser::LASER_BEAM_CHARGE_DURATION = 1.0f;
const mkF32 MKLaser::LASER_BEAM_SHOOT_DURATION = 0.2f;

// Sprite
const mkString MKLaser::LASER_GUN_SPRITE_FILE = "Textures/Gameplay/Obstacles/Laser/Laser_Gun.png";
const mkString MKLaser::LASER_WARNING_LINE_SPRITE_FILE = "Textures/Gameplay/Obstacles/Laser/Laser_Warning_Line.png";
const mkString MKLaser::LASER_WARNING_SPARK_SPRITE_FILE = "Textures/Gameplay/Obstacles/Laser/Laser_Warning_Spark.png";

// Sprite Animation
const mkString MKLaser::LASER_BEAM_PLIST_FILE = "Textures/Gameplay/Obstacles/Laser/Beam/Beam.plist";
const mkString MKLaser::LASER_BEAM_JSON_FILE = "Textures/Gameplay/Obstacles/Laser/Beam/Beam.json";

// Audio
const mkString MKLaser::LASER_CHARGING_SOUND_NAME = "Laser_Charging";
const mkString MKLaser::LASER_SHOOTING_SOUND_NAME = "Laser_Shooting";

// Static Function(s)
MKLaser* MKLaser::Create(MKScene* _scene, mkF32 _startDelay)
{
    MKLaser* obstacle = new (std::nothrow) MKLaser(_scene);
    if (obstacle && obstacle->init(_startDelay))
    {
        obstacle->autorelease();
        return obstacle;
    }

    CC_SAFE_DELETE(obstacle);
    return nullptr;
}

// Virtual Override(s)
mkBool MKLaser::init(mkF32 _startDelay)
{
    if (!Super::init()) { return false; }

    Size visibleSize = Director::getInstance()->getVisibleSize();

    // Set the anchor point.
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    mkF32 followTime = LASER_MOVE_DOWN_DURATION + LASER_MOVE_UP_DURATION + LASER_BEAM_CHARGE_DURATION + LASER_BEAM_SHOOT_DURATION;
    auto followAction = MKFollowNodeAction::Create(followTime, GetScene()->getDefaultCamera(), MKFollowNodeAction::FollowAxis::X);

    auto spawnGunsCallback = CallFunc::create(CC_CALLBACK_0(MKLaser::SpawnLaserGuns, this));
    auto spawnBeamCallback = CallFunc::create(CC_CALLBACK_0(MKLaser::SpawnLaserBeam, this));

    mkF32 verticalMovementDistance = (mkF32)MKMathsHelper::RandomInt(static_cast<mkS32>(visibleSize.height * 0.5f), static_cast<mkS32>(visibleSize.height * 0.9f));
    auto moveDownAction = MoveBy::create(LASER_MOVE_DOWN_DURATION, cocos2d::Vec2(0.0f, -verticalMovementDistance));
    auto moveUpAction = MoveBy::create(LASER_MOVE_UP_DURATION, cocos2d::Vec2(0.0f, verticalMovementDistance));

    auto spawnParticlesCallback = CallFunc::create(CC_CALLBACK_0(MKLaser::SpawnParticles, this));
    auto despawnParticlesCallback = CallFunc::create(CC_CALLBACK_0(MKLaser::DespawnParticles, this));

    auto spawnLaserWarningCallback = CallFunc::create(CC_CALLBACK_0(MKLaser::SpawnLaserWarning, this));
    auto despawnLaserWarningCallback = CallFunc::create(CC_CALLBACK_0(MKLaser::DespawnLaserWarning, this));

    auto spawnPhysicsBodyCallback = CallFunc::create(CC_CALLBACK_0(MKLaser::SpawnPhysicsBody, this));
    auto despawnPhysicsBodyCallback = CallFunc::create(CC_CALLBACK_0(MKLaser::DespawnPhysicsBody, this));

    runAction(
        Sequence::create(
            DelayTime::create(_startDelay),
            Spawn::createWithTwoActions(
                followAction,
                Sequence::create(
                    spawnGunsCallback,
                    // Move the laser into position.
                    moveDownAction,
                    // Charge up the beam.
                    spawnParticlesCallback,
                    spawnLaserWarningCallback,
                    DelayTime::create(LASER_BEAM_CHARGE_DURATION),
                    despawnLaserWarningCallback,
                    // Shoot the beam.
                    spawnBeamCallback,
                    spawnPhysicsBodyCallback,
                    DelayTime::create(LASER_BEAM_SHOOT_DURATION),
                    // Stop the beam.
                    despawnPhysicsBodyCallback,
                    despawnParticlesCallback,
                    // Move the laser away.
                    moveUpAction,
                    // End of sequence.
                    nullptr
                )
            ),
            nullptr
        )
    );

    return true;
}

void MKLaser::pause()
{
    Super::pause();

    // Pause Audio
    MKAudioManager::GetInstance()->PauseSound(m_LaserBeamChargingSoundID);
    MKAudioManager::GetInstance()->PauseSound(m_LaserBeamShootingSoundID);

    // Pause Warning Spark
    if (m_LaserWarningSparkLeft != nullptr)
    {
        m_LaserWarningSparkLeft->pause();
    }

    if (m_LaserWarningSparkRight != nullptr)
    {
        m_LaserWarningSparkRight->pause();
    }
}

void MKLaser::resume()
{
    Super::resume();

    // Resume Audio
    MKAudioManager::GetInstance()->ResumeSound(m_LaserBeamChargingSoundID);
    MKAudioManager::GetInstance()->ResumeSound(m_LaserBeamShootingSoundID);

    // Resume Warning Spark
    if (m_LaserWarningSparkLeft != nullptr)
    {
        m_LaserWarningSparkLeft->resume();
    }

    if (m_LaserWarningSparkRight != nullptr)
    {
        m_LaserWarningSparkRight->resume();
    }
}

// Collision
mkBool MKLaser::OnContactBegin(cocos2d::PhysicsContact& _contact)
{
    if (!Super::OnContactBegin(_contact))
    {
        return false;
    }

    // Stop listening or else this function will stil get called.
    MK_CALL_DEINITIALISE_CONTACT_LISTENER

    if (getPhysicsBody() != nullptr)
    {
        this->removeComponent(getPhysicsBody());
    }

    return true;
}

void MKLaser::SpawnLaserGuns()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    // Create the sprites.
    m_LaserGunLeft = MKSprite::CreateWithFile(LASER_GUN_SPRITE_FILE, MKSprite::REPEAT);
    m_LaserGunLeft->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(m_LaserGunLeft, m_LaserGunZPriority);

    // Make the right laser gun face the opposite direction.
    m_LaserGunRight = MKSprite::CreateWithFile(LASER_GUN_SPRITE_FILE, MKSprite::REPEAT);
    m_LaserGunRight->SetTextureScale(-1.0f, 1.0f);
    m_LaserGunRight->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(m_LaserGunRight, m_LaserGunZPriority);

    // Scale the laser guns to the correct size.
    mkF32 desiredHeight = visibleSize.height * 0.05f;
    mkF32 desiredWidth = desiredHeight;
    
    m_LaserGunLeft->setScale(desiredWidth / m_LaserGunLeft->getContentSize().width, desiredHeight / m_LaserGunLeft->getContentSize().height);
    m_LaserGunLeft->setPositionX((m_LaserGunLeft->getScaledContentSize().width - visibleSize.width) * 0.5f);
     
    m_LaserGunRight->setScale(desiredWidth / m_LaserGunRight->getContentSize().width, desiredHeight / m_LaserGunRight->getContentSize().height);
    m_LaserGunRight->setPositionX((visibleSize.width - m_LaserGunRight->getScaledContentSize().width) * 0.5f);
}

void MKLaser::SpawnParticles()
{
    // Create our particles for the left gun.
    m_ParticlesLeft = ParticleFire::createWithTotalParticles(200);
    m_ParticlesLeft->setEmissionRate(20.0f);
    m_ParticlesLeft->setPositionType(cocos2d::ParticleSystem::PositionType::RELATIVE);
    m_ParticlesLeft->setDuration(ParticleSystem::DURATION_INFINITY);
    m_ParticlesLeft->setLife(0.1f);
    m_ParticlesLeft->setLifeVar(0.05f);
    m_ParticlesLeft->setStartSize(m_LaserGunLeft->getContentSize().height);
    m_ParticlesLeft->setStartSizeVar(m_LaserGunLeft->getContentSize().height * 0.1f);
    m_ParticlesLeft->setEndSize(m_LaserGunLeft->getContentSize().height * 3.0f);
    m_ParticlesLeft->setEndSizeVar(m_LaserGunLeft->getContentSize().height * 0.1f);
    m_ParticlesLeft->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_ParticlesLeft->setPosition(m_LaserGunLeft->getContentSize().width, m_LaserGunLeft->getContentSize().height * 0.5f);
    m_LaserGunLeft->addChild(m_ParticlesLeft);

    // Create our particles for the right gun.
    m_ParticlesRight = ParticleFire::createWithTotalParticles(200);
    m_ParticlesRight->setEmissionRate(20.0f);
    m_ParticlesRight->setPositionType(cocos2d::ParticleSystem::PositionType::RELATIVE);
    m_ParticlesRight->setDuration(ParticleSystem::DURATION_INFINITY);
    m_ParticlesRight->setLife(0.1f);
    m_ParticlesRight->setLifeVar(0.05f);
    m_ParticlesRight->setStartSize(m_LaserGunRight->getContentSize().height);
    m_ParticlesRight->setStartSizeVar(m_LaserGunRight->getContentSize().height * 0.1f);
    m_ParticlesRight->setEndSize(m_LaserGunRight->getContentSize().height);
    m_ParticlesRight->setEndSizeVar(m_LaserGunRight->getContentSize().height * 3.0f);
    m_ParticlesRight->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_ParticlesRight->setPosition(0.0f, m_LaserGunRight->getContentSize().height * 0.5f);
    m_LaserGunRight->addChild(m_ParticlesRight);

    // Play Audio
    if (m_LaserBeamChargingSoundID == MKAudioManager::INVALID_SOUND_ID)
    {
        m_LaserBeamChargingSoundID = MKAudioManager::GetInstance()->PlaySound(LASER_CHARGING_SOUND_NAME, MKSound::SoundType::SFX, 1.0f, true);
    }
}

void MKLaser::DespawnParticles()
{
    if (m_ParticlesLeft)
    {
        m_LaserGunLeft->removeChild(m_ParticlesLeft, true);
        m_ParticlesLeft = nullptr;
    }

    if (m_ParticlesRight)
    {
        m_LaserGunRight->removeChild(m_ParticlesRight, true);
        m_ParticlesRight = nullptr;
    }
}

void MKLaser::SpawnLaserWarning()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    // Create Line
    {
        mkF32 desiredHeight = m_LaserGunLeft->getScaledContentSize().height;
        mkF32 desiredWidth = visibleSize.width;
        m_LaserWarningLine = MKSprite::CreateWithFile(LASER_WARNING_LINE_SPRITE_FILE, MKSprite::REPEAT);
        m_LaserWarningLine->setScaleX(desiredWidth / m_LaserWarningLine->getContentSize().width);
        m_LaserWarningLine->setScaleY(desiredHeight / m_LaserWarningLine->getContentSize().height);
        m_LaserWarningLine->setPositionY(m_LaserGunLeft->getPositionY());
        addChild(m_LaserWarningLine, m_LaserBeamZPriority);
    }

    // Create Left Spark
    {
        mkF32 desiredHeight = m_LaserGunLeft->getScaledContentSize().height;
        mkF32 desiredWidth = desiredHeight;
        m_LaserWarningSparkLeft = MKSprite::CreateWithFile(LASER_WARNING_SPARK_SPRITE_FILE);
        m_LaserWarningSparkLeft->setPositionX(-visibleSize.width * 0.5f);
        m_LaserWarningSparkLeft->setScaleX(desiredWidth / m_LaserWarningSparkLeft->getContentSize().width);
        m_LaserWarningSparkLeft->setScaleY(desiredHeight / m_LaserWarningSparkLeft->getContentSize().height);

        // Create Move Action
        auto moveAction = cocos2d::MoveBy::create(LASER_BEAM_CHARGE_DURATION, Vec2(visibleSize.width * 0.5f, 0.0f));
        m_LaserWarningSparkLeft->runAction(moveAction);
        addChild(m_LaserWarningSparkLeft, m_LaserBeamZPriority);
    }

    // Create Right Spark
    {
        mkF32 desiredHeight = m_LaserGunRight->getScaledContentSize().height;
        mkF32 desiredWidth = desiredHeight;
        m_LaserWarningSparkRight = MKSprite::CreateWithFile(LASER_WARNING_SPARK_SPRITE_FILE);
        m_LaserWarningSparkRight->setPositionX(visibleSize.width * 0.5f);
        m_LaserWarningSparkRight->setScaleX(desiredWidth / m_LaserWarningSparkRight->getContentSize().width);
        m_LaserWarningSparkRight->setScaleY(desiredHeight / m_LaserWarningSparkRight->getContentSize().height);

        // Create Move Action
        auto moveAction = cocos2d::MoveBy::create(LASER_BEAM_CHARGE_DURATION, Vec2(-visibleSize.width * 0.5f, 0.0f));
        m_LaserWarningSparkRight->runAction(moveAction);
        addChild(m_LaserWarningSparkRight, m_LaserBeamZPriority);
    }
}

void MKLaser::DespawnLaserWarning()
{
    // Delete Line
    removeChild(m_LaserWarningLine);
    m_LaserWarningLine = nullptr;

    // Remove Left Spark
    removeChild(m_LaserWarningSparkLeft);
    m_LaserWarningSparkLeft = nullptr;

    // Remove Right Spark
    removeChild(m_LaserWarningSparkRight);
    m_LaserWarningSparkRight = nullptr;
}

void MKLaser::SpawnLaserBeam()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    // Create Sprite Animation
    mkF32 desiredHeight = m_LaserGunLeft->getScaledContentSize().height;
    mkF32 desiredWidth = visibleSize.width;
    MKSpriteAnimation* laserBeam = MKSpriteAnimation::Create(LASER_BEAM_PLIST_FILE, LASER_BEAM_JSON_FILE);
    laserBeam->setScaleX(desiredWidth / laserBeam->getContentSize().width);
    laserBeam->setScaleY(desiredHeight / laserBeam->getContentSize().height);
    laserBeam->setPositionY(m_LaserGunLeft->getPositionY());
    laserBeam->AddTicketToQueue(MKSpriteAnimationTicket::Create("Beam", LASER_BEAM_SHOOT_DURATION, 1, true));
    addChild(laserBeam, m_LaserBeamZPriority);

    // Play Audio
    if (m_LaserBeamChargingSoundID != MKAudioManager::INVALID_SOUND_ID)
    {
        MKAudioManager::GetInstance()->StopSound(m_LaserBeamChargingSoundID);
        m_LaserBeamChargingSoundID = MKAudioManager::INVALID_SOUND_ID;
    }

    if (m_LaserBeamShootingSoundID == MKAudioManager::INVALID_SOUND_ID)
    {
        m_LaserBeamShootingSoundID = MKAudioManager::GetInstance()->PlaySound(LASER_SHOOTING_SOUND_NAME, MKSound::SoundType::SFX);
    }
}

void MKLaser::SpawnPhysicsBody()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    // Create the PhysicsBody.
    cocos2d::PhysicsBody* physicsBody = PhysicsBody::createBox(Size(visibleSize.width, visibleSize.height * 0.05f));
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(false);
    physicsBody->setCategoryBitmask(MK_COLLISION_CATEGORY_OBSTACLE);
    physicsBody->setContactTestBitmask(MK_COLLISION_CATEGORY_PLAYER);
    physicsBody->setCollisionBitmask(MK_COLLISION_CATEGORY_NONE);
    this->setPhysicsBody(physicsBody);

    MK_CALL_INITIALISE_CONTACT_LISTENER;
}

void MKLaser::DespawnPhysicsBody()
{
    MK_CALL_DEINITIALISE_CONTACT_LISTENER;

    if (getPhysicsBody() != nullptr)
    {
        this->removeComponent(getPhysicsBody());
    }
}

NS_MK_END