#pragma once

// Include MK
#include "MKPowerups.h"

NS_MK_BEGIN

class MKShield : public MKPowerup
{
    typedef MKPowerup Super;

protected:
    // Variable(s)
    MKSprite* m_Shield = nullptr;

    // Collision
    MK_DECL_INITIALISE_CONTACT_LISTENER(MKShield);
    MK_DECL_DEINITIALISE_CONTACT_LISTENER(MKShield);
    virtual mkBool OnContactBegin(cocos2d::PhysicsContact& _contact) override;

public:
    // Sprite
    static const mkString SHIELD_SPRITE_FILE;

    // Static Function(s)
    static MKShield* create(MKScene* _scene);

    // Constructor(s) & Destructor
    MKShield(MKScene* _scene);
    virtual ~MKShield();

    // Virtual Override(s)
    virtual mkBool init() override;
};

NS_MK_END