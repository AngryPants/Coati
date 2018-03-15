// Include Cocos
#include "cocos2d.h"

// Include MK
#include "MKPlayerData.h"
#include "MK/Common/MKJSONHelper.h"

NS_MK_BEGIN

// JSON Data Name(s)
const mkString MKPlayerData::HIGHSCORE_JSON_DATA_NAME = "Highscore";
const mkString MKPlayerData::COINS_JSON_DATA_NAME = "Coins";

const mkString MKPlayerData::EQUIPPED_BACKGROUND_JSON_DATA_NAME = "Equipped_Background";
const mkString MKPlayerData::OWNED_BACKGROUNDS_JSON_DATA_NAME = "Owned_Backgrounds";

const mkString MKPlayerData::EQUIPPED_HERO_JSON_DATA_NAME = "Equipped_Hero";
const mkString MKPlayerData::OWNED_HEROES_JSON_DATA_NAME = "Owned_Heroes";

mkBool MKPlayerData::LoadHighscore(RAPIDJSON_NAMESPACE::Document& _dataDocument)
{
    auto highscore = _dataDocument.FindMember(HIGHSCORE_JSON_DATA_NAME.c_str());
    if (!highscore->value.IsUint64()) { return false; }
    SetHighscore(highscore->value.GetUint64());

    return true;
}

mkBool MKPlayerData::SaveHighscore(RAPIDJSON_NAMESPACE::Document& _dataDocument)
{
    RAPIDJSON_NAMESPACE::Value highscoreName;
    highscoreName.SetString(HIGHSCORE_JSON_DATA_NAME.c_str(), HIGHSCORE_JSON_DATA_NAME.length());
    RAPIDJSON_NAMESPACE::Value highscore;
    highscore.SetUint64(m_Highscore);
    _dataDocument.AddMember(highscoreName, highscore, _dataDocument.GetAllocator());

    return true;
}

mkBool MKPlayerData::LoadCoins(RAPIDJSON_NAMESPACE::Document& _dataDocument)
{
    auto coins = _dataDocument.FindMember(COINS_JSON_DATA_NAME.c_str());
    if (!coins->value.IsUint64()) { return false; }
    SetCoins(coins->value.GetUint64());

    return true;
}

mkBool MKPlayerData::SaveCoins(RAPIDJSON_NAMESPACE::Document& _dataDocument)
{
    RAPIDJSON_NAMESPACE::Value coinsName;
    coinsName.SetString(COINS_JSON_DATA_NAME.c_str(), COINS_JSON_DATA_NAME.length());
    RAPIDJSON_NAMESPACE::Value coins;
    coins.SetUint64(m_Coins);
    _dataDocument.AddMember(coinsName, coins, _dataDocument.GetAllocator());

    return true;
}

// Backgrounds
mkBool MKPlayerData::LoadEquippedBackground(RAPIDJSON_NAMESPACE::Document& _dataDocument)
{
    auto equippedBackground = _dataDocument.FindMember(EQUIPPED_BACKGROUND_JSON_DATA_NAME.c_str());
    if (!equippedBackground->value.IsString()) { return false; }
    SetEquippedBackground(equippedBackground->value.GetString());

    return true;
}

mkBool MKPlayerData::SaveEquippedBackground(RAPIDJSON_NAMESPACE::Document& _dataDocument)
{
    RAPIDJSON_NAMESPACE::Value equippedBackgroundName;
    equippedBackgroundName.SetString(EQUIPPED_BACKGROUND_JSON_DATA_NAME.c_str(), EQUIPPED_BACKGROUND_JSON_DATA_NAME.length());
    RAPIDJSON_NAMESPACE::Value equippedBackground;
    equippedBackground.SetString(m_EquippedBackground.c_str(), m_EquippedBackground.length());
    _dataDocument.AddMember(equippedBackgroundName, equippedBackground, _dataDocument.GetAllocator());

    return true;
}

mkBool MKPlayerData::LoadOwnedBackgrounds(RAPIDJSON_NAMESPACE::Document& _dataDocument)
{
    auto ownedBackgrounds = _dataDocument.FindMember(OWNED_BACKGROUNDS_JSON_DATA_NAME.c_str());
    if (!ownedBackgrounds->value.IsArray()) { return false; }
    m_OwnedBackgrounds.clear();
    for (RAPIDJSON_NAMESPACE::Value::ConstValueIterator i = ownedBackgrounds->value.Begin(); i != ownedBackgrounds->value.End(); ++i)
    {
        if (!i->IsString()) { return false; }
        AddOwnedBackground(i->GetString());
    }

    return true;
}

