// Include MK
#include "MK/SceneManagement/MKSceneManager.h"
#include "MK/GameData/MKGameDataLoader.h"
#include "MK/Sprite/MKSprite.h"
#include "MK/UI/MKUIHelper.h"

// Include Header
#include "MainMenu.h"

bool MainMenu::init()
{
	if (!Super::init()) { return false; }

    InitialiseBackground();
    InitialiseButtons();
    StartBGM();

	return true;
}

void MainMenu::InitialiseButtons()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

	// Play
	{
        mkString buttonNormalTexture = "Textures/UI/Buttons/Play/Button_Normal.png";
        mkString buttonSelectedTexture = "Textures/UI/Buttons/Play/Button_Selected.png";
        mkString buttonDisabledTexture = "Textures/UI/Buttons/Play/Button_Disabled.png";

        mkF32 buttonHeight = visibleSize.height * 0.4f;
        mkF32 buttonPositionX = visibleSize.width * 0.5f;
        mkF32 buttonPositionY = visibleSize.height * 0.6f;

		auto button = MKUIHelper::CreateButton(
			"",
			0.0f,
			MKUIHelper::DEFAULT_FONT,
			buttonNormalTexture,
			buttonSelectedTexture,
			buttonDisabledTexture,
			MKUIHelper::DEFAULT_ZOOM_SCALE,
			[=](Ref*) -> void
            {
                StopBGM();
                MKSceneManager::GetInstance()->ReplaceScene("Game");
            }
		);

		button->setPosition(cocos2d::Vec2(buttonPositionX, buttonPositionY));
		button->setScale(buttonHeight / button->getNormalTextureSize().height);

        m_UINode->addChild(button);
	}
    
	// Settings
	{
        mkF32 buttonHeight = visibleSize.height * 0.2f;
        mkF32 buttonPositionX = visibleSize.width * 0.5f - visibleSize.height * 0.3f;
        mkF32 buttonPositionY = visibleSize.height * 0.25f;

        mkString buttonNormalTexture = "Textures/UI/Buttons/Settings/Button_Normal.png";
        mkString buttonSelectedTexture = "Textures/UI/Buttons/Settings/Button_Selected.png";
        mkString buttonDisabledTexture = "Textures/UI/Buttons/Settings/Button_Disabled.png";

		auto button = MKUIHelper::CreateButton(
			"",
			0.0f,
			MKUIHelper::DEFAULT_FONT, 
			buttonNormalTexture,
			buttonSelectedTexture,
			buttonDisabledTexture,
            MKUIHelper::DEFAULT_ZOOM_SCALE,
			[](Ref*) -> void { MKSceneManager::GetInstance()->PushScene("Settings"); }
		);

		button->setPosition(cocos2d::Vec2(buttonPositionX, buttonPositionY));
		button->setScale(buttonHeight / button->getNormalTextureSize().height);

        m_UINode->addChild(button);
	}
    
	// Shop
	{
        mkF32 buttonHeight = visibleSize.height * 0.2f;
        mkF32 buttonPositionX = visibleSize.width * 0.5f;
        mkF32 buttonPositionY = visibleSize.height * 0.25f;

        mkString buttonNormalTexture = "Textures/UI/Buttons/Shop/Button_Normal.png";
        mkString buttonSelectedTexture = "Textures/UI/Buttons/Shop/Button_Selected.png";
        mkString buttonDisabledTexture = "Textures/UI/Buttons/Shop/Button_Disabled.png";

		auto button = MKUIHelper::CreateButton(
			"",
			0.0f,
			MKUIHelper::DEFAULT_FONT,
			buttonNormalTexture,
			buttonSelectedTexture,
			buttonDisabledTexture,
            MKUIHelper::DEFAULT_ZOOM_SCALE,
			[](Ref*) -> void { MKSceneManager::GetInstance()->PushScene("ShopMain"); }
		);

		button->setPosition(cocos2d::Vec2(buttonPositionX, buttonPositionY));
		button->setScale(buttonHeight / button->getNormalTextureSize().height);

        m_UINode->addChild(button);
	}

    // Quit
    {
        mkF32 buttonHeight = visibleSize.height * 0.2f;
        mkF32 buttonPositionX = visibleSize.width * 0.5f + visibleSize.height * 0.3f;
        mkF32 buttonPositionY = visibleSize.height * 0.25f;

        mkString buttonNormalTexture = "Textures/UI/Buttons/Quit/Button_Normal.png";
        mkString buttonSelectedTexture = "Textures/UI/Buttons/Quit/Button_Selected.png";
        mkString buttonDisabledTexture = "Textures/UI/Buttons/Quit/Button_Disabled.png";

		auto button = MKUIHelper::CreateButton(
			"",
			0.0f,
			MKUIHelper::DEFAULT_FONT,
			buttonNormalTexture,
			buttonSelectedTexture,
			buttonDisabledTexture,
            MKUIHelper::DEFAULT_ZOOM_SCALE,
			[=](Ref*) -> void
			{
                StopBGM();
                // End the director.
				Director::getInstance()->end();
				// Destroy the singletons.
				MKInputManager::GetInstance()->Destroy();
				MKSceneManager::GetInstance()->Destroy();
				MKGameDataLoader::GetInstance()->Destroy();
			}
		);

		button->setPosition(cocos2d::Vec2(buttonPositionX, buttonPositionY));
		button->setScale(buttonHeight / button->getNormalTextureSize().height);

        m_UINode->addChild(button);
	}
}

void MainMenu::StartBGM()
{
    if (m_BGMSoundId == MKAudioManager::INVALID_SOUND_ID)
    {
        m_BGMSoundId = MKAudioManager::GetInstance()->PlaySound(m_BGMSoundName, MKSound::SoundType::BGM, 1.0f, true);
    }
}

void MainMenu::StopBGM()
{
    if (m_BGMSoundId != MKAudioManager::INVALID_SOUND_ID)
    {
        MKAudioManager::GetInstance()->StopSound(m_BGMSoundId);
        m_BGMSoundId = MKAudioManager::INVALID_SOUND_ID;
    }
}