#ifndef MK_TESTSCENE_H
#define MK_TESTSCENE_H

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "MK/Common/MKMacros.h"
#include "MK/Input/MKInputManager.h"
#include "MK/SceneManagement/MKSceneManager.h"
#include "MK/Input/MKInput.h"
#include "MK/Common/MKPasskey.h"
#include "MK/Input/MKInputMacros.h"

USING_NS_CC;

NS_MK_BEGIN

class MKScene : public cocos2d::Scene
{
	typedef cocos2d::Scene Super;

protected:
	// Input Event Listeners
	EventListenerCustom* m_ButtonListener = nullptr;
	EventListenerCustom* m_ClickListener = nullptr;
	EventListenerCustom* m_AxisListener = nullptr;

	// Input Callbacks
	virtual void OnButton(EventCustom * _event) {}
	virtual void OnClick(EventCustom * _event) {}
	virtual void OnAxis(EventCustom * _event) {}

    // UI Node
    cocos2d::Node* m_UINode = nullptr;
    mkU32 m_UINodeLayer = 1000;

    virtual void InitialiseUINode()
    {
        if (m_UINode == nullptr)
        {
            m_UINodeLayer = 1000;
            m_UINode = cocos2d::Node::create();
            addChild(m_UINode, m_UINodeLayer);
        }
    }
    virtual void UpdateUINode()
    {
        if (m_UINode != nullptr)
        {
            auto visibleSize = _director->getVisibleSize();
            m_UINode->setPosition(getDefaultCamera()->getPosition() - Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f));
        }
    }

public:
	MKScene() {}
	virtual ~MKScene() {}

	virtual bool init()
    {
        if (!Super::init()) { return false; }
        InitialiseUINode();
        UpdateUINode();
        return true;
    }
    virtual bool initWithPhysics()
    {
        if (!Super::initWithPhysics()) { return false; }
        InitialiseUINode();
        UpdateUINode();
        return true;
    }

	float GetAspectRatio() const
	{
		return Director::getInstance()->getVisibleSize().width / Director::getInstance()->getVisibleSize().height;
	}
};

NS_MK_END

#endif