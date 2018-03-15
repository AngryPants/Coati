#include "MKBackgroundLayer.h"

NS_MK_BEGIN

// Constructor(s) & Destructor
MKBackgroundLayer::MKBackgroundLayer()
    : MKSprite(TextureWrapMethod::REPEAT)
{
}

MKBackgroundLayer::~MKBackgroundLayer()
{
}

// Static Function(s)
MKBackgroundLayer* MKBackgroundLayer::create(const mkString& _textureFile, Vec2 _scrollSpeed)
{
    MKBackgroundLayer* backgroundLayer = new (std::nothrow) MKBackgroundLayer();
    if (backgroundLayer && backgroundLayer->init(_textureFile, _scrollSpeed))
    {
        backgroundLayer->autorelease();
        return backgroundLayer;
    }

    CC_SAFE_DELETE(backgroundLayer);
    return nullptr;
}

// Virtual Override(s)
mkBool MKBackgroundLayer::init(const mkString& _textureFile, Vec2 _scrollSpeed)
{
    if (!Super::initSpriteWithFile(_textureFile)) { return false; }

    // Set Variable(s)
    m_TextureFile = _textureFile;
    m_ScrollSpeed = _scrollSpeed;

    // Set Scale
    auto visibleSize = Director::getInstance()->getVisibleSize();
    mkF32 desiredWidth = visibleSize.width;
    mkF32 desiredHeight = visibleSize.height;
    mkF32 backgroundWidth = getContentSize().width;
    mkF32 backgroundHeight = getContentSize().height;
    mkF32 backgroundAspectRatio = backgroundWidth / backgroundHeight;
    setScale(desiredWidth / backgroundWidth, desiredHeight / backgroundHeight);
    SetTextureScale(getScaleX() / getScaleY(), 1.0f);

    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    scheduleUpdate();

    return true;
}

void MKBackgroundLayer::update(mkF32 _deltaTime)
{
    mkF32 textureOffsetU = m_ScrollSpeed.x / getScaledContentSize().width * GetTextureScale().x * _deltaTime;
    mkF32 textureOffsetV = m_ScrollSpeed.y / getScaledContentSize().height * GetTextureScale().y * _deltaTime;
    OffsetTexture(textureOffsetU, textureOffsetV);
}

NS_MK_END