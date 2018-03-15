#ifndef MK_INPUT_MACROS_H
#define MK_INPUT_MACROS_H

// Include MK
#include "../Common/MKMacros.h"
#include "MKInputManager.h"

USING_NS_MK

NS_MK_BEGIN

#define MK_DECL_INITIALISE_INPUT(__TYPE__) \
void InitialiseInput() \
{ \
    if (m_ButtonListener == NULL) \
    { \
        m_ButtonListener = MKInputManager::GetInstance()->CreateEventListener<MKInputButton>(CC_CALLBACK_1(__TYPE__::OnButton, this)); \
    } \
\
    if (m_ClickListener == NULL) \
    { \
        m_ClickListener = MKInputManager::GetInstance()->CreateEventListener<MKInputClick>(CC_CALLBACK_1(__TYPE__::OnClick, this)); \
    } \
\
    if (m_AxisListener == NULL) \
    { \
        m_AxisListener = MKInputManager::GetInstance()->CreateEventListener<MKInputAxis>(CC_CALLBACK_1(__TYPE__::OnAxis, this)); \
    } \
}

#define MK_CALL_INITIALISE_INPUT InitialiseInput();

#define MK_DECL_DEINITIALISE_INPUT(__TYPE__) \
void DeinitialiseInput() \
{ \
    if (m_ButtonListener != NULL) \
    { \
        MKInputManager::GetInstance()->RemoveEventListener(m_ButtonListener); \
        m_ButtonListener = NULL; \
    } \
\
    if (m_ClickListener != NULL) \
    { \
        MKInputManager::GetInstance()->RemoveEventListener(m_ClickListener); \
        m_ClickListener = NULL; \
    } \
\
    if (m_AxisListener != NULL) \
    { \
        MKInputManager::GetInstance()->RemoveEventListener(m_AxisListener); \
        m_AxisListener = NULL; \
	} \
}

#define MK_CALL_DEINITIALISE_INPUT DeinitialiseInput();

NS_MK_END

#endif // MK_INPUT_MACROS_H