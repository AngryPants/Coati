#include "MKBackground.h"

NS_MK_BEGIN

// Constructor(s) & Destructor
MKBackground::MKBackground()
{
}

MKBackground::~MKBackground()
{
}

// Static Function(s)
MKBackground* MKBackground::create()
{
    MKBackground* background = new (std::nothrow) MKBackground();
    if (background && background->init())
    {
        background->autorelease();
        return background;
    }

    CC_SAFE_DELETE(background);
    return nullptr;
}

// Interface Function(s)
void MKBackground::addLayer(MKBackgroundLayer* _layer)
{
    if (_layer == nullptr) { return; }

    m_Layers.push_back(_layer);
    addChild(_layer);

    if (m_Pause)
    {
        _layer->pause();
    }
    else
    {
        _layer->resume();
    }
}

void MKBackground::removeLayer(mkU32 _layerIndex)
{
    if (_layerIndex >= m_Layers.size()) { return; }

    removeChild(m_Layers[_layerIndex]);
    m_Layers.erase(m_Layers.begin() + _layerIndex);
}

void MKBackground::removeAllLayers()
{
    m_Layers.clear();
    removeAllChildren();
}

MKBackgroundLayer* MKBackground::getLayer(mkU32 _layerIndex)
{
    return (_layerIndex >= m_Layers.size()) ? nullptr : m_Layers[_layerIndex];
}

void MKBackground::pause()
{
    Super::pause();

    m_Pause = true;
    for (std::vector<MKBackgroundLayer*>::iterator iter = m_Layers.begin(); iter != m_Layers.end(); ++iter)
    {
        MKBackgroundLayer* layer = *iter;
        layer->pause();
    }
}

void MKBackground::resume()
{
    Super::resume();

    m_Pause = false;
    for (std::vector<MKBackgroundLayer*>::iterator iter = m_Layers.begin(); iter != m_Layers.end(); ++iter)
    {
        MKBackgroundLayer* layer = *iter;
        layer->resume();
    }
}

// Virtual Override(s)
mkBool MKBackground::init()
{
    if (!Super::init()) { return false; }

    m_Pause = false;

    return true;
}

NS_MK_END