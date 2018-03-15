#ifndef MK_OBSTACLE_H
#define MK_OBSTACLE_H

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "../../MK/Common/MKMathsHelper.h"
#include "../../MK/Sprite/MKSprite.h"
#include "../../MK/SceneManagement/MKScene.h"
#include "../../MK/Gameplay/MKGameplayMacros.h"

NS_MK_BEGIN

// Forward Declaration(s)
class MKPlayer;

class MKObstacle : public cocos2d::Node
{
    typedef cocos2d::Node Super;

protected:
    // Variable(s)
    MKScene* m_Scene;

    // Collision
    EventListenerPhysicsContact* m_ContactListener = nullptr;

    // Collision Callback
    virtual mkBool OnContactBegin(cocos2d::PhysicsContact& _contact)
    {
        PhysicsShape* physicsShapeA = _contact.getShapeA();
        PhysicsShape* physicsShapeB = _contact.getShapeB();
        // Ignore this collision if we're not involved.
        if (physicsShapeA->getBody() != _physicsBody &&
            physicsShapeB->getBody() != _physicsBody)
        {
            return false;
        }

        PhysicsBody* otherPhysicsBody = (physicsShapeA->getBody() == getPhysicsBody()) ? physicsShapeB->getBody() : physicsShapeA->getBody();
        if (!NS_MK::MKMathsHelper::CompareBitmasks<mkS32>(getPhysicsBody()->getContactTestBitmask(), otherPhysicsBody->getCategoryBitmask()))
        {
            return false;
        }

        return true;
    }

    // Internal Function(s)
    mkBool IsOnScreen(mkF32 _obstacleLeft, mkF32 _obstacleRight, mkF32 _obstacleTop, mkF32 _obstacleBottom)
    {
        if (_obstacleLeft > GetScreenRight()) { return false; }
        if (_obstacleRight < GetScreenLeft()) { return false; }
        if (_obstacleBottom > GetScreenTop()) { return false; }
        if (_obstacleTop < GetScreenBottom()) { return false; }

        return true;
    }

    mkF32 MKObstacle::GetScreenTop() const
    {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        return m_Scene->getDefaultCamera()->getPositionX() + visibleSize.height * 0.5f;
    }

    mkF32 MKObstacle::GetScreenBottom() const
    {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        return m_Scene->getDefaultCamera()->getPositionX() - visibleSize.height * 0.5f;
    }

    mkF32 MKObstacle::GetScreenRight() const
    {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        return m_Scene->getDefaultCamera()->getPositionX() + visibleSize.width * 0.5f;
    }

    mkF32 MKObstacle::GetScreenLeft() const
    {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        return m_Scene->getDefaultCamera()->getPositionX() - visibleSize.width * 0.5f;
    }

public:
    // Interface Function(s)
    MKScene* GetScene() { return m_Scene; }
    const MKScene* GetScene() const { return m_Scene; }
    
    const EventListenerPhysicsContact* GetContactListener() const { return m_ContactListener; }
    EventListenerPhysicsContact* GetContactListener() { return m_ContactListener; }

protected:
    // Constructor(s) & Destructor
    MKObstacle(MKScene* _scene)
        : m_Scene(_scene)
    {}
    virtual ~MKObstacle() {}
};

NS_MK_END

#endif // GT_OBSTACLENODE_H