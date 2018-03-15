#ifndef MK_REMOVEFROMPARENT_H
#define MK_REMOVEFROMPARENT_H

// Include Cocos
#include "cocos2d.h"

// Include GT
#include "MK/Common/MKMacros.h"

// Using Namespace
USING_NS_CC;

NS_MK_BEGIN

class MKRemoveFromParentAction : public ActionInstant
{
    typedef ActionInstant Super;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(MKRemoveFromParentAction);

public:
    static MKRemoveFromParentAction* Create()
    {
        MKRemoveFromParentAction* newAction = new (std::nothrow) MKRemoveFromParentAction();
        if (newAction)
        {
            newAction->autorelease();
            return newAction;
        }

        CC_SAFE_DELETE(newAction);
        return NULL;
    }

    // SIMI LANJIAO! The original parameter is called _time.
    // _time your head lah _time. Chao Ji Bai. It is the percentage of 
    // the action done lah! Like if the duration is 10 seconds, and 4 seconds has passed,
    // the value is 0.4.
    virtual void update(mkF32 _percentageComplete)
    {
        Super::update(_percentageComplete);
        if (_target) { _target->removeFromParent(); }
    }

CC_CONSTRUCTOR_ACCESS:
    MKRemoveFromParentAction() {}
    virtual ~MKRemoveFromParentAction() {}
};

NS_MK_END

#endif // MK_REMOVEFROMPARENT_H