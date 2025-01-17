#ifndef MK_REPEATACTIONINSTANTFOREVER_H
#define MK_REPEATACTIONINSTANTFOREVER_H

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "MK/Common/MKMathsHelper.h"

// Include STL
#include <cmath>

// Using Namespace(s)
USING_NS_CC;

NS_MK_BEGIN

class MKRepeatActionInstantForever : public cocos2d::ActionInterval
{
    typedef cocos2d::ActionInterval Super;

protected:
    cocos2d::ActionInstant* m_InnerAction = nullptr;

public:
    static MKRepeatActionInstantForever* Create(cocos2d::ActionInstant* _actionInstant)
    {
        MKRepeatActionInstantForever* newAction = new (std::nothrow) MKRepeatActionInstantForever();
        if (newAction && newAction->initWithAction(_actionInstant))
        {
            newAction->autorelease();
            return newAction;
        }

        CC_SAFE_DELETE(newAction);
        return nullptr;
    }

    /** Sets the inner action.
    *
    * @param action The inner action.
    */
    void setInnerAction(ActionInstant* _action)
    {
        if (_action != m_InnerAction)
        {
            CC_SAFE_RELEASE(m_InnerAction);
            m_InnerAction = _action;
            CC_SAFE_RETAIN(m_InnerAction);
        }
    }

    /** Gets the inner action.
    *
    * @return The inner action.
    */
    ActionInstant* getInnerAction()
    {
        return m_InnerAction;
    }

    void startWithTarget(cocos2d::Node* target) override
    {
        Super::startWithTarget(target);
        m_InnerAction->startWithTarget(target);
    }

    virtual void step(float _deltaTime) override
    {
        Super::step(_deltaTime);
        m_InnerAction->step(_deltaTime);
    }

    virtual void update(float _percentageComplete) override
    {
        Super::update(_percentageComplete);
        m_InnerAction->update(_percentageComplete);
    }

    virtual bool isDone() const override
    {
        return false;
    }

CC_CONSTRUCTOR_ACCESS:
    MKRepeatActionInstantForever() {}
    virtual ~MKRepeatActionInstantForever()
    {
        CC_SAFE_RELEASE(m_InnerAction);
    }

    mkBool initWithAction(cocos2d::ActionInstant* _actionInstant)
    {
        CCASSERT(_actionInstant != nullptr, "_actionInstant can't be nullptr!");
        if (_actionInstant == nullptr)
        {
            cocos2d::log("RepeatForever::initWithAction error: _actionInstant is nullptr!");
            return false;
        }

        _actionInstant->retain();
        m_InnerAction = _actionInstant;

        return true;
    }
};

NS_MK_END

#endif