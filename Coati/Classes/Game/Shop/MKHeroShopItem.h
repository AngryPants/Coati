#pragma once

// Include MK
#include "MK/Shop/MKShopItem.h"

NS_MK_BEGIN

class MKHeroShopItem : public MKShopItem
{
    typedef MKShopItem Super;

private:
    // Variable(s)
    mkString m_PListFile;
    mkString m_JSONFile;

public:
    // Constructor(s) & Destructor
    MKHeroShopItem(const mkString& _name, mkU64 _price, const mkString& _PListFile, const mkString& _jsonFile)
        : MKShopItem(_name, _price), m_PListFile(_PListFile), m_JSONFile(_jsonFile)
    {}
    virtual ~MKHeroShopItem() {}

    // Interface Function(s)
    inline const mkString& GetPListFile() const { return m_PListFile; }
    inline const mkString& GetJSONFile() const { return m_JSONFile; }
};

NS_MK_END