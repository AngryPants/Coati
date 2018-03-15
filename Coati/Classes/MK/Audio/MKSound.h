#ifndef MK_SOUND_H
#define MK_SOUND_H

// Include MK
#include "../Common/MKMacros.h"
#include "../Common/MKMathsHelper.h"
#include "../Common/MKPasskey.h"
#include "../Common/MKAssertions.h"

NS_MK_BEGIN

// Forward Declaration(s)
class MKAudioManager;

class MKSound
{
public:
    // Enum(s)
    enum SoundType
    {
        SFX = 0,
        AMBIENT,
        BGM,

        NUM_SOUNDTYPE,
    };

    // Static Variable(s)
    static const mkS32 INVALID_ID = -1;

private:
    // Variable(s)
    const mkString m_Name;
    const mkS32 m_Id;
    const SoundType m_SoundType;
    mkF32 m_Volume;

public:
    // Constructor(s) & Destructor
    MKSound(MKPasskey<MKAudioManager> _passKey, const mkString& _name, mkS32 _id, SoundType _soundType, mkF32 _volume = 1.0f)
        : m_Name(_name), m_Id(_id), m_SoundType(_soundType), m_Volume(_volume)
    {
        // Validate the SoundType.
        MK_ASSERT((_soundType != SoundType::NUM_SOUNDTYPE));
    }
    ~MKSound() {}

    // Static Function(s)
    static mkF32 GetMinVolume() { return 0.0f; }
    static mkF32 GetMaxVolume() { return 1.0f; }

    // Function(s)
    const mkString& GetName() const { return m_Name; }
    mkS32 GetId() const { return m_Id; }
    SoundType GetSoundType() const { return m_SoundType; }

    void SetVolume(mkF32 _volume) { m_Volume = MKMathsHelper::Clamp<mkF32>(_volume, GetMinVolume(), GetMaxVolume()); }
    mkF32 GetVolume() const { return m_Volume; }
};

NS_MK_END

#endif // MK_SOUND_H