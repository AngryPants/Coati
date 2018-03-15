// Include MK
#include "MKAudioManager.h"
#include "../GameData/MKGameDataLoader.h"
#include "../GameData/MKAudioData.h"
#include "../GameData/MKAudioVolumeData.h"

// Include JSON
#include "external/json/filewritestream.h"
#include "external/json/filereadstream.h"
#include "external/json/writer.h"

// Include Audio
#include "AudioEngine.h"

NS_MK_BEGIN

// Constructor(s) & Destructor
MKAudioManager::MKAudioManager()
{
    // Load the audio data.
    MKAudioVolumeData* audioVolumeData = MKGameDataLoader::GetInstance()->GetGameData<MKAudioVolumeData>();
    audioVolumeData->LoadData(audioVolumeData->GetWritablePath());
    m_MasterVolume = audioVolumeData->GetMasterVolume();
    for (mkU32 i = 0; i < MKSound::SoundType::NUM_SOUNDTYPE; ++i)
    {
        m_Volumes[i] = audioVolumeData->GetVolume(static_cast<MKSound::SoundType>(i));
    }

    MKAudioData* audioData = MKGameDataLoader::GetInstance()->GetGameData<MKAudioData>();
    audioData->LoadData(audioData->GetCachedPath());
}

MKAudioManager::~MKAudioManager()
{
    StopAllSounds();
}

// Function(s)
void MKAudioManager::RemoveAllSounds()
{
    for (std::unordered_map<mkS32, MKSound*>::iterator iter = m_Sounds.begin(); iter != m_Sounds.end(); ++iter)
    {
        delete iter->second;
    }

    m_Sounds.clear();
}

void MKAudioManager::RemoveSound(mkS32 _soundId)
{
    std::unordered_map<mkS32, MKSound*>::iterator iter = m_Sounds.find(_soundId);
    if (iter != m_Sounds.end())
    {
        delete iter->second;
        m_Sounds.erase(iter);
    }
}

MKSound* MKAudioManager::GetSound(mkS32 _soundId)
{
    std::unordered_map<mkS32, MKSound*>::iterator iter = m_Sounds.find(_soundId);
    return iter == m_Sounds.end() ? nullptr : iter->second;
}

const MKSound* MKAudioManager::GetSound(mkS32 _soundId) const
{
    std::unordered_map<mkS32, MKSound*>::const_iterator iter = m_Sounds.find(_soundId);
    return iter == m_Sounds.end() ? nullptr : iter->second;
}

// Interface Function(s)
mkBool MKAudioManager::SaveVolumes()
{
    MKAudioVolumeData* audioVolumeData = MKGameDataLoader::GetInstance()->GetGameData<MKAudioVolumeData>();
    audioVolumeData->SetMasterVolume(m_MasterVolume);
    for (mkU32 i = 0; i < MKSound::SoundType::NUM_SOUNDTYPE; ++i)
    {
        audioVolumeData->SetVolume(static_cast<MKSound::SoundType>(i), m_Volumes[i]);
    }

    return audioVolumeData->SaveData(audioVolumeData->GetWritablePath());
}

void MKAudioManager::SetMasterVolume(mkF32 _volume)
{
    m_MasterVolume = MKMathsHelper::Clamp<mkF32>(_volume, MKSound::GetMinVolume(), MKSound::GetMaxVolume());

    for (std::unordered_map<mkS32, MKSound*>::iterator iter = m_Sounds.begin(); iter != m_Sounds.end(); ++iter)
    {
        MKSound* sound = iter->second;

        // The actual volume output is determined by the sound type, master volume and individual sound volume.
        cocos2d::experimental::AudioEngine::setVolume(sound->GetId(), sound->GetVolume() * GetVolume(sound->GetSoundType()) * m_MasterVolume);
    }
}

void MKAudioManager::SetVolume(MKSound::SoundType _soundType, mkF32 _volume)
{
    m_Volumes[_soundType] = MKMathsHelper::Clamp<mkF32>(_volume, MKSound::GetMinVolume(), MKSound::GetMaxVolume());

    for (std::unordered_map<mkS32, MKSound*>::iterator iter = m_Sounds.begin(); iter != m_Sounds.end(); ++iter)
    {
        MKSound* sound = iter->second;
        if (sound->GetSoundType() != _soundType) { continue; }

        // The actual volume output is determined by the sound type, master volume and individual sound volume.
        cocos2d::experimental::AudioEngine::setVolume(sound->GetId(), sound->GetVolume() * m_Volumes[_soundType] * m_MasterVolume);
    }
}

