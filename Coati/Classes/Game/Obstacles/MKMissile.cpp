// Include MK
#include "MKMissile.h"
#include "../../MK/Gameplay/MKCollisionCategory.h"
#include "../../MK/Sprite/SpriteAnimation/MKSpriteAnimation.h"

NS_MK_BEGIN

// Sprite
const mkString MKMissile::MISSILE_SPRITE_FILE = "Textures/Gameplay/Obstacles/Missile/Missile.png";

// Sprite Animation
const mkString MKMissile::EXPLOSION_PLIST_FILE = "Textures/Gameplay/Obstacles/Missile/Explosion/Explosion.plist";
const mkString MKMissile::EXPLOSION_JSON_FILE = "Textures/Gameplay/Obstacles/Missile/Explosion/Explosion.json";

// Audio
const mkString MKMissile::MISSILE_FLIGHT_SOUND_NAME = "Missile_Flight";
const mkString MKMissile::MISSILE_EXPLOSION_SOUND_NAME = "Missile_Explosion";

// Constructor(s) & Destructor
MKMissile::MKMissile(MKScene* _scene) : MKObstacle(_scene)
{
}

MKMissile::~MKMissile()
{
    MK_CALL_DEINITIALISE_CONTACT_LISTENER
    MKAudioManager::GetInstance()->StopSound(m_MissileFlightSoundID);
}

// Collision
mkBool MKMissile::OnContactBegin(cocos2d::PhysicsContact& _contact)
{
    if (!Super::OnContactBegin(_contact)) { return false; }

     // Stop listening or else this function will stil get called.
    MK_CALL_DEINITIALISE_CONTACT_LISTENER

    // Remove our PhysicsBody. We no longer need to collide.
    removeComponent(getPhysicsBody());

    stopAllActions();
    MKAudioManager::GetInstance()->StopSound(m_MissileFlightSoundID);
    m_ParticleSmoke->pauseEmissions();
    removeChild(m_Missile, true);
    m_Missile = nullptr;

    {
        // Explode
        MKSpriteAnimation* explosion = MKSpriteAnimation::Create(EXPLOSION_PLIST_FILE, EXPLOSION_JSON_FILE);
        mkF32 explosionSize = Director::getInstance()->getVisibleSize().height * 0.3f;
        explosion->setScale(explosionSize / explosion->getContentSize().width, explosionSize / explosion->getContentSize().height);
        explosion->AddTicketToQueue(MKSpriteAnimationTicket::Create("Explosion", 0.5f, 1, true));
        explosion->setPosition(getPosition());
        GetScene()->addChild(explosion);

        // Play the explosion sound.
        MKAudioManager::GetInstance()->PlaySound(MISSILE_EXPLOSION_SOUND_NAME, MKSound::SoundType::SFX);
    }

    return true;
}

// Static Function(s)
MKMissile* MKMissile::Create(MKScene* _scene)
{
    MKMissile* obstacle = new (std::nothrow) MKMissile(_scene);
    if (obstacle && obstacle->init())
    {
        obstacle->autorelease();
        return obstacle;
    }

    CC_SAFE_DELETE(obstacle);
    return nullptr;
}

// Interface Function(s)
mkF32 MKMissile::GetHorizontalVelocity()
{
    return m_HorizontalVelocity;
}

void MKMissile::pause()
{
    Super::pause();

    // Pause Audio
    MKAudioManager::GetInstance()->PauseSound(m_MissileFlightSoundID);

    // Pause Children
    for (Vector<Node*>::iterator iter = _children.begin(); iter != _children.end(); ++iter)
    {
        (*iter)->pause();
    }
}

void MKMissile::resume()
{
    Super::resume();

    // Resume Audio
    MKAudioManager::GetInstance()->ResumeSound(m_MissileFlightSoundID);

    // Resume Children
    for (Vector<Node*>::iterator iter = _children.begin(); iter != _children.end(); ++iter)
    {
        (*iter)->resume();
    }
}

