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
    auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

	mkF32 fontSize = visibleSize.height * 0.03f;
	mkF32 zoomScale = 1.1f;
	mkF32 buttonHeight = visibleSize.height * 0.1f;
	mkF32 buttonWidth = buttonHeight * 3.0f;
	mkF32 buttonPositionX = visibleSize.width * 0.5f + visibleOrigin.x;
	mkF32 buttonPositionY = visibleSize.height * 0.6f + visibleOrigin.y;
	mkString buttonNormalTexture = "Textures/UI/Buttons/Menu/Button_Normal.png";
	mkString buttonSelectedTexture = "Textures/UI/Buttons/Menu/Button_Selected.png";
	mkString buttonDisabledTexture = "Textures/UI/Buttons/Menu/Button_Disabled.png";
	mkU32 buttonCount = 0;

	// Play
	{
		auto button = MKUIHelper::CreateButton(
			"Play",
			fontSize,
			MKUIHelper::DEFAULT_FONT,
			buttonNormalTexture,
			buttonSelectedTexture,
			buttonDisabledTexture,
			zoomScale,
			[=](Ref*) -> void
            {
                StopBGM();
                MKSceneManager::GetInstance()->ReplaceScene("Game");
            }
		);

		button->setPosition(cocos2d::Vec2(buttonPositionX, buttonPositionY - buttonHeight * (mkF32)buttonCount));
		button->setScaleX(buttonWidth / button->getNormalTextureSize().width);
		button->setScaleY(buttonHeight / button->getNormalTextureSize().height);

		this->addChild(button);
		++buttonCount;
	}
    
	// Settings
	{
		auto button = MKUIHelper::CreateButton(
			"Settings",
			fontSize,
			MKUIHelper::DEFAULT_FONT, 
			buttonNormalTexture,
			buttonSelectedTexture,
			buttonDisabledTexture,
			zoomScale,
			[](Ref*) -> void { MKSceneManager::GetInstance()->PushScene("Settings"); }
		);

		button->setPosition(cocos2d::Vec2(buttonPositionX, buttonPositionY - buttonHeight * (mkF32)buttonCount));
		button->setScaleX(buttonWidth / button->getNormalTextureSize().width);
		button->setScaleY(buttonHeight / button->getNormalTextureSize().height);

		this->addChild(button);
		++buttonCount;
	}
    
	// Shop
	{
		auto button = MKUIHelper::CreateButton(
			"Shop",
			fontSize,
			MKUIHelper::DEFAULT_FONT,
			buttonNormalTexture,
			buttonSelectedTexture,
			buttonDisabledTexture,
			zoomScale,
			[](Ref*) -> void { MKSceneManager::GetInstance()->PushScene("ShopMain"); }
		);

		button->setPosition(cocos2d::Vec2(buttonPositionX, buttonPositionY - buttonHeight * (mkF32)buttonCount));
		button->setScaleX(buttonWidth / button->getNormalTextureSize().width);
		button->setScaleY(buttonHeight / button->getNormalTextureSize().height);

		this->addChild(button);
		++buttonCount;
	}

    // Quit
	{
		auto button = MKUIHelper::CreateButton(
			"Quit",
			fontSize,
			MKUIHelper::DEFAULT_FONT,
			buttonNormalTexture,
			buttonSelectedTexture,
			buttonDisabledTexture,
			zoomScale,
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

		button->setPosition(cocos2d::Vec2(buttonPositionX, buttonPositionY - buttonHeight * (mkF32)buttonCount));
		button->setScaleX(buttonWidth / button->getNormalTextureSize().width);
		button->setScaleY(buttonHeight / button->getNormalTextureSize().height);

		this->addChild(button);
		++buttonCount;
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