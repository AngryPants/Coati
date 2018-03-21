// Include Game
#include "StartScreen.h"

// Include MK
#include "MK/GameData/MKGameDataLoader.h"
#include "Game/GameData/MKPlayerData.h"
#include "Game/GameData/MKBackgroundData.h"
#include "Game/GameData/MKHeroData.h"
#include "external/json/document.h"

bool StartScreen::init()
{
    if (!Super::init()) { return false; }

    InitialiseData(); // Do this in Start Screen as it only needs to be loaded once.
    InitialiseBackground();
	InitialiseUI();

	return true;
}

void StartScreen::onEnter()
{
    Super::onEnter();
    InitialiseInput();
    MKInputManager::GetInstance()->SetCurrentContext(MK_INPUT_CONTEXT_STARTSCENE);
}

void StartScreen::onExit()
{
    Super::onExit();
    DeinitialiseInput();
    MKInputManager::GetInstance()->SetCurrentContext(MK_INPUT_CONTEXT_DEFAULT);
}

void StartScreen::InitialiseData()
{
    // Load Game Backgrounds
    {
        MKBackgroundData* backgroundData = MKGameDataLoader::GetInstance()->GetGameData<MKBackgroundData>();
        backgroundData->LoadData(backgroundData->GetCachedPath());
    }

    // Load Heroes
    {
        MKHeroData* heroData = MKGameDataLoader::GetInstance()->GetGameData<MKHeroData>();
        heroData->LoadData(heroData->GetCachedPath());
    }

    // Load Player
    {
        // Check if there is already a save file.
        MKPlayerData* playerData = MKGameDataLoader::GetInstance()->GetGameData<MKPlayerData>();
        mkString writablePath = playerData->GetWritablePath();
        if (playerData->LoadData(playerData->GetWritablePath()))
        {
            CCLOG("Existing player save found.");
        }
        else
        {
            CCLOG("No existing player save found or save file corrupted. A new save will be created.");
            playerData->LoadData(playerData->GetCachedPath());
            playerData->SaveData(playerData->GetWritablePath());
        }
    }
}

void StartScreen::InitialiseUI()
{
	InitialiseLogo();
	InitialiseStartLabel();
}

void StartScreen::InitialiseLogo()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    Size desiredSize(visibleSize.height * 0.75f, visibleSize.height * 0.375f);
    MKSprite* logo = MKSprite::CreateWithFile("Textures/UI/Logo.png", MKSprite::REPEAT);
    logo->setScale(desiredSize.height / logo->getContentSize().height);
    logo->setAnchorPoint(Vec2(0.5f, 1.0f));
    logo->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.8f));

    this->addChild(logo);
}

void StartScreen::InitialiseStartLabel()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    float fontSize = visibleSize.height * 0.05f;
	cocos2d::Color4B fontColor = cocos2d::Color4B::BLACK;

	cocos2d::Label* label = MKUIHelper::CreateLabel("TOUCH SCREEN TO START", fontSize);
    label->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.3f));
    label->setTextColor(fontColor);

    this->addChild(label);
}

void StartScreen::OnClick(EventCustom * _event)
{
    MKInputClick* input = static_cast<MKInputClick*>(_event->getUserData());

    if (input->m_InputName == START_GAME)
    {
        if (input->m_ButtonState == MKInputClick::ButtonState::RELEASE)
        {
            DeinitialiseInput();
            MKSceneManager::GetInstance()->ReplaceScene("MainMenu");
        }
    }
}