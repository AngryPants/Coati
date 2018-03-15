#ifndef MK_FOLLOWSPRITE_H
#define MK_FOLLOWSPRITE_H

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "../../MK/Common/MKMathsHelper.h"

// Include STL
#include <cmath>

NS_MK_BEGIN

class MKFollowNodeAction : public cocos2d::ActionInterval
{
    typedef cocos2d::ActionInterval Super;

public:
    enum FollowAxis
    {
        X,
        Y,
        ALL,
    };

protected:
    FollowAxis m_FollowAxis = FollowAxis::ALL;
    cocos2d::Node* m_FollowedNode = nullptr;
    cocos2d::Vec2 m_StartOffset;

public:
    static MKFollowNodeAction* Create(mkF32 _duration, cocos2d::Node* _followedNode, FollowAxis _followAxis, const cocos2d::Vec2& _offset = cocos2d::Vec2::ZERO)
    {
        MKFollowNodeAction* newAction = new (std::nothrow) MKFollowNodeAction();
        if (newAction && newAction->initWithDuration(_duration, _followedNode, _followAxis, _offset))
        {
            newAction->autorelease();
            return newAction;
        }

        CC_SAFE_DELETE(newAction);
        return nullptr;
    }

    virtual void update(float _percentageComplete)
    {
        Super::update(_percentageComplete);
        if (_target == nullptr || m_FollowedNode == nullptr) { return; }

        cocos2d::Vec2 followNodePosition = m_FollowedNode->getPosition();
        cocos2d::Vec2 targetNodePosition = _target->getPosition();

        switch (m_FollowAxis)
        {
        case X:
            _target->setPosition(cocos2d::Vec2(followNodePosition.x, targetNodePosition.y) + m_StartOffset);
            break;
        case Y:
            _target->setPosition(cocos2d::Vec2(targetNodePosition.x, followNodePosition.y) + m_StartOffset);
            break;
        case ALL:
            _target->setPosition(followNodePosition + m_StartOffset);
            break;
        default:
            CC_ASSERT(0);
            break;
        }
    }

CC_CONSTRUCTOR_ACCESS:
    MKFollowNodeAction() {}
    virtual ~MKFollowNodeAction() {}

    mkBool initWithDuration(mkF32 _duration, cocos2d::Node* _followedNode, FollowAxis _followAxis, const cocos2d::Vec2& _offset)
    {
        if (!Super::initWithDuration(_duration)) { return false; }

        m_FollowedNode = _followedNode;
        m_StartOffset = _offset;
        m_FollowAxis = _followAxis;

        return true;
    }
};

NS_MK_END

#endif // MK_FOLLOWSPRITE_H