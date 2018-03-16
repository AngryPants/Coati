#pragma once

// Include MK
#include "MK/Common/MKMacros.h"

NS_MK_BEGIN

class MKShopItem
{
private:
    // Variable(s)
    mkString m_Name;
    mkU64 m_Price;

public:
    // Constructor(s) & Destructor
    MKShopItem(const mkString& _name, mkU64 _price)
        : m_Name(_name), m_Price(_price)
    {}
    virtual ~MKShopItem() {}

    // Interface Function(s)
    const mkString& GetName() const { return m_Name; }
    mkU64 GetPrice() const { return m_Price; }
};

NS_MK_END