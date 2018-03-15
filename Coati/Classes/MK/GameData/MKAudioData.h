#ifndef MK_AUDIO_DATA_H
#define MK_AUDIO_DATA_H

// Include MK
#include "../Common/MKMacros.h"
#include "../Common/MKPasskey.h"
#include "MKGameData.h"

// Include STL
#include <unordered_map>

NS_MK_BEGIN

class MKAudioData : public MKGameData
{
private:
    // Variable(s)
    std::unordered_map<mkString, mkString> m_Sounds;

public:
    // JSON Data Name(s)
    static const mkString SOUND_ARRAY_JSON_DATA_NAME;
    static const mkString SOUND_NAME_JSON_DATA_NAME;
    static const mkString SOUND_FILE_LOCATION_JSON_DATA_NAME;
    
    // Constructor(s) & Destructor
    MKAudioData() {}
    virtual ~MKAudioData() {}

    // Virtual Function(s)
    virtual mkBool LoadData(const mkString& _filePath) override;
    virtual mkBool SaveData(const mkString& _filePath) override { return false; }

    virtual mkString GetWritablePath() const override { return ""; }
    virtual mkString GetCachedPath() const override;

    // Interface Function(s)
    std::unordered_map<mkString, mkString> GetSounds() { return m_Sounds; }
    const std::unordered_map<mkString, mkString> GetSounds() const { return m_Sounds; }
    mkString GetFileLocation(const mkString& _soundName) const;
};

NS_MK_END

#endif // MK_AUDIO_DATA_H