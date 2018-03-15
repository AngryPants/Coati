#ifndef __SETTINGS_SCENE_H__
#define __SETTINGS_SCENE_H__

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "MenuTemplate.h"
#include "MK/Sprite/MKSprite.h"
#include "MK/Audio/MKAudioManager.h"

USING_NS_CC;
USING_NS_MK

class Settings : public MenuTemplate
{
    typedef MenuTemplate Super;

protected:
    // Sound Names(s)
    const mkString m_BGMSoundName = "Settings_BGM";
    const mkString m_SFXSoundName = "Settings_SFX";

    // Sound IDs
    mkS32 m_BGMSoundId = MKAudioManager::INVALID_SOUND_ID;
    mkS32 m_SFXSoundId = MKAudioManager::INVALID_SOUND_ID;
    mkF32 timeTest = 0.0f;

    void InitialiseVolumeControls();

public:
    Settings() {}
    virtual ~Settings() {}
    CREATE_FUNC(Settings);
    
    virtual bool init();
    virtual void onEnter() override;
    virtual void onExit() override;
};

#endif // __SETTINGS_SCENE_H__