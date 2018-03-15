// Include MK
#include "MKAudioVolumeData.h"
#include "../Common/MKJSONHelper.h"

NS_MK_BEGIN

// JSON Data Name(s)
const mkString MKAudioVolumeData::MASTER_VOLUME_JSON_DATA_NAME = "Master";
const mkString MKAudioVolumeData::SFX_VOLUME_JSON_DATA_NAME = "SFX";
const mkString MKAudioVolumeData::AMBIENT_VOLUME_JSON_DATA_NAME = "AMBIENT";
const mkString MKAudioVolumeData::BGM_VOLUME_JSON_DATA_NAME = "BGM";

// Constructor(s) & Destructor
MKAudioVolumeData::MKAudioVolumeData()
{
    m_MasterVolume = MKSound::GetMaxVolume();
    for (mkU32 i = 0; i < sizeof(m_Volume) / sizeof(m_Volume[0]); ++i)
    {
        m_Volume[i] = MKSound::GetMaxVolume();
    }
}

MKAudioVolumeData::~MKAudioVolumeData()
{
}

// Virtual Function(s)
mkBool MKAudioVolumeData::LoadData(const mkString& _filePath)
{
    // Load the JSON file.
    RAPIDJSON_NAMESPACE::Document dataDocument;
    if (!MKJSONHelper::LoadFromJSON(dataDocument, _filePath)) { return false; }

    // Load Master Volume
    {
        auto volume = dataDocument.FindMember(MASTER_VOLUME_JSON_DATA_NAME.c_str());
        if (!volume->value.IsFloat()) { return false; }
        SetMasterVolume(volume->value.GetFloat());
    }

    // Load SFX Volume
    {
        auto volume = dataDocument.FindMember(SFX_VOLUME_JSON_DATA_NAME.c_str());
        if (!volume->value.IsFloat()) { return false; }
        SetVolume(MKSound::SoundType::SFX, volume->value.GetFloat());
    }

    // Load Ambient Volume
    {
        auto volume = dataDocument.FindMember(AMBIENT_VOLUME_JSON_DATA_NAME.c_str());
        if (!volume->value.IsFloat()) { return false; }
        SetVolume(MKSound::SoundType::AMBIENT, volume->value.GetFloat());
    }

    // Load BGM Volume
    {
        auto volume = dataDocument.FindMember(BGM_VOLUME_JSON_DATA_NAME.c_str());
        if (!volume->value.IsFloat()) { return false; }
        SetVolume(MKSound::SoundType::BGM, volume->value.GetFloat());
    }

    return true;
}

mkBool MKAudioVolumeData::SaveData(const mkString& _filePath)
{
    RAPIDJSON_NAMESPACE::Document dataDocument;
    dataDocument.SetObject();

    // Save Master Volume
    {
        RAPIDJSON_NAMESPACE::Value volumeName;
        volumeName.SetString(MASTER_VOLUME_JSON_DATA_NAME.c_str(), MASTER_VOLUME_JSON_DATA_NAME.length());
        RAPIDJSON_NAMESPACE::Value volume;
        volume.SetFloat(m_MasterVolume);
        dataDocument.AddMember(volumeName, volume, dataDocument.GetAllocator());
    }

    // Save SFX Volume
    {
        RAPIDJSON_NAMESPACE::Value volumeName;
        volumeName.SetString(SFX_VOLUME_JSON_DATA_NAME.c_str(), SFX_VOLUME_JSON_DATA_NAME.length());
        RAPIDJSON_NAMESPACE::Value volume;
        volume.SetFloat(m_Volume[MKSound::SoundType::SFX]);
        dataDocument.AddMember(volumeName, volume, dataDocument.GetAllocator());
    }

    // Save Ambient Volume
    {
        RAPIDJSON_NAMESPACE::Value volumeName;
        volumeName.SetString(AMBIENT_VOLUME_JSON_DATA_NAME.c_str(), AMBIENT_VOLUME_JSON_DATA_NAME.length());
        RAPIDJSON_NAMESPACE::Value volume;
        volume.SetFloat(m_Volume[MKSound::SoundType::AMBIENT]);
        dataDocument.AddMember(volumeName, volume, dataDocument.GetAllocator());
    }

    // Save BGM Volume
    {
        RAPIDJSON_NAMESPACE::Value volumeName;
        volumeName.SetString(BGM_VOLUME_JSON_DATA_NAME.c_str(), BGM_VOLUME_JSON_DATA_NAME.length());
        RAPIDJSON_NAMESPACE::Value volume;
        volume.SetFloat(m_Volume[MKSound::SoundType::BGM]);
        dataDocument.AddMember(volumeName, volume, dataDocument.GetAllocator());
    }

    return MKJSONHelper::JsonToFile(dataDocument, _filePath);
}

mkString MKAudioVolumeData::GetWritablePath() const
{
    return cocos2d::FileUtils::getInstance()->getWritablePath() + "AudioVolumeData.json";
}

NS_MK_END