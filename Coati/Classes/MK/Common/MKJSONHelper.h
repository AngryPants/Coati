#ifndef MK_JSON_HELPER_H
#define MK_JSON_HELPER_H

// Include MK
#include "MKMacros.h"
#include "MKAssertions.h"

// Include STL
#include <fstream>

// Include JSON
#include "external/json/filewritestream.h"
#include "external/json/filereadstream.h"
#include "external/json/stringbuffer.h"
#include "external/json/writer.h"
#include "external/json/document.h"
#include "cocos2d.h"

using namespace RAPIDJSON_NAMESPACE;

NS_MK_BEGIN

class MKJSONHelper
{
public:
    static mkString JsonToString(RAPIDJSON_NAMESPACE::Document& _jsonObject)
    {
        RAPIDJSON_NAMESPACE::StringBuffer buffer;
        RAPIDJSON_NAMESPACE::Writer<RAPIDJSON_NAMESPACE::StringBuffer> jsonWriter(buffer);
        _jsonObject.Accept(jsonWriter);

        return buffer.GetString();
    }

    static bool JsonToFile(RAPIDJSON_NAMESPACE::Document& _jsonObject, const mkString& _filePath)
    {
        return cocos2d::FileUtils::getInstance()->writeStringToFile(JsonToString(_jsonObject), _filePath);
    }

    static bool LoadFromJSON(RAPIDJSON_NAMESPACE::Document& _document, const mkString& _filePath)
    {
        ssize_t contentSize = 0;
        const char* fileContent = (const char*)cocos2d::FileUtils::getInstance()->getFileData(_filePath, "r", &contentSize);

        // If the content size is 0, it failed to load.
        if (contentSize == 0)
        {
            return false;
        }

        // Add a null terminator.
        mkString fileContentWithNullTerminator(fileContent);
        size_t lastPosition = fileContentWithNullTerminator.rfind("}");
        fileContentWithNullTerminator = fileContentWithNullTerminator.substr(0, lastPosition + 1);

        // Parse the document.
        _document.Parse<0>(fileContentWithNullTerminator.c_str());

        return true;
    }
};

NS_MK_END

#endif // MK_JSON_HELPER_H
