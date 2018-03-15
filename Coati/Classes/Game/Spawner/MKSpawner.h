#ifndef MK_SPAWNER_H
#define MK_SPAWNER_H

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "MK/Common/MKMacros.h"
#include "MK/SceneManagement/MKScene.h"
#include "MK/Common/MKMathsHelper.h"
#include "../Player/MKPlayer.h"
#include "../Obstacles/MKMissileWarning.h"

// Include STL
#include <list>
#include <functional>
#include <unordered_set>

USING_NS_CC;
using namespace std;

NS_MK_BEGIN

// Foward Declaration(s)
class MKObstacle;
class MKPowerup;

class MKSpawner : public cocos2d::Node
{
    typedef cocos2d::Node Super;

protected:
    enum ObstacleType
    {
        SPIKE,
        MISSILE,
        SAW,
        LASER,

        NUM_OBSTACLE_TYPE,
    };

    enum PowerupType
    {
        SHIELD,

        NUM_POWERUP_TYPE,
    };

    // Variable(s)
    MKScene* m_Scene;
    MKPlayer* m_Player;

    mkF32 m_PlayerStartPositionX; // The position of the player when the wave starts.
    mkF32 m_WaveEndPositionX;
    mkF32 m_MinPlayHeight;
    mkF32 m_MaxPlayHeight;
    mkU32 m_WaveCount; // The number of waves that has passed.
    std::unordered_set<cocos2d::Node*> m_NodesToMoveDuringWaveStart;

    // Obstacles
    std::list<MKObstacle*> m_Obstacles;
    mkS32 m_WaveObstacleCount;
    mkS32 m_WaveObstacleCountIncreament;
    mkS32 m_WaveObstacleCountMax;
    // Make sure this is large enough that all obstacles will be able to spawn in time.
    // Some obstacles such as missiles require a few seconds to show the warning.
    mkF32 m_PlayerDistanceToFirstObstacle;
    mkF32 m_PlayerDistanceFromLastObstacle;

    std::unordered_set<MKMissileWarning*> m_MissileWarnings;

    // Powerups
    std::list<MKPowerup*> m_Powerups;
    mkS32 m_MinPowerupCount;
    mkS32 m_MaxPowerupCount;

    // Internal Function(s)
    mkF32 GetObstacleInterval() const;
    mkF32 GetScreenTop() const;
    mkF32 GetScreenBottom() const;
    mkF32 GetScreenLeft() const;
    mkF32 GetScreenRight() const;

    void DespawnAllObstacles();
    void DespawnMissileWarnings();
    void DespawnAllPowerups();

    // Obstacle Spawning
    void SpawnWave();
    MKObstacle* SpawnSpike(mkF32 _obstaclePositionX);
    MKObstacle* SpawnMissile(mkF32 _obstaclePositionX);
    MKObstacle* SpawnSaw(mkF32 _obstaclePositionX);
    MKObstacle* SpawnLaser(mkF32 _obstaclePositionX);

    // Powerup
    MKPowerup* SpawnShield(mkF32 _powerupPositionX);

    // Callbacks
    void RemoveMissileWarning(cocos2d::Node* _effect);

public:
    // Constructor(s) & Destructor
    MKSpawner();
    virtual ~MKSpawner();

    // Static Function(s)
    static MKSpawner* create(MKScene* _scene, MKPlayer* _player, mkF32 _playerStartPositionX, mkF32 _minPlayHeight, mkF32 _maxPlayHeight);

    // Interface Function(s)
    void AddNodeToMoveDuringWaveStart(cocos2d::Node* _node); // During the wave start, we reset the position of these nodes (in addition to the player).
    void RemoveNodeToMoveDuringWaveStart(cocos2d::Node* _node);
    void pause() override; // Call this function when opening the pause scene.
    void resume() override;  // Call this function when closing the pause scene.

    // Virtual Override(s)
    virtual mkBool init(MKScene* _scene, MKPlayer* _player, mkF32 _playerStartPositionX, mkF32 _minPlayHeight, mkF32 _maxPlayHeight);
    virtual void update(mkF32 _deltaTime) override;
};

NS_MK_END

#endif