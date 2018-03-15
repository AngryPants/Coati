#ifndef MK_SPRITE_ANIMATION_TICKET_H
#define MK_SPRITE_ANIMATION_TICKET_H

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "../../Common/MKMacros.h"
#include "../../Common/MKAssertions.h"

class MKSpriteAnimationTicket : public cocos2d::Ref
{
private:
    // Variable(s)
    mkString m_StateName;
    mkF32 m_Duration;
    mkS32 m_LoopCount;
    mkBool m_DestroyOnFinish;

public:
    static const mkS32 INFINITE_LOOPS = -1;

    // Static Function(s)
    static MKSpriteAnimationTicket* Create(mkString _stateName, mkF32 _duration, mkS32 _loopCount = 1, mkBool _destroyOnFinish = false)
    {
        MKSpriteAnimationTicket* ticket = new MKSpriteAnimationTicket(_stateName, _duration, _loopCount, _destroyOnFinish);
        ticket->autorelease();

        return ticket;
    }

    // Constructor(s) & Destructor
    MKSpriteAnimationTicket(mkString _stateName, mkF32 _duration, mkS32 _loopCount, mkBool _destroyOnFinish)
        : m_StateName(_stateName), m_Duration(_duration), m_LoopCount(_loopCount), m_DestroyOnFinish(_destroyOnFinish)
    {
        MK_ASSERT(_duration > 0.0f);
        MK_ASSERT((_loopCount >= 0) || (_loopCount == INFINITE_LOOPS));
        MK_ASSERT(!((_loopCount == INFINITE_LOOPS) && _destroyOnFinish));
    }
    virtual ~MKSpriteAnimationTicket() {}

    // Interface Function(s)
    const mkString& GetStateName() const { return m_StateName; }
    mkF32 GetDuration() const { return m_Duration; }
    mkS32 GetLoopCount() const { return m_LoopCount; }
    mkBool GetDestroyOnFinish() const { return m_DestroyOnFinish; } // This only works if the LoopCount is not infinite.
};

#endif // MK_SPRITE_ANIMATION_TICKET_H