#ifndef GAME_SCENE_H
#define GAME_SCENE_H

// Include MK
#include "MK/SceneManagement/MKScene.h"
#include "MK/Sprite/MKSprite.h"
#include "MK/Sprite/SpriteAnimation/MKSpriteAnimation.h"
#include "Game/Player/MKPlayer.h"
#include "Game/Background/MKBackground.h"
#include "Game/Spawner/MKSpawner.h"

USING_NS_MK

class Game : public MKScene
{
	typedef MKScene Super;

private:
    // Sound Names(s)
    const mkString m_BGMSoundName = "Game_BGM";

    // Sound IDs
    mkS32 m_BGMSoundId = MKAudioManager::INVALID_SOUND_ID;

    void StartBGM();
    void PauseBGM();
    void ResumeBGM();
    void StopBGM();

    // Variable(s)
    MKPlayer* m_Player = nullptr;
    MKBackground* m_Background = nullptr;
    MKSpawner* m_Spawner = nullptr;

    // UI
    cocos2d::Node* m_UINode = nullptr;
    mkU32 m_UINodeLayer = 0;
    cocos2d::Label* m_ScoreLabel = nullptr;

    cocos2d::Node* m_GameplayUINode = nullptr;
    cocos2d::Node* m_PauseUINode = nullptr;
    cocos2d::Node* m_GameOverUINode = nullptr;

    // Initialisation
    void InitialisePlayer();
    void InitialiseCamera();
    void InitialiseBackground();
    void InitialiseSpawner();

    // UI
    void CreateScoreUI();
    void DeleteScoreUI();
    void UpdateScoreUI();

    void CreateGameplayUI();
    void DeleteGameplayUI();

    void CreatePauseUI();
    void DeletePauseUI();

    void CreateGameOverUI();
    void DeleteGameOverUI();

    // Callbacks
    void OnPlayerDie();

    // Others
    void PauseGame();
    void ResumeGame();

    // Input Callbacks
	MK_DECL_INITIALISE_INPUT(Game);
	MK_DECL_DEINITIALISE_INPUT(Game);
	virtual void OnButton(EventCustom * _event) override;
	virtual void OnClick(EventCustom * _event) override;
	virtual void OnAxis(EventCustom * _event) override;

public:
    // Constructor(s) & Destructor
    Game();
    virtual ~Game();

    CREATEWITHPHYSICS_FUNC(Game);

    // Overrides
    virtual bool initWithPhysics() override;
    virtual void update(float _deltaTime) override;
    virtual void onEnter() override;
    virtual void onExit() override;
};

#endif // GAME_SCENE_H