// Include MK
#include "ShopTemplate.h"
#include "MK/GameData/MKGameDataLoader.h"
#include "Game/GameData/MKPlayerData.h"

// Constructor(s) & Destructor
ShopTemplate::ShopTemplate()
{
}

ShopTemplate::~ShopTemplate()
{
    // Buy Button
    if (m_BuyButton)
    {
        m_BuyButton->release();
    }

    // Equip Button
    if (m_EquipButton)
    {
        m_EquipButton->release();
    }

    // Item Price
    if (m_ItemPriceCoinIcon)
    {
        m_ItemPriceCoinIcon->release();
    }

    if (m_ItemPriceValue)
    {
        m_ItemPriceValue->release();
    }

    if (m_InsufficientCoins)
    {
        m_InsufficientCoins->release();
    }
}

// Wallet
void ShopTemplate::InitialiseWalletUI()
{
    auto visibleSize = _director->getVisibleSize();

    MKPlayerData* playerData = MKGameDataLoader::GetInstance()->GetGameData<MKPlayerData>();

    m_WalletCoinIcon = MKSprite::CreateWithFile("Textures/UI/Coin.png");
    mkF32 desiredHeight = visibleSize.height * 0.1f;
    mkF32 desiredWidth = desiredHeight;
    mkF32 iconPositionX = visibleSize.width - visibleSize.height * 0.15f;
    mkF32 iconPositionY = m_Banner->getPositionY();
    m_WalletCoinIcon->setPosition(iconPositionX, iconPositionY);
    m_WalletCoinIcon->setScale(desiredWidth / m_WalletCoinIcon->getContentSize().width, desiredHeight / m_WalletCoinIcon->getContentSize().height);
    m_UINode->addChild(m_WalletCoinIcon);

    mkF32 iconValuePadding = m_WalletCoinIcon->getScaledContentSize().width * 0.5f; // The space between the number and the icon.

    m_WalletValue = MKUIHelper::CreateLabel(StringUtils::toString(playerData->GetCoins()), m_WalletCoinIcon->getScaledContentSize().height);
    mkF32 fontSize = m_WalletCoinIcon->getScaledContentSize().height;
    mkF32 valuePositionX = m_WalletCoinIcon->getPositionX() - (m_WalletCoinIcon->getScaledContentSize().width + m_WalletValue->getScaledContentSize().width) * 0.5f - iconValuePadding;
    mkF32 valuePositionY = m_Banner->getPositionY();
    m_WalletValue->setPositionX(valuePositionX);
    m_WalletValue->setPositionY(valuePositionY);
    m_UINode->addChild(m_WalletValue);
}

void ShopTemplate::UpdateWalletUI()
{
    MKPlayerData* playerData = MKGameDataLoader::GetInstance()->GetGameData<MKPlayerData>();

    m_WalletValue->setString(StringUtils::toString(playerData->GetCoins()));
    mkF32 valuePositionX = m_WalletCoinIcon->getPositionX() - m_WalletCoinIcon->getScaledContentSize().width - (m_WalletValue->getScaledContentSize().width * 0.5f);
    mkF32 valuePositionY = m_Banner->getPositionY();
    m_WalletValue->setPositionX(valuePositionX);
    m_WalletValue->setPositionY(valuePositionY);
}

// Buy Button
void ShopTemplate::InitialiseBuyButton()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    mkF32 buttonHeight = visibleSize.height * 0.1f;
    mkF32 buttonPositionX = visibleSize.width * 0.5f;
    mkF32 buttonPositionY = visibleSize.height * 0.2f;
    mkF32 fontSize = buttonHeight;
    mkString buttonNormalTexture = "Textures/UI/Buttons/Menu/Button_Normal.png";
    mkString buttonSelectedTexture = "Textures/UI/Buttons/Menu/Button_Selected.png";
    mkString buttonDisabledTexture = "Textures/UI/Buttons/Menu/Button_Disabled.png";
    
    m_BuyButton = MKUIHelper::CreateButton(
        "BUY",
        fontSize,
        MKUIHelper::DEFAULT_FONT,
        buttonNormalTexture,
        buttonSelectedTexture,
        buttonDisabledTexture,
        MKUIHelper::DEFAULT_ZOOM_SCALE,
        [=](Ref*) -> void { BuyCurrentItem(); }
    );
    
    m_BuyButton->setPosition(Vec2(buttonPositionX, buttonPositionY));
    m_BuyButton->setScale(buttonHeight / m_BuyButton->getNormalTextureSize().height);
    
    m_BuyButton->retain();
}

