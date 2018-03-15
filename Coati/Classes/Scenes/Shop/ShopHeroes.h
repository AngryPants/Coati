#pragma once

#include "ShopTemplate.h"
#include "../../Game/GameData/MKPlayerData.h"
#include "../../Game/GameData/MKHeroData.h"
#include "../../MK/GameData/MKGameDataLoader.h"
#include "../../MK/Sprite/SpriteAnimation/MKSpriteAnimation.h"

class ShopHeroes : public ShopTemplate
{
    typedef ShopTemplate Super;

protected:
    MKPlayerData * m_PlayerData = nullptr;
    MKHeroData* m_HeroData = nullptr;
    mkU32 m_CurrentHero = 0;
    cocos2d::Node* m_HeroHook = nullptr;
    MKSpriteAnimation* m_Hero = nullptr;

    // Hero Node
    void InitialiseHeroHook();

    // Buy
    void BuyCurrentItem();

    // Equip
    void EquipCurrentItem();

    // Select Next/Previous Item
    void SelectNextItem();
    void SelectPreviousItem();

    void SetHero(MKHeroShopItem* _hero);

public:
    // Constructor(s) & Destructor
    ShopHeroes();
    virtual ~ShopHeroes();
    CREATE_FUNC(ShopHeroes);

    virtual mkBool init();
};