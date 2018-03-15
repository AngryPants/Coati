#pragma once

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "../../MK/Common/MKMacros.h"
#include "../../MK/Sprite/MKSprite.h"

NS_MK_BEGIN

class MKBackgroundLayer : public MKSprite
{
    typedef MKSprite Super;

protected:
    // Variable(s)
    mkString m_TextureFile;
    Vec2 m_ScrollSpeed;

public:
    // Constructor(s) & Destructor
    MKBackgroundLayer();
    virtual ~MKBackgroundLayer();

    // Static Function(s)
    static MKBackgroundLayer* create(const mkString& _textureFile, Vec2 _scrollSpeed);

    // Interface Function(s)
    const mkString& getTextureFile() const { return m_TextureFile; }
    void setScrollSpeed(Vec2 _scrollSpeed) { m_ScrollSpeed = _scrollSpeed; }
    Vec2 getScrollSpeed() const { return m_ScrollSpeed; }

    // Virtual Override(s)
    virtual mkBool init(const mkString& _textureFile, Vec2 _scrollSpeed);
    virtual void update(mkF32 _deltaTime);
};

NS_MK_END