mkBool MKPlayerData::SaveOwnedBackgrounds(RAPIDJSON_NAMESPACE::Document& _dataDocument)
{
    RAPIDJSON_NAMESPACE::Value ownedBackgroundsName;
    ownedBackgroundsName.SetString(OWNED_BACKGROUNDS_JSON_DATA_NAME.c_str(), OWNED_BACKGROUNDS_JSON_DATA_NAME.length());
    RAPIDJSON_NAMESPACE::Value ownedBackgrounds;
    ownedBackgrounds.SetArray();
    for (std::set<mkString>::iterator i = m_OwnedBackgrounds.begin(); i != m_OwnedBackgrounds.end(); ++i)
    {
        RAPIDJSON_NAMESPACE::Value background;
        background.SetString(i->c_str(), i->length());
        ownedBackgrounds.PushBack(background, _dataDocument.GetAllocator());
    }
    _dataDocument.AddMember(ownedBackgroundsName, ownedBackgrounds, _dataDocument.GetAllocator());

    return true;
}

// Heroes
mkBool MKPlayerData::LoadEquippedHero(RAPIDJSON_NAMESPACE::Document& _dataDocument)
{
    auto equippedHero = _dataDocument.FindMember(EQUIPPED_HERO_JSON_DATA_NAME.c_str());
    if (!equippedHero->value.IsString()) { return false; }
    SetEquippedHero(equippedHero->value.GetString());

    return true;
}

mkBool MKPlayerData::SaveEquippedHero(RAPIDJSON_NAMESPACE::Document& _dataDocument)
{
    RAPIDJSON_NAMESPACE::Value equippedHeroName;
    equippedHeroName.SetString(EQUIPPED_HERO_JSON_DATA_NAME.c_str(), EQUIPPED_HERO_JSON_DATA_NAME.length());
    RAPIDJSON_NAMESPACE::Value equippedHero;
    equippedHero.SetString(m_EquippedHero.c_str(), m_EquippedHero.length());
    _dataDocument.AddMember(equippedHeroName, equippedHero, _dataDocument.GetAllocator());

    return true;
}

mkBool MKPlayerData::LoadOwnedHeroes(RAPIDJSON_NAMESPACE::Document& _dataDocument)
{
    auto ownedHeroes = _dataDocument.FindMember(OWNED_HEROES_JSON_DATA_NAME.c_str());
    if (!ownedHeroes->value.IsArray()) { return false; }
    m_OwnedHeroes.clear();
    for (RAPIDJSON_NAMESPACE::Value::ConstValueIterator i = ownedHeroes->value.Begin(); i != ownedHeroes->value.End(); ++i)
    {
        if (!i->IsString()) { return false; }
        AddOwnedHero(i->GetString());
    }

    return true;
}

mkBool MKPlayerData::SaveOwnedHeroes(RAPIDJSON_NAMESPACE::Document& _dataDocument)
{
    RAPIDJSON_NAMESPACE::Value ownedHeroesName;
    ownedHeroesName.SetString(OWNED_HEROES_JSON_DATA_NAME.c_str(), OWNED_HEROES_JSON_DATA_NAME.length());
    RAPIDJSON_NAMESPACE::Value ownedHeroes;
    ownedHeroes.SetArray();
    for (std::set<mkString>::iterator i = m_OwnedHeroes.begin(); i != m_OwnedHeroes.end(); ++i)
    {
        RAPIDJSON_NAMESPACE::Value hero;
        hero.SetString(i->c_str(), i->length());
        ownedHeroes.PushBack(hero, _dataDocument.GetAllocator());
    }
    _dataDocument.AddMember(ownedHeroesName, ownedHeroes, _dataDocument.GetAllocator());

    return true;
}

mkBool MKPlayerData::LoadData(const mkString& _filePath)
{
    RAPIDJSON_NAMESPACE::Document dataDocument;
    if (!MKJSONHelper::LoadFromJSON(dataDocument, _filePath))
    {
        return false;
    }

    if (!LoadHighscore(dataDocument)) { return false; }
    if (!LoadCoins(dataDocument)) { return false; }
    
    if (!LoadEquippedBackground(dataDocument)) { return false; }
    if (!LoadOwnedBackgrounds(dataDocument)) { return false; }
    
    if (!LoadEquippedHero(dataDocument)) { return false; }
    if (!LoadOwnedHeroes(dataDocument)) { return false; }

    return true;
}

mkBool MKPlayerData::SaveData(const mkString& _filePath)
{
    RAPIDJSON_NAMESPACE::Document dataDocument;
    dataDocument.SetObject();

    if (!SaveHighscore(dataDocument)) { return false; }
    if (!SaveCoins(dataDocument)) { return false; }
    
    if (!SaveEquippedBackground(dataDocument)) { return false; }
    if (!SaveOwnedBackgrounds(dataDocument)) { return false; }

    if (!SaveEquippedHero(dataDocument)) { return false; }
    if (!SaveOwnedHeroes(dataDocument)) { return false; }

    // Open the file and write to it.
    return MKJSONHelper::JsonToFile(dataDocument, _filePath);
}

mkString MKPlayerData::GetWritablePath() const
{
    return cocos2d::FileUtils::getInstance()->getWritablePath() + "PlayerData.json";
}

mkString MKPlayerData::GetCachedPath() const
{
    return cocos2d::FileUtils::getInstance()->fullPathForFilename("SaveData/PlayerData.json");
}

NS_MK_END