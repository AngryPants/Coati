#ifndef MK_SPRITE_ANIMATION_STATE_H
#define MK_SPRITE_ANIMATION_STATE_H

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "MK/Common/MKMacros.h"

// Include STL
#include <vector>

NS_MK_BEGIN

class MKSpriteAnimationState
{
private:
    mkString m_Name;
    std::vector<mkString> m_Frames;
    cocos2d::Animation* m_Animation;

public:
    // Constructor(s) & Destructor
    MKSpriteAnimationState(const mkString& _name, const std::vector<mkString>& _frames);
    ~MKSpriteAnimationState();

    // Interface Function(s)
    const mkString& GetName() const { return m_Name; }
    const std::vector<mkString>& GetFrames() const { return m_Frames; }
    mkU32 GetFrameCount() const { return m_Frames.size(); }
    const mkString& GetFrame(mkU32 _frameIndex) const { return m_Frames[_frameIndex]; }
    cocos2d::Animation* GetAnimation() { return m_Animation; }
    const cocos2d::Animation* GetAnimation() const { return m_Animation; }
};

NS_MK_END

#endif // MK_SPRITE_ANIMATION_STATE_H