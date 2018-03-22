#ifndef MK_ASSERTIONS_H
#define MK_ASSERTIONS_H

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "MKMacros.h"

#if MK_DEBUG
#define MK_ASSERT(__CONDITION__) assert(__CONDITION__);
#define MK_ASSERT_WITH_LOG(__CONDITION__, __MESSAGE__) \
    if (__CONDITION__ == false) \
    { \
        CCLOG(#__MESSAGE__); \
    } \
    assert(__CONDITION__); 
#else
	#define MK_ASSERT(__CONDITION__) __CONDITION__
	#define MK_ASSERT_WITH_LOG(__CONDITION__, __MESSAGE__) __CONDITION__ // MK_ASSERT_WITH_LOG is Deprecated
#endif // MK_DEBUG

#endif // MK_ASSERTIONS_H