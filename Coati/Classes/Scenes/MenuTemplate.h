#ifndef TEMPLATE_MENU_SCENE_H
#define TEMPLATE_MENU_SCENE_H

// Include MK
#include "MK/SceneManagement/MKScene.h"
#include "MK/UI/MKUIHelper.h"
#include "Game/Background/MKBackground.h"
#include "MK/Sprite/MKSprite.h"
#include "MK/Audio/MKAudioManager.h"

USING_NS_MK

class MenuTemplate : public MKScene
{
	typedef MKScene Super;

protected:
    // Variable(s)
    MKBackground* m_Background = nullptr;
    MKSprite* m_Banner = nullptr;
    Label* m_Title = nullptr;

    // UI
	virtual void InitialiseBackground();
    virtual void InitialiseBanner();
	virtual void InitialiseBackButton();
    virtual void InitialiseTitle(const mkString& _titleName);
    virtual void UpdateTitle(const mkString& _titleName);

public:
	// Constructor(s) & Destructor
	MenuTemplate() {}
	virtual ~MenuTemplate() {}

    virtual void onEnter() override;
};

#endif // TEMPLATE_MENU_SCENE_H