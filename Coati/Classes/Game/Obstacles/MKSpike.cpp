// Include GT
#include "MKSpike.h"
#include "../../MK/Gameplay/MKCollisionCategory.h"

NS_MK_BEGIN

// Sprite
const mkString MKSpike::SPIKE_SPRITE_FILE = "Textures/Gameplay/Obstacles/Spikes/Spike.png";

// Audio
const mkString MKSpike::SPIKE_HIT_SOUND_NAME = "Spike_Hit";

// Constructor(s) & Destructor
MKSpike::MKSpike(MKScene* _scene)
    : MKObstacle(_scene)
{
}

MKSpike::~MKSpike()
{
    MK_CALL_DEINITIALISE_CONTACT_LISTENER
}

// Collision
mkBool MKSpike::OnContactBegin(cocos2d::PhysicsContact& _contact)
{
    if (!Super::OnContactBegin(_contact)) { return false; }

    // Stop listening or else this function will stil get called.
    MK_CALL_DEINITIALISE_CONTACT_LISTENER

        // Remove our PhysicsBody. We no longer need to collide.
        removeComponent(getPhysicsBody());

    // Play the hit sound.
    MKAudioManager::GetInstance()->PlaySound(SPIKE_HIT_SOUND_NAME, MKSound::SoundType::SFX);

    return true;
}

// Static Function(s)
MKSpike* MKSpike::Create(MKScene* _scene, mkU32 _spikeCount)
{
    MKSpike* obstacle = new (std::nothrow) MKSpike(_scene);
    if (obstacle && obstacle->init(_spikeCount))
    {
        obstacle->autorelease();
        return obstacle;
    }

    CC_SAFE_DELETE(obstacle);
    return nullptr;
}

// Interface Function(s)
void MKSpike::SetSpikeCount(mkU32 _spikeCount)
{
    m_SpikeCount = _spikeCount;
    m_Spike->SetTextureScale((mkF32)m_SpikeCount, 1.0f);
    setScaleX(getScaleY() * (mkF32)m_SpikeCount);
}

mkU32 MKSpike::GetSpikeCount() const
{
    return m_SpikeCount;
}

// Virtual Override(s)
mkBool MKSpike::init(mkU32 _spikeCount)
{
    if (!Super::init()) { return false; }

    Size visibleSize = Director::getInstance()->getVisibleSize();

    // Set Variable(s)
    m_SpikeCount = _spikeCount;

    // Create the spike.
    m_Spike = MKSprite::CreateWithFile(SPIKE_SPRITE_FILE, MKSprite::REPEAT);
    m_Spike->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_Spike->SetTextureScale((mkF32)m_SpikeCount, 1.0f);
    addChild(m_Spike);

    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setContentSize(m_Spike->getContentSize());
    m_Spike->setPosition(getContentSize() * 0.5f);

    // Create the PhysicsBody.
    mkF32 physicsBodyWidth = getContentSize().width * static_cast<mkF32>(m_SpikeCount - 1) / static_cast<mkF32>(m_SpikeCount);
    mkF32 physicsBodyHeight = getContentSize().height;
    cocos2d::PhysicsBody* physicsBody = PhysicsBody::createBox(Size(physicsBodyWidth, physicsBodyHeight));
    physicsBody->setDynamic(false);
    physicsBody->setCategoryBitmask(MK_COLLISION_CATEGORY_OBSTACLE);
    physicsBody->setContactTestBitmask(MK_COLLISION_CATEGORY_PLAYER);
    physicsBody->setCollisionBitmask(MK_COLLISION_CATEGORY_NONE);
    setPhysicsBody(physicsBody);

    // Set the spike scale.
    mkF32 desiredScale = (visibleSize.height * 0.15f) / this->getContentSize().height;
    setScale(desiredScale * (mkF32)m_SpikeCount, desiredScale);

    MK_CALL_INITIALISE_CONTACT_LISTENER

    return true;
}

NS_MK_END