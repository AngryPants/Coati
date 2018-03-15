#ifndef MK_GAMEPLAYMACROS_H
#define MK_GAMEPLAYMACROS_H

// Include Cocos
#include "cocos2d.h"

// Include GT
#include "MK/Common/MKMacros.h"

NS_MK_BEGIN

#define MK_DECL_INITIALISE_CONTACT_LISTENER(__TYPE__) \
void InitialiseContactListener() \
{ \
    if (m_ContactListener == NULL) \
    { \
        m_ContactListener = EventListenerPhysicsContact::create(); \
        m_ContactListener->onContactBegin = CC_CALLBACK_1(__TYPE__::OnContactBegin, this); \
        m_ContactListener->retain(); \
        GetScene()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_ContactListener, GetScene()); \
    } \
}

#define MK_CALL_INITIALISE_CONTACT_LISTENER InitialiseContactListener();

#define MK_DECL_DEINITIALISE_CONTACT_LISTENER(__TYPE__) \
void DeinitialiseContactListener() \
{ \
    if (m_ContactListener != NULL) \
    { \
        GetScene()->getEventDispatcher()->removeEventListener(m_ContactListener); \
        m_ContactListener->onContactBegin = NULL; \
        m_ContactListener->release(); \
        m_ContactListener = NULL; \
    } \
}

#define MK_CALL_DEINITIALISE_CONTACT_LISTENER DeinitialiseContactListener();

NS_MK_END

#endif // MK_GAMEPLAYMACROS_H