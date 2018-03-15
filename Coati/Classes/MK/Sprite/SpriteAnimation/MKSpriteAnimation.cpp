// Include MK
#include "MKSpriteAnimation.h"
#include "../../Common/MKJSONHelper.h"
#include "../../Common/MKAssertions.h"
#include "../../Actions/MKRemoveFromParentAction.h"

NS_MK_BEGIN

// JSON Data Names
const mkString MKSpriteAnimation::STATE_ARRAY_JSON_DATA_NAME = "States";
const mkString MKSpriteAnimation::STATE_NAME_JSON_DATA_NAME = "Name";
const mkString MKSpriteAnimation::SPRITE_FRAMES_JSON_DATA_NAME = "Frames";

// Constructor(s) & Destructor
MKSpriteAnimation::MKSpriteAnimation()
    : MKSprite(MKSprite::REPEAT)
{
}

MKSpriteAnimation::~MKSpriteAnimation()
{
    ClearAllTickets();
    ClearAllStates();
}

// Virtual Function(s)
mkBool MKSpriteAnimation::initSpriteAnimation(const mkString& _pListFile, const mkString& _jsonFile, const mkString& _vertexShader, const mkString& _fragmentShader)
{
    if (!Super::initSprite(_vertexShader, _fragmentShader))
    {
        return false;
    }

    // Add the PList to the SpriteFrameCache.
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(_pListFile);

    // Load the JSON file.
    if (!LoadJSONFile(_jsonFile))
    {
        return false;
    }

    // Set the default sprite frame.
    MKSpriteAnimationState* animationState = m_States.begin()->second;
    if (animationState == nullptr)
    {
        return false;
    }
    setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(animationState->GetFrame(0)));

    return true;
}

// Internal Function(s)
mkBool MKSpriteAnimation::LoadJSONFile(const mkString& _fileLocation)
{
    Document dataDocument;
    MK_ASSERT((MKJSONHelper::LoadFromJSON(dataDocument, _fileLocation)));

    // Load Animation States
    auto statesArray = dataDocument.FindMember(STATE_ARRAY_JSON_DATA_NAME.c_str());
    MK_ASSERT(statesArray->value.IsArray());
    for (RAPIDJSON_NAMESPACE::Value::ConstValueIterator i = statesArray->value.Begin(); i != statesArray->value.End(); ++i)
    {
        // Get the state name.
        auto stateName = i->FindMember(STATE_NAME_JSON_DATA_NAME.c_str());
        MK_ASSERT(stateName->value.IsString());

        // Get the state frames.
        std::vector<mkString> spriteFramesVector;
        auto spriteFrames = i->FindMember(SPRITE_FRAMES_JSON_DATA_NAME.c_str());
        MK_ASSERT(spriteFrames->value.IsArray());
        for (RAPIDJSON_NAMESPACE::Value::ConstValueIterator j = spriteFrames->value.Begin(); j != spriteFrames->value.End(); ++j)
        {
            MK_ASSERT(j->IsString());
            spriteFramesVector.push_back(j->GetString());
        }
        MK_ASSERT(spriteFramesVector.empty() == false); // An animation state cannot be empty.

        // Add the state to m_States.
        MKSpriteAnimationState* spriteAnimationState = new MKSpriteAnimationState(stateName->value.GetString(), spriteFramesVector);
        MK_ASSERT(m_States.find(spriteAnimationState->GetName()) == m_States.end()); // Ensure that every state has a different name.
        m_States.insert(std::pair<mkString, MKSpriteAnimationState*>(spriteAnimationState->GetName(), spriteAnimationState));
    }

    return true;
}

void MKSpriteAnimation::ClearAllStates()
{
    for (std::unordered_map<mkString, MKSpriteAnimationState*>::iterator i = m_States.begin(); i != m_States.end(); ++i)
    {
        delete i->second;
    }
    m_States.clear();
}

void MKSpriteAnimation::StopCurrentTicket()
{
    if (m_CurrentAnimation == nullptr) { return; }

    stopAction(m_CurrentAnimation);
    m_CurrentAnimation->release();
    m_CurrentAnimation = nullptr;
}

