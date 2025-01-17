#ifndef MK_INPUTMANAGER_H
#define MK_INPUTMANAGER_H

// Include Cocos
#include "cocos2d.h"
#include "base/CCEventCustom.h"
#include "base/CCEventDispatcher.h"

// Include STL
#include <queue>
#include <typeinfo>
#include <type_traits>

// Include MK
#include "MK/Common/MKMacros.h"
#include "MK/Common/MKSingletonTemplate.h"
#include "MK/Common/MKAssertions.h"
#include "MKInputDefinition.h"
#include "MKInputContext.h"
#include "MKInputControllerIndex.h"

#define MK_USE_DIRECTOR_DISPATCHER 0

USING_NS_CC;
using namespace std;

NS_MK_BEGIN

// This is for when compiling for android.
// std::hash does not work on Enums.
#define MK_INPUTNAME mkU32

class MKInputManager : public MKSingletonTemplate<MKInputManager>
{
	friend class MKSingletonTemplate<MKInputManager>;

private:
	MKInputContext m_CurrentContext;
#if MK_USE_DIRECTOR_DISPATCHER
#else
	cocos2d::EventDispatcher* m_EventDispatcher;
#endif
	std::queue<EventCustom*> m_InputEventQueue;
	MKInputDefinition** m_InputDefinitions;

	MKInputManager();
	virtual ~MKInputManager();

	void InitializeDefinitions();
	void SendAllInputEvents();
	void FlushBuffer();

	void Update();

public:
	void SetCurrentContext(MKInputContext _currentContext);
	MKInputContext GetCurrentContext() const;

#if MK_USE_DIRECTOR_DISPATCHER
#else
	inline cocos2d::EventDispatcher* GetEventDispatcher() { return m_EventDispatcher; }
	inline const cocos2d::EventDispatcher* GetEventDispatcher() const { return m_EventDispatcher; }
#endif

	inline void update(mkF32 _deltaTime) { Update(); }

	// I didn't really test to see if changing the key bindings while keys are being held down etc. will cause any problems. Use with
	// caution. Tell me if anything bad happens. Theorectically it SHOULD (not definitely, just should) work.
	MKInputDefinition* GetInputDefinition(MKInputName _inputName) { return m_InputDefinitions[_inputName]; }
	const MKInputDefinition* GetInputDefinition(MKInputName _inputName) const { return m_InputDefinitions[_inputName]; }

	template<typename T>
	void AddInput(T* _input)
	{
#if MK_DEBUG
		mkString baseName = typeid(MKInputBase).name();
		mkString typeName = typeid(T).name();
		mkString assertMessage = "MKInputManager::AddInput - " + typeName + " is not a base class of " + baseName + "!";
		MK_ASSERT_WITH_LOG((std::is_base_of<MKInputBase, T>::value), assertMessage.c_str());
#endif // MK_DEBUG

		EventCustom* inputEvent = new EventCustom(T::GetName());
		inputEvent->setUserData(_input);
		m_InputEventQueue.push(inputEvent);
	}

	template<typename T>
	EventListenerCustom* CreateEventListener(const std::function<void(EventCustom*)>& _callback)
	{
#if MK_DEBUG
		mkString baseName = typeid(MKInputBase).name();
		mkString typeName = typeid(T).name();
		mkString assertMessage = "MKInputManager::CreateEventListener - " + typeName + " is not a base class of " + baseName + "!";
		MK_ASSERT_WITH_LOG((std::is_base_of<MKInputBase, T>::value), assertMessage.c_str());
#endif // MK_DEBUG
		
		EventListenerCustom* listener = EventListenerCustom::create(T::GetName(), _callback);
#if MK_USE_DIRECTOR_DISPATCHER
		Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
#else
        m_EventDispatcher->addEventListenerWithFixedPriority(listener, 1);
#endif

		return listener;
	}

	void RemoveEventListener(EventListenerCustom* _eventListener)
	{
#if MK_USE_DIRECTOR_DISPATCHER
        Director::getInstance()->getEventDispatcher()->removeEventListener(_eventListener);
#else
        m_EventDispatcher->removeEventListener(_eventListener);
#endif
	}

	// Static Functions
	static mkU64 GenerateMask(mkU16 _context, mkU16 _controllerIndex, mkU32 _keyCode)
	{
		mkU64 result = 0;

		// Context
		mkU64 contextMask = (mkU64)_context;
		contextMask <<= 48;
		contextMask &= 0xFFFF000000000000;
		result |= contextMask;

		// Controller Index
		mkU64 controllerIndexMask = (mkU64)_controllerIndex;
		controllerIndexMask <<= 32;
		controllerIndexMask &= 0x0000FFFF00000000;
		result |= controllerIndexMask;

		// KeyCode
		mkU64 keyCodeMask = (mkU64)_keyCode;
		keyCodeMask &= 0x00000000FFFFFFFF;
		result |= keyCodeMask;
		
		return result;
	}

	static mkU16 GetContext(mkU64 _mask)
	{
		_mask >>= 48;
		_mask &= 0x000000000000FFFF;
		mkU16 contextMask = static_cast<mkU16>(_mask);

		return contextMask;
	}

	static mkU16 GetControllerIndex(mkU64 _mask)
	{
		_mask >>= 32;
		_mask &= 0x000000000000FFFF;
		mkU16 controllerIndexMask = static_cast<mkU16>(_mask);

		return controllerIndexMask;
	}

	static mkU32 GetKeyCode(mkU64 _mask)
	{
		_mask &= 0x00000000FFFFFFFF;
		mkU32 keyCodeMask = static_cast<mkU32>(_mask);

		return keyCodeMask;
	}

	static mkBool CompareContext(mkU16 _contextA, mkU16 _contextB)
	{
		return (_contextA & _contextB) != 0;
	}

	static mkBool CompareControllerIndex(mkU16 _controllerIndexA, mkU16 _controllerIndexB)
	{
		return (_controllerIndexA & _controllerIndexB) != 0;
	}

	static mkBool CompareKeyCode(mkU32 _keyCodeA, mkU32 _keyCodeB)
	{
		return (_keyCodeA == _keyCodeB);
	}

	static mkBool CompareMask(mkU64 _maskA, mkU64 _maskB)
	{
		mkU16 contextA = GetContext(_maskA);
		mkU16 contextB = GetContext(_maskB);
		mkU16 controllerIndexA = GetControllerIndex(_maskA);
		mkU16 controllerIndexB = GetControllerIndex(_maskB);
		mkU32 keyCodeA = GetKeyCode(_maskA);
		mkU32 keyCodeB = GetKeyCode(_maskB);

		return (CompareContext(contextA, contextB) && CompareControllerIndex(controllerIndexA, controllerIndexB) && CompareKeyCode(keyCodeA, keyCodeB));
	}

};

NS_MK_END

#endif // MK_INPUTMANAGER_H