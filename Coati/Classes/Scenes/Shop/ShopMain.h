#ifndef SCENE_SHOP_H
#define SCENE_SHOP_H

#include "ShopTemplate.h"

class ShopMain : public ShopTemplate
{
	typedef ShopTemplate Super;

protected:
	// UI
	virtual void InitialiseButtons();

    // Virtual Override(s)
    virtual void BuyCurrentItem() {}
    virtual void EquipCurrentItem() {}
    virtual void SelectNextItem() {}
    virtual void SelectPreviousItem() {}

public:
    // Constructor(s) & Destructor
	ShopMain() {}
	virtual ~ShopMain() {}
    CREATE_FUNC(ShopMain);

	virtual mkBool init();
};

#endif // SCENE_SHOP_H