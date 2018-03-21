// Include MK
#include "MK/SceneManagement/MKSceneManager.h"

// Include Header
#include "ShopMain.h"

mkBool ShopMain::init()
{
	if (!Super::init()) { return false; }

	InitialiseBackground();
    InitialiseBanner();
    InitialiseBackButton();
    InitialiseTitle("Shop");
    InitialiseButtons();
    InitialiseWalletUI();

    return true;
}

void ShopMain::InitialiseButtons()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	mkF32 zoomScale = 1.1f;
	mkF32 buttonHeight = visibleSize.height * 0.4f;

    // Buy Heroes
    {
        auto button = MKUIHelper::CreateButton(
            "",
            0.0f,
            MKUIHelper::DEFAULT_FONT,
            "Textures/UI/Buttons/Shop/Heroes/Button_Normal.png",
            "Textures/UI/Buttons/Shop/Heroes/Button_Selected.png",
            "Textures/UI/Buttons/Shop/Heroes/Button_Disabled.png",
            zoomScale,
            [](Ref*) -> void { MKSceneManager::GetInstance()->PushScene("ShopHeroes"); }
        );

        button->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f - visibleSize.height * 0.3f, visibleSize.height * 0.5f));
        button->setScale(buttonHeight / button->getNormalTextureSize().height);

        this->addChild(button);
    }

	// Buy Backgrounds
	{
		auto button = MKUIHelper::CreateButton(
			"",
			0.0f,
			MKUIHelper::DEFAULT_FONT,
			"Textures/UI/Buttons/Shop/Backgrounds/Button_Normal.png",
			"Textures/UI/Buttons/Shop/Backgrounds/Button_Selected.png",
			"Textures/UI/Buttons/Shop/Backgrounds/Button_Disabled.png",
			zoomScale,
			[](Ref*) -> void { MKSceneManager::GetInstance()->PushScene("ShopBackgrounds"); }
		);

        button->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f + visibleSize.height * 0.3f, visibleSize.height * 0.5f));
		button->setScale(buttonHeight / button->getNormalTextureSize().height);

		this->addChild(button);
	}
}