void MKAudioManager::SetVolume(mkS32 _soundId, mkF32 _volume)
{
    MKSound* sound = GetSound(_soundId);
    if (sound == nullptr) { return; }

    // Update the volume of the sound.
    sound->SetVolume(MKMathsHelper::Clamp<mkF32>(_volume, MKSound::GetMinVolume(), MKSound::GetMaxVolume()));

    // The actual volume output is determined by the sound type, master volume and individual sound volume.
    cocos2d::experimental::AudioEngine::setVolume(_soundId, sound->GetVolume() * GetVolume(sound->GetSoundType()) * m_MasterVolume);
}

mkF32 MKAudioManager::GetVolume(mkS32 _soundId) const
{
    const MKSound* sound = GetSound(_soundId);
    return sound == nullptr ? 0.0f : sound->GetVolume();
}

void MKAudioManager::SetLoop(mkS32 _soundId, mkBool _loop)
{
    return cocos2d::experimental::AudioEngine::setLoop(_soundId, _loop);
}

mkBool MKAudioManager::IsLooped(mkS32 _soundId) const
{
    return cocos2d::experimental::AudioEngine::isLoop(_soundId);
}

mkF32 MKAudioManager::GetSoundDuration(mkS32 _soundId) const
{
    return cocos2d::experimental::AudioEngine::getDuration(_soundId);
}

mkF32 MKAudioManager::GetSoundCurrentTime(mkS32 _soundId) const
{
    return cocos2d::experimental::AudioEngine::getCurrentTime(_soundId);
}

mkBool MKAudioManager::SetSoundCurrentTime(mkS32 _soundId, mkF32 _seconds)
{
    return cocos2d::experimental::AudioEngine::setCurrentTime(_soundId, _seconds);
}

mkS32 MKAudioManager::PlaySound(const mkString& _soundName, MKSound::SoundType _soundType, mkF32 _volume, mkBool _loop, const std::function<void(int, const std::string&)> _finishCallback)
{
    MKAudioData* audioData = MKGameDataLoader::GetInstance()->GetGameData<MKAudioData>();
    mkString fileLocation = audioData->GetFileLocation(_soundName);
    mkS32 soundId = cocos2d::experimental::AudioEngine::play2d(fileLocation, _loop, _volume * m_Volumes[_soundType] * m_MasterVolume);

    // Check if the sound failed to play.
    if (soundId == cocos2d::experimental::AudioEngine::INVALID_AUDIO_ID)
    {
        return INVALID_SOUND_ID;
    }

    // Add the sound to m_PlayingSounds.
    MKSound* sound = new MKSound({}, _soundName, soundId, _soundType, _volume);
    // If there was already a sound with this id, remove it.
    RemoveSound(soundId);
    // Add the sound to m_Sounds.
    m_Sounds.insert(std::pair<mkS32, MKSound*>(soundId, sound));

    // Register the finish callback.
    cocos2d::experimental::AudioEngine::setFinishCallback(
        soundId,
        [_finishCallback, this](mkS32 _soundId, const mkString& _fileLocation) -> void
        {
            RemoveSound(_soundId);
            if (_finishCallback != nullptr)
            {
                _finishCallback(_soundId, _fileLocation);
            }
        }
    );

    return soundId;
}

void MKAudioManager::PauseSound(mkS32 _soundId)
{
    if (_soundId == INVALID_SOUND_ID) { return; }
    cocos2d::experimental::AudioEngine::pause(_soundId);
}

void MKAudioManager::ResumeSound(mkS32 _soundId)
{
    if (_soundId == INVALID_SOUND_ID) { return; }
    cocos2d::experimental::AudioEngine::resume(_soundId);
}

void MKAudioManager::StopSound(mkS32 _soundId)
{
    if (_soundId == INVALID_SOUND_ID) { return; }
    cocos2d::experimental::AudioEngine::stop(_soundId);
    RemoveSound(_soundId);
}

void MKAudioManager::PauseAllSounds()
{
    cocos2d::experimental::AudioEngine::pauseAll();
}

void MKAudioManager::ResumeAllSounds()
{
    cocos2d::experimental::AudioEngine::resumeAll();
}

void MKAudioManager::StopAllSounds()
{
    cocos2d::experimental::AudioEngine::stopAll();
    RemoveAllSounds();
}

mkBool MKAudioManager::IsSoundPaused(mkS32 _soundId) const
{
    return (cocos2d::experimental::AudioEngine::getState(_soundId) == cocos2d::experimental::AudioEngine::AudioState::PAUSED);
}

mkBool MKAudioManager::IsSoundPlaying(mkS32 _soundId) const
{
    return (cocos2d::experimental::AudioEngine::getState(_soundId) == cocos2d::experimental::AudioEngine::AudioState::PLAYING);
}

NS_MK_END