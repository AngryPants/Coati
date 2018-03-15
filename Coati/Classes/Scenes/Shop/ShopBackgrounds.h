#ifndef SCENE_SHOP_BACKGROUND
#define SCENE_SHOP_BACKGROUND

#include "ShopTemplate.h"
#include "../../Game/GameData/MKPlayerData.h"
#include "../../Game/GameData/MKBackgroundData.h"
#include "../../MK/GameData/MKGameDataLoader.h"

class ShopBackgrounds : public ShopTemplate
{
	typedef ShopTemplate Super;

protected:
    MKPlayerData* m_PlayerData = nullptr;
    MKBackgroundData* m_BackgroundData = nullptr;
    mkU32 m_CurrentBackground = 0;

    // Buy
    void BuyCurrentItem();

    // Equip
    void EquipCurrentItem();

    // Select Next/Previous Item
    void SelectNextItem();
    void SelectPreviousItem();

    void SetBackground(MKBackgroundShopItem* _background);

public:
	// Constructor(s) & Destructor
    ShopBackgrounds();
    virtual ~ShopBackgrounds();
	CREATE_FUNC(ShopBackgrounds);

	virtual mkBool init();
};

#endif // SCENE_SHOP_BACKGROUND