void ShopTemplate::UpdateBuyButton(mkBool _ownsItem, mkBool _sufficientCoins)
{
    m_UINode->removeChild(m_BuyButton, false);
    if (_ownsItem)
    {
        return;
    }

    m_UINode->addChild(m_BuyButton);
    m_BuyButton->setEnabled(_sufficientCoins);
    m_BuyButton->setBright(_sufficientCoins);
    m_BuyButton->getTitleLabel()->setString("BUY");
}

// Equip Button
void ShopTemplate::InitialiseEquipButton()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    mkF32 buttonHeight = visibleSize.height * 0.1f;
    mkF32 buttonPositionX = visibleSize.width * 0.5f;
    mkF32 buttonPositionY = visibleSize.height * 0.2f;
    mkF32 fontSize = buttonHeight;
    mkString buttonNormalTexture = "Textures/UI/Buttons/Menu/Button_Normal.png";
    mkString buttonSelectedTexture = "Textures/UI/Buttons/Menu/Button_Selected.png";
    mkString buttonDisabledTexture = "Textures/UI/Buttons/Menu/Button_Disabled.png";

    m_EquipButton = MKUIHelper::CreateButton(
        "EQUIP",
        fontSize,
        MKUIHelper::DEFAULT_FONT,
        buttonNormalTexture,
        buttonSelectedTexture,
        buttonDisabledTexture,
        MKUIHelper::DEFAULT_ZOOM_SCALE,
        [=](Ref*) -> void { EquipCurrentItem(); }
    );

    m_EquipButton->setPosition(Vec2(buttonPositionX, buttonPositionY));
    m_EquipButton->setScale(buttonHeight / m_EquipButton->getNormalTextureSize().height);

    m_EquipButton->retain();
}

void ShopTemplate::UpdateEquipButton(mkBool _ownsItem, mkBool _equippedItem)
{
    m_UINode->removeChild(m_EquipButton, false);
    if (!_ownsItem)
    {
        return;
    }

    m_UINode->addChild(m_EquipButton);
    if (_equippedItem)
    {
        m_EquipButton->setEnabled(false);
        m_EquipButton->setBright(false);
        m_EquipButton->getTitleLabel()->setString("EQUIPPED");
    }
    else
    {
        m_EquipButton->setEnabled(true);
        m_EquipButton->setBright(true);
        m_EquipButton->getTitleLabel()->setString("EQUIP");
    }
}

// Item Price
void ShopTemplate::InitialiseItemPrice()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    m_ItemPriceCoinIcon = MKSprite::CreateWithFile("Textures/UI/Coin.png");
    mkF32 desiredHeight = visibleSize.height * 0.075f;
    mkF32 desiredWidth = desiredHeight;
    m_ItemPriceCoinIcon->setScale(desiredWidth / m_ItemPriceCoinIcon->getContentSize().width, desiredHeight / m_ItemPriceCoinIcon->getContentSize().height);
    m_ItemPriceCoinIcon->setPositionY(visibleSize.height * 0.7f);
    m_ItemPriceCoinIcon->retain();
    m_UINode->addChild(m_ItemPriceCoinIcon);

    mkF32 fontSize = m_ItemPriceCoinIcon->getScaledContentSize().height;
    m_ItemPriceValue = MKUIHelper::CreateLabel("0", fontSize);
    m_ItemPriceValue->setPositionY(m_ItemPriceCoinIcon->getPositionY());
    m_ItemPriceValue->retain();
    m_UINode->addChild(m_ItemPriceValue);

    m_InsufficientCoins = MKUIHelper::CreateLabel("[NOT ENOUGH COINS]", fontSize);
    m_InsufficientCoins->setPositionX(visibleSize.width * 0.5f);
    m_InsufficientCoins->setPositionY(m_ItemPriceValue->getPositionY() - m_ItemPriceValue->getScaledContentSize().height);
    m_InsufficientCoins->setTextColor(m_InsufficientCoinsColor);
    m_InsufficientCoins->retain();
}

