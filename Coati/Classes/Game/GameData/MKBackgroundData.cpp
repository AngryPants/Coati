// Include MK
#include "MKBackgroundData.h"
#include "MK/Common/MKJSONHelper.h"

NS_MK_BEGIN

// Name of the default background.
const mkString MKBackgroundData::DEFAULT_BACKGROUND_NAME = "Default";

// JSON Data Name(s)
const mkString MKBackgroundData::BACKGROUND_ARRAY_JSON_DATA_NAME = "Backgrounds";
const mkString MKBackgroundData::BACKGROUND_NAME_JSON_DATA_NAME = "Name";
const mkString MKBackgroundData::BACKGROUND_PRICE_JSON_DATA_NAME = "Price";
const mkString MKBackgroundData::TEXTURE_FILE_PATHS_JSON_DATA_NAME = "Textures";

// Internal Function(s)
void MKBackgroundData::ClearBackgrounds()
{
    for (std::unordered_map<mkString, MKBackgroundShopItem*>::iterator i = m_Backgrounds.begin(); i != m_Backgrounds.end(); ++i)
    {
        delete i->second;
    }
    m_Backgrounds.clear();
}

// Virtual Function(s)
mkBool MKBackgroundData::LoadData(const mkString& _filePath)
{
    // Load the JSON file.
    RAPIDJSON_NAMESPACE::Document dataDocument;
    if (!MKJSONHelper::LoadFromJSON(dataDocument, _filePath))
    {
        return false;
    }

    // Load Backgrounds
    auto backgroundArray = dataDocument.FindMember(BACKGROUND_ARRAY_JSON_DATA_NAME.c_str());
    if (!backgroundArray->value.IsArray())
    {
        return false;
    }
    ClearBackgrounds();
    for (RAPIDJSON_NAMESPACE::Value::ConstValueIterator i = backgroundArray->value.Begin(); i != backgroundArray->value.End(); ++i)
    {
        // Name
        auto name = i->FindMember(BACKGROUND_NAME_JSON_DATA_NAME.c_str());
        if (!name->value.IsString())
        {
            return false;
        }

        // Price
        auto price = i->FindMember(BACKGROUND_PRICE_JSON_DATA_NAME.c_str());
        if (!price->value.IsUint64())
        {
            return false;
        }

        // Texture File Paths
        std::vector<mkString> textureFilePathsVector;
        auto textureFilePaths = i->FindMember(TEXTURE_FILE_PATHS_JSON_DATA_NAME.c_str());
        if (!textureFilePaths->value.IsArray())
        {
            return false;
        }
        for (RAPIDJSON_NAMESPACE::Value::ConstValueIterator j = textureFilePaths->value.Begin(); j != textureFilePaths->value.End(); ++j)
        {
            if (!j->IsString())
            {
                return false;
            }
            textureFilePathsVector.push_back(j->GetString());
        }

        MKBackgroundShopItem* background = new MKBackgroundShopItem(name->value.GetString(), price->value.GetUint64(), textureFilePathsVector);
        m_Backgrounds.insert(std::pair<mkString, MKBackgroundShopItem*>(background->GetName(), background));
    }

    return true;
}

mkString MKBackgroundData::GetCachedPath() const
{
    return cocos2d::FileUtils::getInstance()->fullPathForFilename("Shop/Backgrounds.json");
}

// Interface Function(s)
const MKBackgroundShopItem* MKBackgroundData::GetBackground(const mkString& _backgroundName) const
{
    std::unordered_map<mkString, MKBackgroundShopItem*>::const_iterator iter = m_Backgrounds.find(_backgroundName);
    if (iter == m_Backgrounds.end())
    {
        return nullptr;
    }

    return iter->second;
}

const std::vector<MKBackgroundShopItem*> MKBackgroundData::GetBackgrounds() const
{
    std::vector<MKBackgroundShopItem*> result;

    for (std::unordered_map<mkString, MKBackgroundShopItem*>::const_iterator i = m_Backgrounds.begin(); i != m_Backgrounds.end(); ++i)
    {
        result.push_back(i->second);
    }

    return result;
}

NS_MK_END