// Include MK
#include "ShopBackgrounds.h"

// Constructor(s) & Destructor
ShopBackgrounds::ShopBackgrounds()
{
}

ShopBackgrounds::~ShopBackgrounds()
{
}

mkBool ShopBackgrounds::init()
{
	if (!Super::init()) { return false; }

    m_PlayerData = MKGameDataLoader::GetInstance()->GetGameData<MKPlayerData>();
    m_BackgroundData = MKGameDataLoader::GetInstance()->GetGameData<MKBackgroundData>();

	InitialiseBackground();
    InitialiseBanner();
    InitialiseBackButton();
    InitialiseTitle("");
    InitialiseWalletUI();
    InitialiseBuyButton();
    InitialiseEquipButton();
    InitialiseItemPrice();
    InitialisePreviousItemButton();
    InitialiseNextItemButton();

    // Set first background.
    if (m_BackgroundData->GetBackgroundCount() > 0)
    {
        SetBackground(m_BackgroundData->GetBackgrounds()[0]);
    }

	return true;
}

// Buy
void ShopBackgrounds::BuyCurrentItem()
{
    MKBackgroundShopItem* background = m_BackgroundData->GetBackgrounds()[m_CurrentBackground];

    if (m_PlayerData->GetCoins() < background->GetPrice())
    {
        return;
    }

    if (m_PlayerData->OwnsBackground(background->GetName()))
    {
        return;
    }

    m_PlayerData->RemoveCoins(background->GetPrice());
    m_PlayerData->AddOwnedBackground(background->GetName());
    m_PlayerData->SaveData(m_PlayerData->GetWritablePath());
    SetBackground(background);
    UpdateWalletUI();
}

// Equip
void ShopBackgrounds::EquipCurrentItem()
{
    MKBackgroundShopItem* background = m_BackgroundData->GetBackgrounds()[m_CurrentBackground];

    if (!m_PlayerData->OwnsBackground(background->GetName()))
    {
        return;
    }

    m_PlayerData->SetEquippedBackground(background->GetName());
    m_PlayerData->SaveData(m_PlayerData->GetWritablePath());
    SetBackground(background);
}

// Select Next/Previous Item
void ShopBackgrounds::SelectNextItem()
{
    if (m_BackgroundData->GetBackgroundCount() == 0)
    {
        m_CurrentBackground = 0;
        return;
    }

    m_CurrentBackground = (m_CurrentBackground + 1) % m_BackgroundData->GetBackgroundCount();
    SetBackground(m_BackgroundData->GetBackgrounds()[m_CurrentBackground]);
}

void ShopBackgrounds::SelectPreviousItem()
{
    if (m_BackgroundData->GetBackgroundCount() == 0)
    {
        m_CurrentBackground = 0;
        return;
    }

    if (m_CurrentBackground == 0)
    {
        m_CurrentBackground = m_BackgroundData->GetBackgroundCount() - 1;
    }
    else
    {
        --m_CurrentBackground;
    }
    SetBackground(m_BackgroundData->GetBackgrounds()[m_CurrentBackground]);
}

void ShopBackgrounds::SetBackground(MKBackgroundShopItem* _background)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    mkBool ownsItem = m_PlayerData->OwnsBackground(_background->GetName());
    mkU64 itemPrice = _background->GetPrice();
    mkBool sufficientCoins = m_PlayerData->GetCoins() >= _background->GetPrice();
    mkBool equippedItem = (m_PlayerData->GetEquippedBackground() == _background->GetName());

    // Update Title String
    UpdateTitle(_background->GetName());

    // Update Item Price
    UpdateItemPrice(ownsItem, itemPrice, sufficientCoins);

    // Update Buy
    UpdateBuyButton(ownsItem, sufficientCoins);

    // Update Equip Button
    UpdateEquipButton(ownsItem, equippedItem);

    // Update Background
    m_Background->removeAllLayers();
    MK_ASSERT(_background->GetTextureFilesCount() >= 4);

    m_Background->addLayer(MKBackgroundLayer::create(_background->GetTextureFile(0), Vec2(0.0f * visibleSize.width, 0.0f)));
    m_Background->addLayer(MKBackgroundLayer::create(_background->GetTextureFile(1), Vec2(0.1f * visibleSize.width, 0.0f)));
    m_Background->addLayer(MKBackgroundLayer::create(_background->GetTextureFile(2), Vec2(0.2f * visibleSize.width, 0.0f)));
    m_Background->addLayer(MKBackgroundLayer::create(_background->GetTextureFile(3), Vec2(0.3f * visibleSize.width, 0.0f)));
}

void ShopBackgrounds::onEnter()
{
    MKScene::onEnter();
}