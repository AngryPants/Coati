#ifndef MK_SPIKE_H
#define MK_SPIKE_H

// Include MK
#include "MKObstacle.h"
#include "../../MK/Audio/MKAudioManager.h"

NS_MK_BEGIN

class MKSpike : public MKObstacle
{
	typedef MKObstacle Super;

protected:
    // Variable(s)
    MKSprite* m_Spike = nullptr;
    mkU32 m_SpikeCount = 2;

    // Collision
    MK_DECL_INITIALISE_CONTACT_LISTENER(MKSpike);
    MK_DECL_DEINITIALISE_CONTACT_LISTENER(MKSpike);
	virtual mkBool OnContactBegin(cocos2d::PhysicsContact& _contact) override;

public:
    // Sprite
    static const mkString SPIKE_SPRITE_FILE;

    // Audio
    static const mkString SPIKE_HIT_SOUND_NAME;

    // Static Function(s)
    static MKSpike* Create(MKScene* _scene, mkU32 _spikeCount);

    // Interface Function(s)
    void SetSpikeCount(mkU32 _spikeCount);
    mkU32 GetSpikeCount() const;

CC_CONSTRUCTOR_ACCESS:
	// Constructor(s) & Destructor
    MKSpike(MKScene* _scene);
    virtual ~MKSpike();

    // Virtual Override(s)
	virtual mkBool init(mkU32 _spikeCount);
};

NS_MK_END

#endif