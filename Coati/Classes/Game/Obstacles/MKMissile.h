#ifndef MK_MISSILE_H
#define MK_MISSILE_H

// Include MK
#include "MKObstacle.h"
#include "../../MK/Audio/MKAudioManager.h"
#include "../../MK/Input/MKInput.h"
#include "../../MK/Input/MKInputMacros.h"

NS_MK_BEGIN

class MKMissile : public MKObstacle
{
    typedef MKObstacle Super;

protected:
    // Variable(s)
    MKSprite* m_Missile = nullptr;
    ParticleSmoke* m_ParticleSmoke = nullptr;
    mkF32 m_HorizontalVelocity = 0.0f;

    // Sound IDs
    mkS32 m_MissileFlightSoundID = MKAudioManager::INVALID_SOUND_ID;

    // Collision
    MK_DECL_INITIALISE_CONTACT_LISTENER(MKMissile);
    MK_DECL_DEINITIALISE_CONTACT_LISTENER(MKMissile);
    virtual mkBool OnContactBegin(cocos2d::PhysicsContact& _contact) override;

public:
    // Sprite
    static const mkString MISSILE_SPRITE_FILE;

    // Sprite Animation
    static const mkString EXPLOSION_PLIST_FILE;
    static const mkString EXPLOSION_JSON_FILE;

    // Audio
    static const mkString MISSILE_FLIGHT_SOUND_NAME;
    static const mkString MISSILE_EXPLOSION_SOUND_NAME;
    static const mkString MISSILE_WARNING_SOUND_NAME;

    // Static Function(s)
    static MKMissile* Create(MKScene* _scene);

    // Interface Function(s)
    mkF32 GetHorizontalVelocity();
    virtual void pause() override;
    virtual void resume() override;

CC_CONSTRUCTOR_ACCESS:
    // Constructor(s) & Destructor
    MKMissile(MKScene* _scene);
    virtual ~MKMissile();

    // Virtual Function(s)
    virtual mkBool init() override;
    virtual void update(mkF32 _deltaTime) override;
};

NS_MK_END

#endif // MK_MISSILE_H