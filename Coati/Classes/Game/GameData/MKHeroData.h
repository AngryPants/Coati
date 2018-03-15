#pragma once

// Include MK
#include "../../MK/Common/MKMacros.h"
#include "../../MK/Common/MKPasskey.h"
#include "../Shop/MKHeroShopItem.h"
#include "../../MK/GameData/MKGameData.h"

// Include STL
#include <unordered_map>

NS_MK_BEGIN

class MKHeroData : public MKGameData
{
    typedef MKGameData Super;

private:
    std::unordered_map<mkString, MKHeroShopItem*> m_Heroes;

    // Internal Function(s)
    void ClearHeroes();

public:
    // Name of default hero.
    static const mkString DEFAULT_HERO_NAME;

    // JSON Data Name(s)
    static const mkString HERO_ARRAY_JSON_DATA_NAME;
    static const mkString HERO_NAME_JSON_DATA_NAME;
    static const mkString HERO_PRICE_JSON_DATA_NAME;
    static const mkString PLIST_FILE_JSON_DATA_NAME;
    static const mkString JSON_FILE_JSON_DATA_NAME;

    // Constructor(s) & Destructor
    MKHeroData() {}
    virtual ~MKHeroData() {}

    // Virtual Function(s)
    virtual mkBool LoadData(const mkString& _filePath) override;
    virtual mkBool SaveData(const mkString& _filePath) override { return false; }

    virtual mkString GetWritablePath() const override { return ""; }
    virtual mkString GetCachedPath() const override;

    // Interface Function(s)
    const MKHeroShopItem* GetHero(const mkString& _heroName) const;
    const std::vector<MKHeroShopItem*> GetHeroes() const;
    inline mkU32 GetHeroCount() const { return m_Heroes.size(); }
};

NS_MK_END