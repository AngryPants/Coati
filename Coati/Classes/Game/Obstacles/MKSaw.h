#ifndef MK_SAW_H
#define MK_SAW_H

// Include MK
#include "MKObstacle.h"
#include "../../MK/Audio/MKAudioManager.h"

NS_MK_BEGIN

class MKSaw : public MKObstacle
{
    typedef MKObstacle Super;

protected:
    // Variable(s)
    MKSprite* m_Saw = nullptr;
    mkF32 m_RotationSpeed = 0.0f;
    mkF32 m_CycleDuration = 0.0f; // The time it takes for the saw to move along the sine curve once.
    mkF32 m_StartOffset;
    mkF32 m_MoveDisplacement = 0.0f; // The Amplitude of the sine movment.
    mkS32 m_SawSpinningSoundID = MKAudioManager::INVALID_SOUND_ID;

    // Collision
    MK_DECL_INITIALISE_CONTACT_LISTENER(MKSaw);
    MK_DECL_DEINITIALISE_CONTACT_LISTENER(MKSaw);
    virtual mkBool OnContactBegin(cocos2d::PhysicsContact& _contact) override;

public:
    // Sprite
    static const mkString SAW_SPRITE_FILE;

    // Audio
    static const mkString SAW_SPINNING_SOUND_NAME;
    static const mkString SAW_HIT_SOUND_NAME;

    // Static Function(s)
    static MKSaw* Create(MKScene* _scene, mkF32 _startOffset);

    // Interface Function(s)
    mkF32 GetStartOffset() const { return m_StartOffset; }
    mkF32 GetRotationSpeed() const { return m_RotationSpeed; }
    mkF32 GetCycleDuration() const { return m_CycleDuration; }
    mkF32 GetMoveDisplacement() const { return m_MoveDisplacement; }

CC_CONSTRUCTOR_ACCESS:
	// Constructor(s) & Destructor
    MKSaw(MKScene* _scene);
    virtual ~MKSaw();

    // Virtual Override(s)
    virtual mkBool init(mkF32 _startOffset);
    virtual void update(mkF32 _deltaTime) override;
    virtual void pause() override;
    virtual void resume() override;
};

NS_MK_END

#endif // MK_SAW_H