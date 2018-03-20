// Include MK
#include "MKSpawner.h"
#include "MK/Common/MKMathsHelper.h"
#include "MK/Actions/MKFollowNodeAction.h"
#include "MK/Actions/MKRepeatActionInstantForever.h"

// Include Game
#include "Game/Obstacles/MKMissile.h"
#include "Game/Obstacles/MKSpike.h"
#include "Game/Obstacles/MKSaw.h"
#include "Game/Obstacles/MKLaser.h"
#include "Game/Powerups/MKPowerups.h"

NS_MK_BEGIN

// Constructor(s) & Destructor
MKSpawner::MKSpawner()
{
}

MKSpawner::~MKSpawner()
{
    DespawnAllObstacles();
    DespawnMissileWarnings();
    DespawnAllPowerups();
}

// Internal Function(s)
mkF32 MKSpawner::GetObstacleInterval() const
{
    return Director::getInstance()->getVisibleSize().height;
}

mkF32 MKSpawner::GetScreenTop() const
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    return m_Scene->getDefaultCamera()->getPositionX() + visibleSize.height * 0.5f;
}

mkF32 MKSpawner::GetScreenBottom() const
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    return m_Scene->getDefaultCamera()->getPositionX() - visibleSize.height * 0.5f;
}

mkF32 MKSpawner::GetScreenLeft() const
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    return m_Scene->getDefaultCamera()->getPositionX() - visibleSize.width * 0.5f;
}

mkF32 MKSpawner::GetScreenRight() const
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    return m_Scene->getDefaultCamera()->getPositionX() + visibleSize.width * 0.5f;
}

void MKSpawner::DespawnAllObstacles()
{
    for (std::list<MKObstacle*>::iterator i = m_Obstacles.begin(); i != m_Obstacles.end(); ++i)
    {
        MKObstacle* obstacleNode = *i;
        obstacleNode->removeFromParent();
    }

    m_Obstacles.clear();
}

void MKSpawner::DespawnMissileWarnings()
{
    for (std::unordered_set<MKMissileWarning*>::iterator iter = m_MissileWarnings.begin(); iter != m_MissileWarnings.end(); ++iter)
    {
        (*iter)->setFinishCallback(nullptr);
    }

    m_MissileWarnings.clear();
}

void MKSpawner::DespawnAllPowerups()
{
    for (std::list<MKPowerup*>::iterator i = m_Powerups.begin(); i != m_Powerups.end(); ++i)
    {
        MKPowerup* powerupNode = *i;
        powerupNode->removeFromParent();
    }

    m_Powerups.clear();
}

void MKSpawner::SpawnWave()
{
    mkF32 distanceMoved = m_PlayerStartPositionX - m_Player->getPositionX();
    m_Player->setPositionX(m_PlayerStartPositionX);
    for (std::unordered_set<cocos2d::Node*>::iterator i = m_NodesToMoveDuringWaveStart.begin(); i != m_NodesToMoveDuringWaveStart.end(); ++i)
    {
        cocos2d::Node* nodeToMove = *i;
        nodeToMove->setPositionX(nodeToMove->getPositionX() + distanceMoved);
    }

    // Obstacle Spawning
    mkF32 obstaclePositionX = m_PlayerStartPositionX + m_PlayerDistanceToFirstObstacle;
    mkF32 obstacleInterval = GetObstacleInterval();
    m_WaveEndPositionX = m_PlayerStartPositionX + m_PlayerDistanceToFirstObstacle + (obstacleInterval * (mkF32)m_WaveObstacleCount) + m_PlayerDistanceFromLastObstacle;
    for (mkU32 i = 0; i < m_WaveObstacleCount; ++i)
    {
        ObstacleType obstacleType = (ObstacleType)MKMathsHelper::RandomInt(0, (mkS32)NUM_OBSTACLE_TYPE);
    
        MKObstacle* obstacle = nullptr;
        switch (obstacleType)
        {
        case MinamiKotori::MKSpawner::SPIKE:
            obstacle = SpawnSpike(obstaclePositionX);
            break;
        case MinamiKotori::MKSpawner::MISSILE:
            obstacle = SpawnMissile(obstaclePositionX);
            break;
        case MinamiKotori::MKSpawner::SAW:
            obstacle = SpawnSaw(obstaclePositionX);
            break;
        case MinamiKotori::MKSpawner::LASER:
            obstacle = SpawnLaser(obstaclePositionX);
            break;
        default:
            MK_ASSERT(false);
            break;
        }
        m_Obstacles.push_back(obstacle);
    
        obstaclePositionX += obstacleInterval;
    }

    // Powerup Spawning
    mkF32 playDistance = m_WaveEndPositionX - (m_PlayerStartPositionX + m_PlayerDistanceToFirstObstacle);
    mkF32 powerupSpawnInterval = playDistance / static_cast<mkF32>(m_WaveObstacleCount);
    mkS32 numPowerups = MKMathsHelper::RandomInt(m_MinPowerupCount, m_MaxPowerupCount);
    numPowerups = MKMathsHelper::Min(numPowerups, m_WaveObstacleCount / 5);
    for (mkU32 i = 0; i < numPowerups; ++i)
    {
        PowerupType powerupType = (PowerupType)MKMathsHelper::RandomInt(0, (mkS32)NUM_POWERUP_TYPE);
        mkF32 powerupPositionX = powerupSpawnInterval * static_cast<mkF32>(i) + m_PlayerStartPositionX + m_PlayerDistanceToFirstObstacle;
    
        MKPowerup* powerup = nullptr;
        switch (powerupType)
        {
        case MinamiKotori::MKSpawner::SHIELD:
            powerup = SpawnShield(powerupPositionX);
            break;
        default:
            MK_ASSERT(false);
            break;
        }
        m_Powerups.push_back(powerup);
    }

    // Update Wave Count & Obstacle Count
    ++m_WaveCount;
    m_WaveObstacleCount = MKMathsHelper::Min<mkS32>(m_WaveObstacleCountIncreament + m_WaveObstacleCount, m_WaveObstacleCountMax);
}