// Virtual Function(s)
mkBool MKMissile::init()
{
    if (!Super::init()) { return false; }

    Size visibleSize = Director::getInstance()->getVisibleSize();

    m_HorizontalVelocity = -Director::getInstance()->getVisibleSize().height * 2.0f;

    // Create the Missile.
    m_Missile = MKSprite::CreateWithFile(MISSILE_SPRITE_FILE);
    m_Missile->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(m_Missile);

    // Create our particles.
    m_ParticleSmoke = ParticleSmoke::createWithTotalParticles(400);
    m_ParticleSmoke->setEmissionRate(200.f);
    m_ParticleSmoke->setPositionType(cocos2d::ParticleSystem::PositionType::FREE);
    m_ParticleSmoke->setDuration(cocos2d::ParticleSystem::DURATION_INFINITY);
    m_ParticleSmoke->setLife(0.8f);
    m_ParticleSmoke->setLifeVar(0.2f);
    m_ParticleSmoke->setStartSize(m_Missile->getContentSize().height);
    m_ParticleSmoke->setStartSizeVar(m_Missile->getContentSize().height * 0.1f);
    m_ParticleSmoke->setEndSize(m_Missile->getContentSize().height * 3.0f);
    m_ParticleSmoke->setEndSizeVar(m_Missile->getContentSize().height * 0.1f);
    m_ParticleSmoke->setAutoRemoveOnFinish(false);
    m_ParticleSmoke->setScale(getScale());
    addChild(m_ParticleSmoke);

    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setContentSize(m_Missile->getContentSize());
    m_Missile->setPosition(getContentSize() * 0.5f);
    m_ParticleSmoke->setPosition(Vec2(getContentSize().width, getContentSize().height * 0.5f));

    // Create the PhysicsBody.
    cocos2d::PhysicsBody* physicsBody = PhysicsBody::createBox(getContentSize() * 0.8f);
    physicsBody->setDynamic(false);
    physicsBody->setCategoryBitmask(MK_COLLISION_CATEGORY_OBSTACLE);
    physicsBody->setContactTestBitmask(MK_COLLISION_CATEGORY_PLAYER);
    physicsBody->setCollisionBitmask(MK_COLLISION_CATEGORY_NONE);
    setPhysicsBody(physicsBody);

    // Set the missile scale.
    mkF32 desiredScale = (visibleSize.height * 0.03f) / this->getContentSize().height;
    setScale(desiredScale, desiredScale);

    // Run Action(s)
    MoveBy* moveAction = MoveBy::create(1.0f, Vec2(GetHorizontalVelocity(), 0.0f));
    runAction(RepeatForever::create(moveAction));

    scheduleUpdate();

    MK_CALL_INITIALISE_CONTACT_LISTENER;

    return true;
}

void MKMissile::update(mkF32 _deltaTime)
{
    // Play flight sound if the missile is on the screen.
    mkF32 obstacleLeft = getPosition().x - getScaledContentSize().width * 0.5f;
    mkF32 obstacleRight = getPosition().x + getScaledContentSize().width * 0.5f;
    mkF32 obstacleTop = getPosition().x + getScaledContentSize().height * 0.5f;
    mkF32 obstacleBottom = getPosition().x - getScaledContentSize().height * 0.5f;

    if (IsOnScreen(obstacleLeft, obstacleRight, obstacleTop, obstacleBottom))
    {
        if (m_MissileFlightSoundID == MKAudioManager::INVALID_SOUND_ID)
        {
            m_MissileFlightSoundID = MKAudioManager::GetInstance()->PlaySound(MISSILE_FLIGHT_SOUND_NAME, MKSound::SoundType::SFX, 1.0f, true);
        }
    }
    else
    {
        if (m_MissileFlightSoundID != MKAudioManager::INVALID_SOUND_ID)
        {
            MKAudioManager::GetInstance()->StopSound(m_MissileFlightSoundID);
            m_MissileFlightSoundID = MKAudioManager::INVALID_SOUND_ID;
        }
    }
}

NS_MK_END