#pragma once

// Include MK
#include "MK/Common/MKMacros.h"
#include "MK/Sprite/MKSprite.h"
#include "MK/Audio/MKAudioManager.h"

NS_MK_BEGIN

class MKMissileWarning : public MKSprite
{
    typedef MKSprite Super;

protected:
    // Variable(s)
    mkF32 m_WarningDuration;
    std::function<void(cocos2d::Node*)> m_FinishCallback = nullptr;

public:
    // Sprite
    static const mkString MISSILE_WARNING_SPRITE_FILE;

    // Audio
    static const mkString MISSILE_WARNING_SOUND_NAME;

    // Constructor(s) & Destructor
    MKMissileWarning();
    virtual ~MKMissileWarning();

    // Interface Function(s)
    void setFinishCallback(const std::function<void(cocos2d::Node*)>& _finishCallback) { m_FinishCallback = _finishCallback; }

    // Static Function(s)
    static MKMissileWarning* create(mkF32 _warningDuration, cocos2d::Node* _followNode, const Vec2& _offset, const std::function<void(cocos2d::Node*)>& _finishCallback);

    // Virtual Override(s)
    virtual mkBool init(mkF32 _warningDuration, cocos2d::Node* _followNode, const Vec2& _offset, const std::function<void(cocos2d::Node*)>& _finishCallback);
};

NS_MK_END