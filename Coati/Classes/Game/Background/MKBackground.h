#pragma once

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "../../MK/Common/MKMacros.h"
#include "../../MK/Sprite/MKSprite.h"
#include "MKBackgroundLayer.h"

// Include STL
#include <list>
#include <vector>

NS_MK_BEGIN

class MKBackground : public cocos2d::Node
{
    typedef cocos2d::Node Super;

protected:
    std::vector<MKBackgroundLayer*> m_Layers;
    mkBool m_Pause = false;

public:
    // Constructor(s) & Destructor
    MKBackground();
    virtual ~MKBackground();

    // Static Function(s)
    static MKBackground* create();

    // Interface Function(s)
    mkU32 getLayerCount() const { return m_Layers.size(); }
    void addLayer(MKBackgroundLayer* _layer);
    void removeLayer(mkU32 _layerIndex);
    void removeAllLayers();
    MKBackgroundLayer* getLayer(mkU32 _layerIndex);
    void pause() override;
    void resume() override;

    // Virtual Override(s)
    virtual mkBool init() override;
};

NS_MK_END