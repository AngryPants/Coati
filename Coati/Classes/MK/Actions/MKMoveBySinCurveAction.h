#ifndef MK_MOVE_BY_SIN_CURVE_ACTION_H
#define MK_MOVE_BY_SIN_CURVE_ACTION_H

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "MK/Common/MKMathsHelper.h"

// Include STL
#include <cmath>

NS_MK_BEGIN

class MKMoveBySinCurveAction : public cocos2d::ActionInterval
{
    typedef cocos2d::ActionInterval Super;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(MKMoveBySinCurveAction);

protected:
    mkF32 m_PhaseShift = 0.0f;
    mkF32 m_Amplitude = 1.0f;
    mkF32 m_Frequency = 1.0f;
    cocos2d::Vec2 m_Direction;
    mkF32 m_PreviousDisplacementMagnitude = 0.0f;

public:
    static MKMoveBySinCurveAction* Create(mkF32 _actionDuration = 1.0f, mkF32 _phaseShift = 0.0f, mkF32 _amplitude = 1.0f, mkF32 _frequency = 1.0f, const cocos2d::Vec2& _direction = cocos2d::Vec2(0.0f, 1.0f))
    {
        MKMoveBySinCurveAction* newAction = new (std::nothrow) MKMoveBySinCurveAction();
        if (newAction && newAction->initWithDuration(_actionDuration, _phaseShift, _amplitude, _frequency, _direction))
        {
            newAction->autorelease();
            return newAction;
        }

        CC_SAFE_DELETE(newAction);
        return NULL;
    }

    // SIMI LANJIAO! The original parameter is called time.
    // time your head lah time. Chao Ji Bai. It is the percentage of 
    // the action done lah! Like if the duration is 10 seconds, and 4 seconds has passed,
    // the value is 0.4.
    virtual void update(mkF32 _percentageComplete) override
    {
        Super::update(_percentageComplete);
        if (_target == NULL) { return; }

        mkF32 displacementMagnitude = sin((_percentageComplete * MKMathsHelper::TWO_PI * m_Frequency) + m_PhaseShift) * m_Amplitude;
        cocos2d::Vec2 displacement = m_Direction * (displacementMagnitude - m_PreviousDisplacementMagnitude);
        m_PreviousDisplacementMagnitude = displacementMagnitude;

        _target->setPosition(displacement + _target->getPosition());
    }

    virtual void startWithTarget(cocos2d::Node* _targetNode) override
    {
        Super::startWithTarget(_targetNode);
        m_PreviousDisplacementMagnitude = sin(m_PhaseShift) * m_Amplitude;
    }

CC_CONSTRUCTOR_ACCESS:
    MKMoveBySinCurveAction() {}
    virtual ~MKMoveBySinCurveAction() {}

    mkBool initWithDuration(mkF32 _actionDuration, mkF32 _phaseShift, mkF32 _amplitude, mkF32 _frequency, const cocos2d::Vec2& _direction)
    {
        if (!Super::initWithDuration(_actionDuration)) { return false; }

        CC_ASSERT(_actionDuration > 0.0f);

        m_PhaseShift = _phaseShift;
        m_Amplitude = _amplitude;
        m_Frequency = _frequency;
        m_Direction = _direction.getNormalized();
        m_PreviousDisplacementMagnitude = sin(m_PhaseShift) * m_Amplitude;

        return true;
    }
};

NS_MK_END

#endif // MK_MOVE_BY_SIN_CURVE_ACTION_H