void ShopTemplate::UpdateItemPrice(mkBool _ownsItem, mkU64 _price, mkBool _sufficientCoins)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    m_UINode->removeChild(m_ItemPriceCoinIcon, false);
    m_UINode->removeChild(m_ItemPriceValue, false);
    m_UINode->removeChild(m_InsufficientCoins, false);
    if (_ownsItem)
    {
        return;
    }

    if (_price == 0)
    {
        m_ItemPriceValue->setString("FREE");
    }
    else
    {
        m_ItemPriceValue->setString(StringUtils::toString(_price));
    }
    if (_sufficientCoins)
    {
        m_ItemPriceValue->setTextColor(m_SufficientCoinsColor);
    }
    else
    {
        m_ItemPriceValue->setTextColor(m_InsufficientCoinsColor);
        m_UINode->addChild(m_InsufficientCoins);
    }
    
    mkF32 iconValuePadding = m_ItemPriceCoinIcon->getScaledContentSize().width * 0.5f; // The space between the number and the icon.
    mkF32 totalWidth = m_ItemPriceCoinIcon->getScaledContentSize().width + m_ItemPriceValue->getScaledContentSize().width + iconValuePadding;
    
    m_ItemPriceCoinIcon->setPositionX((visibleSize.width * 0.5f) + (-totalWidth * 0.5f) + (m_ItemPriceCoinIcon->getScaledContentSize().width * 0.5f));
    m_ItemPriceValue->setPositionX((visibleSize.width * 0.5f) + (totalWidth * 0.5f) + (-m_ItemPriceValue->getScaledContentSize().width * 0.5f));
    
    m_UINode->addChild(m_ItemPriceCoinIcon);
    m_UINode->addChild(m_ItemPriceValue);
}

void ShopTemplate::InitialisePreviousItemButton()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    mkF32 buttonHeight = visibleSize.height * 0.2f;
    mkF32 buttonPositionX = visibleSize.height * 0.2f;
    mkF32 buttonPositionY = visibleSize.height * 0.5f;
    mkString buttonNormalTexture = "Textures/UI/Buttons/Arrow/Left/Button_Normal.png";
    mkString buttonSelectedTexture = "Textures/UI/Buttons/Arrow/Left/Button_Selected.png";
    mkString buttonDisabledTexture = "Textures/UI/Buttons/Arrow/Left/Button_Disabled.png";

    auto button = MKUIHelper::CreateButton(
        "",
        0.0f,
        MKUIHelper::DEFAULT_FONT,
        buttonNormalTexture,
        buttonSelectedTexture,
        buttonDisabledTexture,
        MKUIHelper::DEFAULT_ZOOM_SCALE,
        [=](Ref*) -> void { SelectPreviousItem(); }
    );

    button->setPosition(Vec2(buttonPositionX, buttonPositionY));
    button->setScale(buttonHeight / button->getNormalTextureSize().height);

    m_UINode->addChild(button);
}

void ShopTemplate::InitialiseNextItemButton()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    mkF32 buttonHeight = visibleSize.height * 0.2f;
    mkF32 buttonPositionX = visibleSize.width - visibleSize.height * 0.2f;
    mkF32 buttonPositionY = visibleSize.height * 0.5f;
    mkString buttonNormalTexture = "Textures/UI/Buttons/Arrow/Right/Button_Normal.png";
    mkString buttonSelectedTexture = "Textures/UI/Buttons/Arrow/Right/Button_Selected.png";
    mkString buttonDisabledTexture = "Textures/UI/Buttons/Arrow/Right/Button_Disabled.png";

    auto button = MKUIHelper::CreateButton(
        "",
        0.0f,
        MKUIHelper::DEFAULT_FONT,
        buttonNormalTexture,
        buttonSelectedTexture,
        buttonDisabledTexture,
        MKUIHelper::DEFAULT_ZOOM_SCALE,
        [=](Ref*) -> void { SelectNextItem(); }
    );

    button->setPosition(Vec2(buttonPositionX, buttonPositionY));
    button->setScale(buttonHeight / button->getNormalTextureSize().height);

    m_UINode->addChild(button);
}