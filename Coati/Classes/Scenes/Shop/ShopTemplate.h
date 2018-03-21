#pragma once

#include "Scenes/MenuTemplate.h"

class ShopTemplate : public MenuTemplate
{
    typedef MenuTemplate Super;

protected:
    Color4B m_InsufficientCoinsColor = Color4B::RED;
    Color4B m_SufficientCoinsColor = Color4B(0, 200, 0, 255);

    // Wallet
    MKSprite* m_WalletCoinIcon = nullptr;
    Label* m_WalletValue = nullptr;

    void InitialiseWalletUI();
    void UpdateWalletUI();

    // Buy Button
    ui::Button* m_BuyButton = nullptr;
    
    virtual void InitialiseBuyButton();
    virtual void UpdateBuyButton(mkBool _ownsItem, mkBool _sufficientCoins);
    virtual void BuyCurrentItem() = 0;

    // Equip Button
    ui::Button* m_EquipButton = nullptr;

    virtual void InitialiseEquipButton();
    virtual void UpdateEquipButton(mkBool _ownsItem, mkBool _equippedItem);
    virtual void EquipCurrentItem() = 0;

    // Item Price
    MKSprite* m_ItemPriceCoinIcon = nullptr;
    Label* m_ItemPriceValue = nullptr;
    Label* m_InsufficientCoins = nullptr;

    virtual void InitialiseItemPrice();
    virtual void UpdateItemPrice(mkBool _ownsItem, mkU64 _price, mkBool _sufficientCoins);

    // Select Next/Previous Item
    virtual void InitialisePreviousItemButton();
    virtual void InitialiseNextItemButton();
    virtual void SelectNextItem() = 0;
    virtual void SelectPreviousItem() = 0;

protected:
    // Constructor(s) & Destructor
    ShopTemplate();
    virtual ~ShopTemplate();
};