#ifndef MK_SPRITE_ANIMATION_H
#define MK_SPRITE_ANIMATION_H

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "../../Common/MKMacros.h"
#include "../MKSprite.h"
#include "MKSpriteAnimationState.h"
#include "MKSpriteAnimationTicket.h"

// Include STL
#include <unordered_map>
#include <queue>

// Using Namespace(s)
USING_NS_CC;
using namespace std;

NS_MK_BEGIN

class MKSpriteAnimation : public MKSprite
{
    typedef MKSprite Super;

protected:
    // Variable(s)
    std::unordered_map<mkString, MKSpriteAnimationState*> m_States;
    std::queue<MKSpriteAnimationTicket*> m_TicketQueue;
    cocos2d::Action* m_CurrentAnimation = nullptr;

    // Internal Function(s)
    mkBool LoadJSONFile(const mkString& _fileLocation);
    void ClearAllStates();

    void StopCurrentTicket();
    void PlayNextTicket();

    MKSpriteAnimationState* GetState(const mkString& _stateName);
    const MKSpriteAnimationState* GetState(const mkString& _stateName) const;

public:
    // JSON Data Names
    static const mkString STATE_ARRAY_JSON_DATA_NAME;
    static const mkString STATE_NAME_JSON_DATA_NAME;
    static const mkString SPRITE_FRAMES_JSON_DATA_NAME;

    // Interface Function(s)
    void AddTicketToQueue(MKSpriteAnimationTicket* _ticket);
    void SkipCurrentTicket();
    void ClearAllTickets();

    // Static Function(s)
    static MKSpriteAnimation* Create(const mkString& _pListFile, const mkString& _jsonFile);

CC_CONSTRUCTOR_ACCESS:
    // Constructor(s) & Destructor
    MKSpriteAnimation();
    virtual ~MKSpriteAnimation();

    // Virtual Function(s)
    virtual mkBool initSpriteAnimation(const mkString& _pListFile, const mkString& _jsonFile, const mkString& _vertexShader = DEFAULT_VERTEX_SHADER, const mkString& _fragmentShader = DEFAULT_FRAGMENT_SHADER);
};

NS_MK_END

#endif // MK_SPRITE_ANIMATION_H