#ifndef MK_AUDIO_MANAGER_H
#define MK_AUDIO_MANAGER_H

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "../../MK/Common/MKMacros.h"
#include "../../MK/Common/MKSingletonTemplate.h"
#include "MKSound.h"

// Include JSON
#include "external/json/document.h"

// Include STL
#include <vector>
#include <unordered_map>

NS_MK_BEGIN

class MKAudioManager : public MKSingletonTemplate<MKAudioManager>
{
    friend class MKSingletonTemplate<MKAudioManager>;

private:
    // Member Variable(s)
    mkF32 m_MasterVolume;
    mkF32 m_Volumes[MKSound::NUM_SOUNDTYPE];
    std::unordered_map<mkS32, MKSound*> m_Sounds;

    // Constructor(s) & Destructor
    MKAudioManager();
    virtual ~MKAudioManager();
    
    // Function(s)
    void RemoveAllSounds();
    void RemoveSound(mkS32 _soundId);

    MKSound* GetSound(mkS32 _soundId);
    const MKSound* GetSound(mkS32 _soundId) const;

public:
    // Static Variable(s)
    static const mkS32 INVALID_SOUND_ID = -1; // Ensure that this is the same as AudioEngine::INVALID_AUDIO_ID.

    // Interface Function(s)
    mkBool SaveVolumes(); // Only the master volume and SoundType volumes are saved. Individual sound volumes are not saved.

    void SetMasterVolume(mkF32 _volume);
    mkF32 GetMasterVolume() const { return m_MasterVolume; }

    void SetVolume(MKSound::SoundType _soundType, mkF32 _volume);
    mkF32 GetVolume(MKSound::SoundType _soundType) const { return m_Volumes[_soundType]; }

    void SetVolume(mkS32 _soundId, mkF32 _volume);
    mkF32 GetVolume(mkS32 _soundId) const;
    void SetLoop(mkS32 _soundId, mkBool _loop);
    mkBool IsLooped(mkS32 _soundId) const;

    mkF32 GetSoundDuration(mkS32 _soundId) const;
    mkF32 GetSoundCurrentTime(mkS32 _soundId) const;
    mkBool SetSoundCurrentTime(mkS32 _soundId, mkF32 _seconds);

    mkS32 PlaySound(const mkString& _soundName, MKSound::SoundType _soundType, mkF32 _volume = 1.0f, mkBool _loop = false, const std::function<void(int, const std::string&)> _finishCallback = nullptr);
    void PauseSound(mkS32 _soundId);
    void ResumeSound(mkS32 _soundId);
    void StopSound(mkS32 _soundId);

    void PauseAllSounds();
    void ResumeAllSounds();
    void StopAllSounds();

    mkBool IsSoundPaused(mkS32 _soundId) const;
    mkBool IsSoundPlaying(mkS32 _soundId) const;
};

NS_MK_END

#endif // MK_AUDIO_MANAGER_H