void MKSpriteAnimation::PlayNextTicket()
{
    // Stop the current animation.
    StopCurrentTicket();
    
    // Check that there is a ticket to play.
    if (m_TicketQueue.empty()) { return; }

    // Get the next ticket.
    MKSpriteAnimationTicket* ticket = m_TicketQueue.front();
    m_TicketQueue.pop();

    // Get the animation state.
    MKSpriteAnimationState* animationState = GetState(ticket->GetStateName());
    MK_ASSERT(animationState != nullptr);

    // Get the animation from the animation state.
    cocos2d::Animation* animation = animationState->GetAnimation();
    MK_ASSERT(animation != nullptr);

    // Set the sprite frame.
    setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(animationState->GetFrame(0)));

    // Update the animation parameters.
    animation->setDelayPerUnit(ticket->GetDuration() / static_cast<mkF32>(animationState->GetFrameCount()));
    if (ticket->GetLoopCount() == MKSpriteAnimationTicket::INFINITE_LOOPS)
    {
        animation->setLoops(1);
    }
    else
    {
        animation->setLoops(ticket->GetLoopCount());
    }

    // Run the action.
    if (ticket->GetLoopCount() == MKSpriteAnimationTicket::INFINITE_LOOPS)
    {
        m_CurrentAnimation = RepeatForever::create(Animate::create(animation));
    }
    else
    {
        Animate* animateAction = Animate::create(animation);
        if (ticket->GetDestroyOnFinish())
        {
            MKRemoveFromParentAction* removeFromParentAction = MKRemoveFromParentAction::Create();
            m_CurrentAnimation = Sequence::create(animateAction, removeFromParentAction, nullptr);
        }
        else
        {
            CallFunc* callbackAction = CallFunc::create(CC_CALLBACK_0(MKSpriteAnimation::PlayNextTicket, this));
            m_CurrentAnimation = Sequence::create(animateAction, callbackAction, nullptr);
        }
    }

    m_CurrentAnimation->retain();
    runAction(m_CurrentAnimation);

    // Delete the ticket.
    ticket->release();
}

MKSpriteAnimationState* MKSpriteAnimation::GetState(const mkString& _stateName)
{
    std::unordered_map<mkString, MKSpriteAnimationState*>::iterator iter = m_States.find(_stateName);
    return iter == m_States.end() ? nullptr : iter->second;
}

const MKSpriteAnimationState* MKSpriteAnimation::GetState(const mkString& _stateName) const
{
    std::unordered_map<mkString, MKSpriteAnimationState*>::const_iterator iter = m_States.find(_stateName);
    return iter == m_States.end() ? nullptr : iter->second;
}

// Interface Function(s)
void MKSpriteAnimation::AddTicketToQueue(MKSpriteAnimationTicket* _ticket)
{
    MK_ASSERT(_ticket != nullptr);
    MK_ASSERT(m_States.find(_ticket->GetStateName()) != m_States.end()); // Ensure that the state exists.
    _ticket->retain();
    m_TicketQueue.push(_ticket);

    // If there are no current animations, play this new ticket.
    // Otherwise, this ticket will be played when it is its turn.
    if (m_CurrentAnimation == nullptr)
    {
        PlayNextTicket();
    }
}

void MKSpriteAnimation::SkipCurrentTicket()
{
    StopCurrentTicket();
    PlayNextTicket();
}

void MKSpriteAnimation::ClearAllTickets()
{
    StopCurrentTicket();

    // Delete all tickets.
    while (m_TicketQueue.empty() == false)
    {
        m_TicketQueue.front()->release();
        m_TicketQueue.pop();
    }
}

// Static Function(s)
MKSpriteAnimation* MKSpriteAnimation::Create(const mkString& _pListFile, const mkString& _jsonFile)
{
    // Create the animation handler.
    MKSpriteAnimation* spriteAnimation = new (std::nothrow) MKSpriteAnimation();
    if (spriteAnimation && spriteAnimation->initSpriteAnimation(_pListFile, _jsonFile))
    {
        spriteAnimation->autorelease();
        return spriteAnimation;
    }

    CC_SAFE_DELETE(spriteAnimation);
    return nullptr;
}

NS_MK_END