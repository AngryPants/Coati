#pragma once

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "MK/Common/MKMathsHelper.h"
#include "MK/Sprite/MKSprite.h"
#include "MK/SceneManagement/MKScene.h"
#include "MK/Gameplay/MKGameplayMacros.h"

NS_MK_BEGIN

class MKPowerup : public cocos2d::Node
{
    typedef cocos2d::Node Super;

private:
    MKScene* m_Scene = nullptr;

protected:
    // Collision
    EventListenerPhysicsContact * m_ContactListener = nullptr;

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

public:
    // Constructor(s) & Destructor
    MKPowerup(MKScene* _scene)
        : m_Scene(_scene)
    {}
    virtual ~MKPowerup() {}

    // Interface Function(s)
    MKScene* GetScene() { return m_Scene; }
    const MKScene* GetScene() const { return m_Scene; }
};

NS_MK_END