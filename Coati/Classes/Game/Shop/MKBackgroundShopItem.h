#pragma once

// Include MK
#include "MK/Shop/MKShopItem.h"

// Include STL
#include <vector>

NS_MK_BEGIN

class MKBackgroundShopItem : public MKShopItem
{
private:
    // Variable(s)
    std::vector<mkString> m_TextureFilePaths;

public:
    // Constructor(s) & Destructor
    MKBackgroundShopItem(const mkString& _name, mkU64 _price, const std::vector<mkString>& _textureFilePaths)
        : MKShopItem(_name, _price), m_TextureFilePaths(_textureFilePaths)
    {}
    virtual ~MKBackgroundShopItem() {}

    // Interface Function(s)
    inline mkU32 GetTextureFilesCount() const { return m_TextureFilePaths.size(); }
    inline const mkString& GetTextureFile(mkU32 _index) const { return m_TextureFilePaths[_index]; }
};

NS_MK_END