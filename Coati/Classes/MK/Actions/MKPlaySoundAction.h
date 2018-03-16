#ifndef MK_PLAYSOUNDACTION_H
#define MK_PLAYSOUNDACTION_H

// Include Cocos
#include "cocos2d.h"

// Include GT
#include "MK/Audio/MKAudioManager.h"

USING_NS_CC;

NS_MK_BEGIN

class MKPlaySoundAction : public ActionInstant
{
    typedef ActionInstant Super;

private:
    const mkString m_SoundName;
    const MKSound::SoundType m_SoundType;
    mkF32 m_Volume;
    mkBool m_Loop;

    CC_DISALLOW_COPY_AND_ASSIGN(MKPlaySoundAction);

public:
    static MKPlaySoundAction* Create(const mkString& _soundName, MKSound::SoundType _soundType, mkF32 _volume, mkBool _loop)
    {
        MKPlaySoundAction* newAction = new (std::nothrow) MKPlaySoundAction(_soundName, _soundType, _volume, _loop);
        if (newAction)
        {
            newAction->autorelease();
            return newAction;
        }

        CC_SAFE_DELETE(newAction);
        return NULL;
    }

    // SIMI LANJIAO! The original parameter is called _time.
    // _time your head lah _time. Chao Ji Bai. It is the percentage of 
    // the action done lah! Like if the duration is 10 seconds, and 4 seconds has passed,
    // the value is 0.4.
    virtual void update(mkF32 _percentageComplete)
    {
        Super::update(_percentageComplete);
        MKAudioManager::GetInstance()->PlaySound(m_SoundName, m_SoundType, m_Volume, m_Loop);
    }

CC_CONSTRUCTOR_ACCESS:
    MKPlaySoundAction(const mkString& _soundName, MKSound::SoundType _soundType, mkF32 _volume, mkBool _loop)
        : m_SoundName(_soundName), m_SoundType(_soundType), m_Volume(_volume), m_Loop(_loop)
    {}
    virtual ~MKPlaySoundAction() {}
};

NS_MK_END

#endif // MK_PLAYSOUNDACTION_H