#ifndef MAIN_MENU_SCENE_H
#define MAIN_MENU_SCENE_H

// Include MK
#include "MenuTemplate.h"

USING_NS_MK

class MainMenu : public MenuTemplate
{
    typedef MenuTemplate Super;

protected:
    virtual void InitialiseButtons();

public:
    // Constructor(s) & Destructor
    MainMenu() {}
	virtual ~MainMenu() {}
    CREATE_FUNC(MainMenu);

    virtual bool init();
};

#endif // MAIN_MENU_SCENE_H