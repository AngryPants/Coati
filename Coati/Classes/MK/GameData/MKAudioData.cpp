// Include MK
#include "MKAudioData.h"
#include "../Common/MKJSONHelper.h"

NS_MK_BEGIN

// JSON Data Name(s)
const mkString MKAudioData::SOUND_ARRAY_JSON_DATA_NAME = "Sounds";
const mkString MKAudioData::SOUND_NAME_JSON_DATA_NAME = "Name";
const mkString MKAudioData::SOUND_FILE_LOCATION_JSON_DATA_NAME = "File_Location";

// Virtual Function(s)
mkBool MKAudioData::LoadData(const mkString& _filePath)
{
    // Load the JSON file.
    RAPIDJSON_NAMESPACE::Document dataDocument;
    if (!MKJSONHelper::LoadFromJSON(dataDocument, _filePath)) { return false; }

    // Load Sounds
    auto soundArray = dataDocument.FindMember(SOUND_ARRAY_JSON_DATA_NAME.c_str());
    MK_ASSERT(soundArray->value.IsArray());
    m_Sounds.clear();
    for (RAPIDJSON_NAMESPACE::Value::ConstValueIterator i = soundArray->value.Begin(); i != soundArray->value.End(); ++i)
    {
        // Sound Name
        auto soundName = i->FindMember(SOUND_NAME_JSON_DATA_NAME.c_str());
        MK_ASSERT(soundName->value.IsString());

        // File Location
        auto fileLocation = i->FindMember(SOUND_FILE_LOCATION_JSON_DATA_NAME.c_str());
        MK_ASSERT(fileLocation->value.IsString());

        // Add Sound to array.
        MK_ASSERT((m_Sounds.find(soundName->value.GetString()) == m_Sounds.end())); // Ensure that no 2 sounds have the same name.
        m_Sounds.insert(std::pair<mkString, mkString>(soundName->value.GetString(), fileLocation->value.GetString()));
    }

    return true;
}

mkString MKAudioData::GetCachedPath() const
{
    return cocos2d::FileUtils::getInstance()->fullPathForFilename("Audio/AudioData.json");
}

// Interface Function(s)
mkString MKAudioData::GetFileLocation(const mkString& _soundName) const
{
    std::unordered_map<mkString, mkString>::const_iterator iter = m_Sounds.find(_soundName);
    return iter == m_Sounds.end() ? "" : iter->second;
}

NS_MK_END