#include "MKShield.h"
#include "../../MK/Gameplay/MKCollisionCategory.h"

NS_MK_BEGIN

// Sprite
const mkString MKShield::SHIELD_SPRITE_FILE = "Textures/Gameplay/Powerups/Shield/Shield.png";

// Constructor(s) & Destructor
MKShield::MKShield(MKScene* _scene)
    : MKPowerup(_scene)
{
}

MKShield::~MKShield()
{
    MK_CALL_DEINITIALISE_CONTACT_LISTENER;
}

// Collision Callback
mkBool MKShield::OnContactBegin(cocos2d::PhysicsContact& _contact)
{
    if (!Super::OnContactBegin(_contact)) { return false; }

    // Stop listening or else this function will stil get called.
    MK_CALL_DEINITIALISE_CONTACT_LISTENER;

    // Remove our PhysicsBody. We no longer need to collide.
    removeComponent(getPhysicsBody());

    removeChild(m_Shield, true);
    m_Shield = nullptr;

    return true;
}

// Static Function(s)
MKShield* MKShield::create(MKScene* _scene)
{
    MKShield* shield = new (std::nothrow) MKShield(_scene);
    if (shield && shield->init())
    {
        shield->autorelease();
        return shield;
    }

    CC_SAFE_DELETE(shield);
    return nullptr;
}

// Virtual Override(s)
mkBool MKShield::init()
{
    if (!Super::init()) { return false; }

    Size visibleSize = Director::getInstance()->getVisibleSize();

    // Create the Shield.
    m_Shield = MKSprite::CreateWithFile(SHIELD_SPRITE_FILE);
    m_Shield->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(m_Shield);

    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setContentSize(m_Shield->getContentSize());
    m_Shield->setPosition(getContentSize() * 0.5f);

    mkF32 desiredScale = (visibleSize.height * 0.1f) / this->getContentSize().height;
    setScale(desiredScale, desiredScale);

    // Create the PhysicsBody.
    cocos2d::PhysicsBody* physicsBody = PhysicsBody::createBox(getContentSize());
    physicsBody->setDynamic(false);
    physicsBody->setCategoryBitmask(MK_COLLISION_CATEGORY_SHIELD);
    physicsBody->setContactTestBitmask(MK_COLLISION_CATEGORY_PLAYER);
    physicsBody->setCollisionBitmask(MK_COLLISION_CATEGORY_NONE);
    setPhysicsBody(physicsBody);

    MK_CALL_INITIALISE_CONTACT_LISTENER;

    return true;
}

NS_MK_END