// Include MK
#include "MenuTemplate.h"
#include "MK/Actions/MKFollowNodeAction.h"
#include "Game/GameData/MKPlayerData.h"
#include "Game/GameData/MKBackgroundData.h"
#include "MK/GameData/MKGameDataLoader.h"

// Background
void MenuTemplate::InitialiseBackground()
{
    MKPlayerData* playerData = MKGameDataLoader::GetInstance()->GetGameData<MKPlayerData>();

    MKBackgroundData* backgroundData = MKGameDataLoader::GetInstance()->GetGameData<MKBackgroundData>();
    const MKBackgroundShopItem* equippedBackground = backgroundData->GetBackground(playerData->GetEquippedBackground());
    if (equippedBackground == nullptr)
    {
        equippedBackground = backgroundData->GetBackground(MKBackgroundData::DEFAULT_BACKGROUND_NAME);
        playerData->SetEquippedBackground(MKBackgroundData::DEFAULT_BACKGROUND_NAME);
    }
    MK_ASSERT(equippedBackground != nullptr);
    MK_ASSERT(equippedBackground->GetTextureFilesCount() >= 4);

    m_Background = MKBackground::create();

    m_Background->addLayer(MKBackgroundLayer::create(equippedBackground->GetTextureFile(0), Vec2(0.0f, 0.0f)));
    m_Background->addLayer(MKBackgroundLayer::create(equippedBackground->GetTextureFile(1), Vec2(0.0f, 0.0f)));
    m_Background->addLayer(MKBackgroundLayer::create(equippedBackground->GetTextureFile(2), Vec2(0.0f, 0.0f)));
    m_Background->addLayer(MKBackgroundLayer::create(equippedBackground->GetTextureFile(3), Vec2(0.0f, 0.0f)));

    m_Background->runAction(RepeatForever::create(MKFollowNodeAction::Create(1.0f, getDefaultCamera(), MinamiKotori::MKFollowNodeAction::ALL)));
    addChild(m_Background);
}

void MenuTemplate::InitialiseBanner()
{
    auto visibleSize = _director->getVisibleSize();

    m_Banner = MKSprite::CreateWithFile("Textures/UI/Banner/Banner.png", MKSprite::TextureWrapMethod::REPEAT);
    mkF32 desiredWidth = visibleSize.width;
    mkF32 desiredHeight = visibleSize.height * 0.2f;
    m_Banner->setScale(desiredWidth / m_Banner->getContentSize().width, desiredHeight / m_Banner->getContentSize().height);
    m_Banner->setPosition(visibleSize.width * 0.5f, visibleSize.height - desiredHeight * 0.5f);
    m_Banner->SetTextureScale(desiredWidth / m_Banner->getContentSize().width, 1.0f);
    addChild(m_Banner);
}

void MenuTemplate::InitialiseBackButton()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	mkF32 buttonHeight = visibleSize.height * 0.1f;
	mkF32 buttonWidth = buttonHeight;
	mkF32 buttonPositionX = visibleSize.height * 0.15f;
	mkF32 buttonPositionY = m_Banner->getPositionY();
	mkF32 zoomScale = 1.1f;
	mkString buttonNormalTexture = "Textures/UI/Buttons/Back/Button_Normal.png";
	mkString buttonSelectedTexture = "Textures/UI/Buttons/Back/Button_Selected.png";
	mkString buttonDisabledTexture = "Textures/UI/Buttons/Back/Button_Disabled.png";

    auto button = MKUIHelper::CreateButton(
		"",
		0.0f,
		MKUIHelper::DEFAULT_FONT,
		buttonNormalTexture,
		buttonSelectedTexture,
		buttonDisabledTexture,
		zoomScale,
		[](Ref*) -> void { MKSceneManager::GetInstance()->PopScene(); }
	);

	button->setPosition(cocos2d::Vec2(buttonPositionX, buttonPositionY));
	button->setScaleX(buttonWidth / button->getNormalTextureSize().width);
	button->setScaleY(buttonHeight / button->getNormalTextureSize().height);

	this->addChild(button);
}

void MenuTemplate::InitialiseTitle(const mkString& _titleName)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    mkF32 fontSize = visibleSize.height * 0.1f;
    cocos2d::Color4B fontColor = cocos2d::Color4B::BLACK;

    m_Title = MKUIHelper::CreateLabel(_titleName, fontSize);
    m_Title->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f, m_Banner->getPositionY()));
    m_Title->setTextColor(fontColor);

    this->addChild(m_Title);
}

void MenuTemplate::UpdateTitle(const mkString& _titleName)
{
    m_Title->setString(_titleName);
}