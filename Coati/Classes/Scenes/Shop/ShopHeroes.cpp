#include "ShopHeroes.h"
#include "Game/Player/MKPlayer.h"

// Constructor(s) & Destructor
ShopHeroes::ShopHeroes()
{
}

ShopHeroes::~ShopHeroes()
{
}

mkBool ShopHeroes::init()
{
    if (!Super::init()) { return false; }

    m_PlayerData = MKGameDataLoader::GetInstance()->GetGameData<MKPlayerData>();
    m_HeroData = MKGameDataLoader::GetInstance()->GetGameData<MKHeroData>();

    InitialiseBackground();
    InitialiseHeroHook();
    InitialiseBanner();
    InitialiseBackButton();
    InitialiseTitle("");
    InitialiseWalletUI();
    InitialiseBuyButton();
    InitialiseEquipButton();
    InitialiseItemPrice();
    InitialisePreviousItemButton();
    InitialiseNextItemButton();

    // Set first hero.
    if (m_HeroData->GetHeroCount() > 0)
    {
        SetHero(m_HeroData->GetHeroes()[0]);
    }

    return true;
}

// Hero Node
void ShopHeroes::InitialiseHeroHook()
{
    m_HeroHook = cocos2d::Node::create();
    m_UINode->addChild(m_HeroHook);
}

// Buy
void ShopHeroes::BuyCurrentItem()
{
    MKHeroShopItem* hero = m_HeroData->GetHeroes()[m_CurrentHero];

    if (m_PlayerData->GetCoins() < hero->GetPrice())
    {
        return;
    }

    if (m_PlayerData->OwnsHero(hero->GetName()))
    {
        return;
    }

    m_PlayerData->RemoveCoins(hero->GetPrice());
    m_PlayerData->AddOwnedHero(hero->GetName());
    m_PlayerData->SaveData(m_PlayerData->GetWritablePath());
    SetHero(hero);
}

// Equip
void ShopHeroes::EquipCurrentItem()
{
    MKHeroShopItem* hero = m_HeroData->GetHeroes()[m_CurrentHero];

    if (!m_PlayerData->OwnsHero(hero->GetName()))
    {
        return;
    }

    m_PlayerData->SetEquippedHero(hero->GetName());
    m_PlayerData->SaveData(m_PlayerData->GetWritablePath());
    SetHero(hero);
}

// Select Next/Previous Item
void ShopHeroes::SelectNextItem()
{
    if (m_HeroData->GetHeroCount() == 0)
    {
        m_CurrentHero = 0;
        return;
    }

    m_CurrentHero = (m_CurrentHero + 1) % m_HeroData->GetHeroCount();
    SetHero(m_HeroData->GetHeroes()[m_CurrentHero]);
}

void ShopHeroes::SelectPreviousItem()
{
    if (m_HeroData->GetHeroCount() == 0)
    {
        m_CurrentHero = 0;
        return;
    }

    if (m_CurrentHero == 0)
    {
        m_CurrentHero = m_HeroData->GetHeroCount() - 1;
    }
    else
    {
        --m_CurrentHero;
    }
    SetHero(m_HeroData->GetHeroes()[m_CurrentHero]);
}

void ShopHeroes::SetHero(MKHeroShopItem* _hero)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    mkBool ownsItem = m_PlayerData->OwnsHero(_hero->GetName());
    mkU64 itemPrice = _hero->GetPrice();
    mkBool sufficientCoins = m_PlayerData->GetCoins() >= _hero->GetPrice();
    mkBool equippedItem = (m_PlayerData->GetEquippedHero() == _hero->GetName());

    // Update Title String
    UpdateTitle(_hero->GetName());

    // Update Item Price
    UpdateItemPrice(ownsItem, itemPrice, sufficientCoins);

    // Update Buy
    UpdateBuyButton(ownsItem, sufficientCoins);

    // Update Equip Button
    UpdateEquipButton(ownsItem, equippedItem);

    // Update Hero
    if (m_Hero != nullptr)
    {
        m_HeroHook->removeChild(m_Hero);
        m_Hero = nullptr;
    }

    mkF32 desiredHeight = visibleSize.height * 0.5f;
    m_Hero = MKSpriteAnimation::Create(_hero->GetPListFile(), _hero->GetJSONFile());
    m_Hero->setScale(desiredHeight / m_Hero->getContentSize().height);
    m_Hero->setPositionX(visibleSize.width * 0.5f);
    m_Hero->setPositionY(visibleSize.height * 0.15f + m_Hero->getScaledContentSize().height * 0.5f);
    m_Hero->AddTicketToQueue(MKSpriteAnimationTicket::Create(MKPlayer::RUN_ANIMATION_NAME, 0.5f, MKSpriteAnimationTicket::INFINITE_LOOPS, false));
    m_HeroHook->addChild(m_Hero);
}