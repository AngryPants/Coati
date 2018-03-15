// Include MK
#include "MKHeroData.h"

NS_MK_BEGIN

// Name of default hero.
const mkString MKHeroData::DEFAULT_HERO_NAME = "Default";

// JSON Data Name(s)
const mkString MKHeroData::HERO_ARRAY_JSON_DATA_NAME = "Heroes";
const mkString MKHeroData::HERO_NAME_JSON_DATA_NAME = "Name";
const mkString MKHeroData::HERO_PRICE_JSON_DATA_NAME = "Price";
const mkString MKHeroData::PLIST_FILE_JSON_DATA_NAME = "PList";
const mkString MKHeroData::JSON_FILE_JSON_DATA_NAME = "JSON";

// Internal Function(s)
void MKHeroData::ClearHeroes()
{
    for (std::unordered_map<mkString, MKHeroShopItem*>::iterator i = m_Heroes.begin(); i != m_Heroes.end(); ++i)
    {
        delete i->second;
    }
    m_Heroes.clear();
}

// Virtual Function(s)
mkBool MKHeroData::LoadData(const mkString& _filePath)
{
    // Load the JSON file.
    RAPIDJSON_NAMESPACE::Document dataDocument;
    if (!MKJSONHelper::LoadFromJSON(dataDocument, _filePath))
    {
        return false;
    }

    // Load Heroes
    auto heroArray = dataDocument.FindMember(HERO_ARRAY_JSON_DATA_NAME.c_str());
    if (!heroArray->value.IsArray())
    {
        return false;
    }
    ClearHeroes();
    for (RAPIDJSON_NAMESPACE::Value::ConstValueIterator i = heroArray->value.Begin(); i != heroArray->value.End(); ++i)
    {
        // Name
        auto name = i->FindMember(HERO_NAME_JSON_DATA_NAME.c_str());
        if (!name->value.IsString())
        {
            return false;
        }

        // Price
        auto price = i->FindMember(HERO_PRICE_JSON_DATA_NAME.c_str());
        if (!price->value.IsUint64())
        {
            return false;
        }

        // PList File
        auto pListFile = i->FindMember(PLIST_FILE_JSON_DATA_NAME.c_str());
        if (!pListFile->value.IsString())
        {
            return false;
        }

        // JSON File
        auto jsonFile = i->FindMember(JSON_FILE_JSON_DATA_NAME.c_str());
        if (!jsonFile->value.IsString())
        {
            return false;
        }

        MKHeroShopItem* hero = new MKHeroShopItem(name->value.GetString(), price->value.GetUint64(), pListFile->value.GetString(), jsonFile->value.GetString());
        m_Heroes.insert(std::pair<mkString, MKHeroShopItem*>(hero->GetName(), hero));
    }

    return true;
}

mkString MKHeroData::GetCachedPath() const
{
    return cocos2d::FileUtils::getInstance()->fullPathForFilename("Shop/Heroes.json");
}

// Interface Function(s)
const MKHeroShopItem* MKHeroData::GetHero(const mkString& _heroName) const
{
    std::unordered_map<mkString, MKHeroShopItem*>::const_iterator iter = m_Heroes.find(_heroName);
    return iter == m_Heroes.end() ? nullptr : iter->second;
}

const std::vector<MKHeroShopItem*> MKHeroData::GetHeroes() const
{
    std::vector<MKHeroShopItem*> results;

    for (std::unordered_map<mkString, MKHeroShopItem*>::const_iterator i = m_Heroes.begin(); i != m_Heroes.end(); ++i)
    {
        results.push_back(i->second);
    }

    return results;
}

NS_MK_END