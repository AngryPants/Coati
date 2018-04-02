#pragma once

// Include MK
#include "MKObstacle.h"
#include "MK/Audio/MKAudioManager.h"
#include "MK/Input/MKInput.h"
#include "MK/Input/MKInputMacros.h"

NS_MK_BEGIN

class MKLaser : public MKObstacle
{
    typedef MKObstacle Super;

protected:
    // Variable(s)
    MKSprite* m_LaserGunLeft = nullptr;
    MKSprite* m_LaserGunRight = nullptr;
    cocos2d::ParticleSystem* m_ParticlesLeft = nullptr;
    cocos2d::ParticleSystem* m_ParticlesRight = nullptr;
    MKSprite* m_LaserWarningLine = nullptr;
    MKSprite* m_LaserWarningSparkLeft = nullptr;
    MKSprite* m_LaserWarningSparkRight = nullptr;

    mkS32 m_LaserGunZPriority = 1;
    mkS32 m_LaserBeamZPriority = 0;

    mkS32 m_LaserBeamChargingSoundID = MKAudioManager::INVALID_SOUND_ID;
    mkS32 m_LaserBeamShootingSoundID = MKAudioManager::INVALID_SOUND_ID;

    virtual void SpawnLaserGuns();
    virtual void SpawnParticles();
    virtual void DespawnParticles();
    virtual void SpawnLaserWarning();
    virtual void DespawnLaserWarning();
    virtual void SpawnLaserBeam();
    virtual void SpawnPhysicsBody();
    virtual void DespawnPhysicsBody();

    // Collision
    MK_DECL_INITIALISE_CONTACT_LISTENER(MKLaser);
    MK_DECL_DEINITIALISE_CONTACT_LISTENER(MKLaser);
    virtual mkBool OnContactBegin(cocos2d::PhysicsContact& _contact) override;

public:
    static const mkF32 LASER_MOVE_DOWN_DURATION;
    static const mkF32 LASER_MOVE_UP_DURATION;
    static const mkF32 LASER_BEAM_CHARGE_DURATION;
    static const mkF32 LASER_BEAM_SHOOT_DURATION;

    // Sprite
    static const mkString LASER_GUN_SPRITE_FILE;
    static const mkString LASER_WARNING_LINE_SPRITE_FILE;
    static const mkString LASER_WARNING_SPARK_SPRITE_FILE;

    // Sprite Animation
    static const mkString LASER_BEAM_PLIST_FILE;
    static const mkString LASER_BEAM_JSON_FILE;

    // Audio
    static const mkString LASER_CHARGING_SOUND_NAME;
    static const mkString LASER_SHOOTING_SOUND_NAME;

    // Static Function(s)
    static MKLaser* Create(MKScene* _scene, mkF32 _startDelay);

CC_CONSTRUCTOR_ACCESS:
    // Constructor(s) & Destructor
    MKLaser(MKScene* _scene) : MKObstacle(_scene) {}
    virtual ~MKLaser() {}

    // Virtual Override(s)
    virtual mkBool init(mkF32 _startDelay);
    virtual void pause() override;
    virtual void resume() override;
};

NS_MK_END