#ifndef START_SCREEN_SCENE_H
#define START_SCREEN_SCENE_H

// Include MK
#include "MenuTemplate.h"
#include "MK/Sprite/MKSprite.h"

// Include STL
#include <cmath>

USING_NS_MK
USING_NS_CC;
using namespace std;

class StartScreen : public MenuTemplate
{
    typedef MenuTemplate Super;

protected:
    // Data
	void InitialiseUI();
    void InitialiseData();
    void InitialiseLogo();
    void InitialiseStartLabel();

    // Input Callbacks
    MK_DECL_INITIALISE_INPUT(StartScreen);
    MK_DECL_DEINITIALISE_INPUT(StartScreen);
    virtual void OnButton(EventCustom * _event) {}
    virtual void OnClick(EventCustom * _event);
    virtual void OnAxis(EventCustom * _event) {}

public:
    // Constructor(s) & Destructor
    StartScreen() {}
    virtual ~StartScreen() {}
    CREATE_FUNC(StartScreen);

    virtual bool init();
    virtual void update(float _deltaTime) override {}
    virtual void onEnter() override;
    virtual void onExit() override;
};

#endif // START_SCREEN_SCENE_H