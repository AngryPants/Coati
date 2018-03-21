#ifndef MAIN_MENU_SCENE_H
#define MAIN_MENU_SCENE_H

// Include MK
#include "MenuTemplate.h"

USING_NS_MK

class MainMenu : public MenuTemplate
{
    typedef MenuTemplate Super;

protected:
    // Sound Names(s)
    const mkString m_BGMSoundName = "Menu_BGM";

    // Sound IDs
    mkS32 m_BGMSoundId = MKAudioManager::INVALID_SOUND_ID;

    void StartBGM();
    void StopBGM();

    virtual void InitialiseButtons();

public:
    // Constructor(s) & Destructor
    MainMenu() {}
	virtual ~MainMenu() {}
    CREATE_FUNC(MainMenu);

    virtual bool init();
};

#endif // MAIN_MENU_SCENE_H