MKObstacle* MKSpawner::SpawnSpike(mkF32 _obstaclePositionX)
{
    mkS32 minSpikeCount = 2;
    mkS32 maxSpikeCount = 4;
    MKSpike* spike = MKSpike::Create(m_Scene, (mkU32)MKMathsHelper::RandomInt(minSpikeCount, maxSpikeCount + 1));
    spike->setPosition(Vec2(_obstaclePositionX, m_MinPlayHeight + (spike->getScaledContentSize().height * 0.5f)));
    m_Scene->addChild(spike);

    return spike;
}

MKObstacle* MKSpawner::SpawnMissile(mkF32 _obstaclePositionX)
{
    // Find out the time it takes for the player to reach _obstaclePosition.
    mkF32 playerVelocity = m_Player->GetHorizontalVelocity();
    mkF32 playerDistanceToObstacle = _obstaclePositionX - m_Player->getPosition().x;
    mkF32 playerTimeToObstacle = playerDistanceToObstacle / playerVelocity;

    // Given the missile's velocity, find out the distance it can travel in that time.
    MKMissile* missile = MKMissile::Create(m_Scene);
    mkF32 missileVelocity = missile->GetHorizontalVelocity();
    mkF32 missileTravelDistance = playerTimeToObstacle * missileVelocity;
    mkF32 missilePositionX = _obstaclePositionX - missileTravelDistance;
    mkF32 midPlayHeight = (m_MinPlayHeight + m_MaxPlayHeight) * 0.5f;
    mkF32 playHeightDifference = m_MaxPlayHeight - m_MinPlayHeight;
    mkF32 missilePositionY = MKMathsHelper::RandomInt((mkS32)midPlayHeight - playHeightDifference * 0.4f, midPlayHeight + playHeightDifference * 0.4f);
    missile->setPosition(Vec2(missilePositionX, missilePositionY));
    m_Scene->addChild(missile);

    // Find out the time it takes for the missile to appear on screen.
    mkF32 missileLeft = missile->getPosition().x - missile->getScaledContentSize().width * 0.5f;
    mkF32 missileDistanceToScreenRight = missileLeft - GetScreenRight();
    mkF32 relativeVelocity = m_Player->GetHorizontalVelocity() - missileVelocity;
    mkF32 missileTimeToScreen = missileDistanceToScreenRight / relativeVelocity;
    mkF32 missileWarningDuration = 1.0f;
    mkF32 missileWarningDelay = missileTimeToScreen - missileWarningDuration;

    DelayTime* warningDelay = DelayTime::create(missileWarningDelay);
    CallFunc* spawnMissileWarning = CallFunc::create(
        [=]() -> void
        {
            auto visibleSize = Director::getInstance()->getVisibleSize();

            cocos2d::Camera* sceneCamera = m_Scene->getDefaultCamera();
            MKMissileWarning* missileWarning = MKMissileWarning::create(1.0f, sceneCamera, Vec2(visibleSize.width * 0.45f, missilePositionY - sceneCamera->getPositionY()), CC_CALLBACK_1(MKSpawner::RemoveMissileWarning, this));
            m_MissileWarnings.insert(missileWarning);
            m_Scene->addChild(missileWarning);
        }
    );
    runAction(Sequence::createWithTwoActions(warningDelay, spawnMissileWarning));

    return missile;
}

MKObstacle* MKSpawner::SpawnSaw(mkF32 _obstaclePositionX)
{
    // Randomise the start offset percentage.
    mkF32 offsetPercentage = (mkF32)MKMathsHelper::RandomInt(0, 100) / 100.0f;

    MKSaw* saw = MKSaw::Create(m_Scene, offsetPercentage);
    mkF32 sawOffsetY = sin(offsetPercentage * MKMathsHelper::TWO_PI) * saw->GetMoveDisplacement();
    saw->setPosition(Vec2(_obstaclePositionX, (m_MaxPlayHeight + m_MinPlayHeight) * 0.5f + sawOffsetY));
    m_Scene->addChild(saw);

    return saw;
}

