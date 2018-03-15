#pragma once

// Include MK
#include "../Common/MKMacros.h"
#include "../Common/MKPasskey.h"
#include "../Audio/MKSound.h"
#include "MKGameData.h"

NS_MK_BEGIN

class MKAudioVolumeData : public MKGameData
{
private:
    mkF32 m_MasterVolume;
    mkF32 m_Volume[MKSound::SoundType::NUM_SOUNDTYPE];

public:
    // JSON Data Name(s)
    static const mkString MASTER_VOLUME_JSON_DATA_NAME;
    static const mkString SFX_VOLUME_JSON_DATA_NAME;
    static const mkString AMBIENT_VOLUME_JSON_DATA_NAME;
    static const mkString BGM_VOLUME_JSON_DATA_NAME;

    // Constructor(s) & Destructor
    MKAudioVolumeData();
    virtual ~MKAudioVolumeData();

    // Virtual Function(s)
    virtual mkBool LoadData(const mkString& _filePath) override;
    virtual mkBool SaveData(const mkString& _filePath) override;

    virtual mkString GetWritablePath() const override;
    virtual mkString GetCachedPath() const override { return ""; }

    // Interface Function(s)
    void SetMasterVolume(mkF32 _volume) { m_MasterVolume = MKMathsHelper::Clamp<mkF32>(_volume, MKSound::GetMinVolume(), MKSound::GetMaxVolume()); }
    mkF32 GetMasterVolume() const { return m_MasterVolume; }
    void SetVolume(MKSound::SoundType _soundType, mkF32 _volume) { m_Volume[_soundType] = MKMathsHelper::Clamp<mkF32>(_volume, MKSound::GetMinVolume(), MKSound::GetMaxVolume()); }
    mkF32 GetVolume(MKSound::SoundType _soundType) const { return m_Volume[_soundType]; }
};

NS_MK_END