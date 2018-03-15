#pragma once

// Include MK
#include "../../MK/Common/MKMacros.h"
#include "../../MK/Common/MKPasskey.h"
#include "../Shop/MKBackgroundShopItem.h"
#include "../../MK/GameData/MKGameData.h"

// Include STL
#include <unordered_map>

NS_MK_BEGIN

class MKBackgroundData : public MKGameData
{
    typedef MKGameData Super;

private:
    // Variable(s)
    std::unordered_map<mkString, MKBackgroundShopItem*> m_Backgrounds;

    // Internal Function(s)
    void ClearBackgrounds();

public:
    // Name of the default background.
    static const mkString DEFAULT_BACKGROUND_NAME;

    // JSON Data Name(s)
    static const mkString BACKGROUND_ARRAY_JSON_DATA_NAME;
    static const mkString BACKGROUND_NAME_JSON_DATA_NAME;
    static const mkString BACKGROUND_PRICE_JSON_DATA_NAME;
    static const mkString TEXTURE_FILE_PATHS_JSON_DATA_NAME;

    // Constructor(s) & Destructor
    MKBackgroundData() {}
    virtual ~MKBackgroundData() {}

    // Virtual Function(s)
    virtual mkBool LoadData(const mkString& _filePath) override;
    virtual mkBool SaveData(const mkString& _filePath) override { return false; }

    virtual mkString GetWritablePath() const override { return ""; }
    virtual mkString GetCachedPath() const override;

    // Interface Function(s)
    const MKBackgroundShopItem* GetBackground(const mkString& _backgroundName) const;
    const std::vector<MKBackgroundShopItem*> GetBackgrounds() const;
    inline mkU32 GetBackgroundCount() const { return m_Backgrounds.size(); }
};

NS_MK_END