MKObstacle* MKSpawner::SpawnLaser(mkF32 _obstaclePositionX)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Find out the time it takes for the player to reach _obstaclePosition.
    mkF32 playerVelocity = m_Player->GetHorizontalVelocity();
    mkF32 playerDistanceToObstacle = _obstaclePositionX - m_Player->getPosition().x;
    mkF32 playerTimeToObstacle = playerDistanceToObstacle / playerVelocity;
    mkF32 laserDelay = playerTimeToObstacle - MKLaser::LASER_MOVE_DOWN_DURATION - MKLaser::LASER_BEAM_CHARGE_DURATION;

    MKLaser* laser = MKLaser::Create(m_Scene, laserDelay);
    laser->setPosition(_obstaclePositionX, visibleSize.height * 1.1f);
    m_Scene->addChild(laser);

    return laser;
}

// Powerup
MKPowerup* MKSpawner::SpawnShield(mkF32 _powerupPositionX)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    MKShield* shield = MKShield::create(m_Scene);
    mkF32 positionY = (mkF32)MKMathsHelper::RandomInt(m_MinPlayHeight + shield->getScaledContentSize().height * 0.5f, m_MaxPlayHeight - shield->getScaledContentSize().height * 0.5f);
    shield->setPosition(Vec2(_powerupPositionX, positionY));
    m_Scene->addChild(shield);

    return shield;
}

// Callback
void MKSpawner::RemoveMissileWarning(cocos2d::Node* _warning)
{
    m_MissileWarnings.erase(static_cast<MKMissileWarning*>(_warning));
}

// Static Function(s)
MKSpawner* MKSpawner::create(MKScene* _scene, MKPlayer* _player, mkF32 _playerStartPositionX, mkF32 _minPlayHeight, mkF32 _maxPlayHeight)
{
    MKSpawner* spawner = new (std::nothrow) MKSpawner();
    if (spawner && spawner->init(_scene, _player, _playerStartPositionX, _minPlayHeight, _maxPlayHeight))
    {
        spawner->autorelease();
        return spawner;
    }

    CC_SAFE_DELETE(spawner);
    return nullptr;
}

// Interface Function(s)
void MKSpawner::AddNodeToMoveDuringWaveStart(cocos2d::Node* _node)
{
    if (_node == nullptr) { return; }
    m_NodesToMoveDuringWaveStart.insert(_node);
}

void MKSpawner::RemoveNodeToMoveDuringWaveStart(cocos2d::Node* _node)
{
    if (_node == nullptr) { return; }
    m_NodesToMoveDuringWaveStart.erase(_node);
}

void MKSpawner::pause()
{
    Super::pause();

    for (std::list<MKObstacle*>::iterator i = m_Obstacles.begin(); i != m_Obstacles.end(); ++i)
    {
        (*i)->pause();
    }

    for (std::unordered_set<MKMissileWarning*>::iterator i = m_MissileWarnings.begin(); i != m_MissileWarnings.end(); ++i)
    {
        (*i)->pause();
    }
}

void MKSpawner::resume()
{
    Super::resume();

    for (std::list<MKObstacle*>::iterator i = m_Obstacles.begin(); i != m_Obstacles.end(); ++i)
    {
        (*i)->resume();
    }

    for (std::unordered_set<MKMissileWarning*>::iterator i = m_MissileWarnings.begin(); i != m_MissileWarnings.end(); ++i)
    {
        (*i)->resume();
    }
}

// Virtual Override(s)
mkBool MKSpawner::init(MKScene* _scene, MKPlayer* _player, mkF32 _playerStartPositionX, mkF32 _minPlayHeight, mkF32 _maxPlayHeight)
{
    if (!Super::init()) { return false; }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    m_Scene = _scene;
    m_Player = _player;

    m_PlayerStartPositionX = _playerStartPositionX;
    m_WaveEndPositionX = m_PlayerStartPositionX;
    m_MinPlayHeight = _minPlayHeight;
    m_MaxPlayHeight = _maxPlayHeight;
    m_WaveCount = 0;
    
    m_WaveObstacleCount = 8; // This number increases by m_WaveObstacleCountIncreament per wave.
    m_WaveObstacleCountIncreament = 4;
    m_WaveObstacleCountMax = 30;
    m_PlayerDistanceToFirstObstacle = MKMathsHelper::Max<mkF32>(_player->GetHorizontalVelocity() * 5.0f, visibleSize.width * 2.0f);
    m_PlayerDistanceFromLastObstacle = visibleSize.width;

    m_MinPowerupCount = 0;
    m_MaxPowerupCount = 5;

    scheduleUpdate();

    return true;
}

void MKSpawner::update(mkF32 _deltaTime)
{
    if (m_Player->getPositionX() > m_WaveEndPositionX)
    {
        DespawnAllObstacles();
        DespawnMissileWarnings();
        DespawnAllPowerups();
        SpawnWave();
    }
}

NS_MK_END