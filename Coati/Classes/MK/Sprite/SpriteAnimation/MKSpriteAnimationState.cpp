// Include MK
#include "MKSpriteAnimationState.h"
#include "MK/Common/MKAssertions.h"

NS_MK_BEGIN

// Constructor(s) & Destructor
MKSpriteAnimationState::MKSpriteAnimationState(const mkString& _name, const std::vector<mkString>& _frames)
    : m_Name(_name), m_Frames(_frames)
{
    MK_ASSERT(m_Frames.empty() == false);

    m_Animation = cocos2d::Animation::create();
    m_Animation->retain();

    for (std::vector<mkString>::iterator i = m_Frames.begin(); i != m_Frames.end(); ++i)
    {
        cocos2d::SpriteFrame* spriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(*i);
        MK_ASSERT(spriteFrame != nullptr);
        m_Animation->addSpriteFrame(spriteFrame);
    }
}

MKSpriteAnimationState::~MKSpriteAnimationState()
{
    m_Animation->release();
}

NS_MK_END