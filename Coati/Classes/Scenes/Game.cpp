// Include Game
#include "Game.h"

// Include MK
#include "MK/Audio/MKAudioManager.h"
#include "MK/SceneManagement/MKSceneManager.h"
#include "Game/GameData/MKPlayerData.h"
#include "Game/GameData/MKBackgroundData.h"
#include "MK/GameData/MKGameDataLoader.h"
#include "MK/Common/MKAssertions.h"
#include "MK/UI/MKUIHelper.h"
#include "MK/Gameplay/MKGameplayMacros.h"
#include "MK/Gameplay/MKCollisionCategory.h"
#include "MK/Actions/MKFollowNodeAction.h"

// Constructor(s) & Destructor
Game::Game()
{
}

Game::~Game()
{
    if (m_GameplayUINode)
    {
        m_GameplayUINode->release();
    }
    if (m_PauseUINode)
    {
        m_PauseUINode->release();
    }
}

// Overrides
bool Game::initWithPhysics()
{
    if (!Super::initWithPhysics()) { return false; }

    // Load Player Data
    MKPlayerData* playerData = MKGameDataLoader::GetInstance()->GetGameData<MKPlayerData>();
    playerData->LoadData(playerData->GetWritablePath());

    // Let's do some physics.
    getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // Initialisation
    InitialisePlayer();
    InitialiseCamera();
    InitialiseBackground();
    InitialiseSpawner();

    // UI
    m_UINodeLayer = 1000;
    m_UINode = cocos2d::Node::create();
    addChild(m_UINode, m_UINodeLayer);
    CreateScoreUI();
    CreateGameplayUI();

    // Schedule Update
    scheduleUpdate();

    return true;
}

void Game::update(float _deltaTime)
{
    auto visibleSize = _director->getVisibleSize();
    m_UINode->setPosition(getDefaultCamera()->getPosition() - Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f));
    
    UpdateScoreUI();
}

void Game::onEnter()
{
    Super::onEnter();
	InitialiseInput();
	MKInputManager::GetInstance()->SetCurrentContext(MK_INPUT_CONTEXT_GAMEPLAY);
    StartBGM();
}

void Game::onExit()
{
    Super::onExit();
	DeinitialiseInput();
	MKInputManager::GetInstance()->SetCurrentContext(MK_INPUT_CONTEXT_DEFAULT);
    StopBGM();
}

// Initialisation
void Game::InitialisePlayer()
{
    auto visibleSize = _director->getVisibleSize();

    m_Player = MKPlayer::Create(visibleSize.height * 0.15f, this);
    m_Player->setPositionX(0.0f);
    m_Player->SetDieCallback(CC_CALLBACK_0(Game::OnPlayerDie, this));

    addChild(m_Player);
}

void Game::InitialiseCamera()
{
    auto visibleSize = _director->getVisibleSize();

    cocos2d::Vec2 cameraOffset(visibleSize.width * 0.3f, 0.0f);
    getDefaultCamera()->runAction(RepeatForever::create(MKFollowNodeAction::Create(1.0f, m_Player, MinamiKotori::MKFollowNodeAction::X, cameraOffset)));
}

void Game::InitialiseBackground()
{
    MKPlayerData* playerData = MKGameDataLoader::GetInstance()->GetGameData<MKPlayerData>();

    MKBackgroundData* backgroundData = MKGameDataLoader::GetInstance()->GetGameData<MKBackgroundData>();
    const MKBackgroundShopItem* equippedBackground = backgroundData->GetBackground(playerData->GetEquippedBackground());
    if (equippedBackground == nullptr)
    {
        equippedBackground = backgroundData->GetBackground(MKBackgroundData::DEFAULT_BACKGROUND_NAME);
        playerData->SetEquippedBackground(MKBackgroundData::DEFAULT_BACKGROUND_NAME);
    }
    MK_ASSERT(equippedBackground != nullptr);
    MK_ASSERT(equippedBackground->GetTextureFilesCount() >= 4);

    m_Background = MKBackground::create();

    m_Background->addLayer(MKBackgroundLayer::create(equippedBackground->GetTextureFile(0), Vec2(m_Player->GetHorizontalVelocity() * 0.0f, 0.0f)));
    m_Background->addLayer(MKBackgroundLayer::create(equippedBackground->GetTextureFile(1), Vec2(m_Player->GetHorizontalVelocity() * 0.1f, 0.0f)));
    m_Background->addLayer(MKBackgroundLayer::create(equippedBackground->GetTextureFile(2), Vec2(m_Player->GetHorizontalVelocity() * 0.5f, 0.0f)));
    m_Background->addLayer(MKBackgroundLayer::create(equippedBackground->GetTextureFile(3), Vec2(m_Player->GetHorizontalVelocity() * 1.0f, 0.0f)));

    m_Background->runAction(RepeatForever::create(MKFollowNodeAction::Create(1.0f, getDefaultCamera(), MinamiKotori::MKFollowNodeAction::ALL)));
    addChild(m_Background, m_Player->getLocalZOrder() - 1);
}

void Game::InitialiseSpawner()
{
    auto visibleSize = _director->getVisibleSize();

    m_Spawner = MKSpawner::create(this, m_Player, m_Player->getPositionX(), visibleSize.height * 0.15f, visibleSize.height * 0.9f);
    m_Spawner->AddNodeToMoveDuringWaveStart(getDefaultCamera());
    m_Spawner->AddNodeToMoveDuringWaveStart(m_Background);
    addChild(m_Spawner);
}

void Game::CreateScoreUI()
{
    if (m_ScoreLabel) { return; }

    auto visibleSize = _director->getVisibleSize();
    
    mkF32 fontSize = visibleSize.height * 0.05f;
    m_ScoreLabel = MKUIHelper::CreateLabel("SCORE: ", fontSize);
    m_ScoreLabel->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.9f);
    m_ScoreLabel->setTextColor(cocos2d::Color4B::BLACK);
    
    m_UINode->addChild(m_ScoreLabel);
}

void Game::DeleteScoreUI()
{
    if (m_ScoreLabel == nullptr) { return; }

    m_UINode->removeChild(m_ScoreLabel);
    m_ScoreLabel = nullptr;
}

void Game::CreateGameplayUI()
{
    if (m_GameplayUINode) { return; }

    auto visibleSize = _director->getVisibleSize();
    
    m_GameplayUINode = cocos2d::Node::create();
    m_GameplayUINode->retain();
    
    // Pause Button
    {
        mkF32 buttonHeight = visibleSize.height * 0.1f;
        mkF32 buttonPositionX = visibleSize.height * 0.15f;
        mkF32 buttonPositionY = visibleSize.height * 0.85f;
        mkF32 zoomScale = 1.1f;
        mkString buttonNormalTexture = "Textures/UI/Buttons/Pause/Button_Normal.png";
        mkString buttonSelectedTexture = "Textures/UI/Buttons/Pause/Button_Selected.png";
        mkString buttonDisabledTexture = "Textures/UI/Buttons/Pause/Button_Disabled.png";
    
        cocos2d::ui::Button* button = MKUIHelper::CreateButton(
            "",
            0.0f,
            MKUIHelper::DEFAULT_FONT,
            buttonNormalTexture,
            buttonSelectedTexture,
            buttonDisabledTexture,
            zoomScale,
            [=](Ref*) -> void { PauseGame(); }
        );
    
        button->setPosition(Vec2(buttonPositionX, buttonPositionY));
        button->setScale(buttonHeight / button->getContentSize().height);
        m_GameplayUINode->addChild(button);
    }
    
    m_UINode->addChild(m_GameplayUINode);
}

void Game::DeleteGameplayUI()
{
    if (m_GameplayUINode == nullptr) { return; }

    m_UINode->removeChild(m_GameplayUINode);
    m_GameplayUINode = nullptr;
}

void Game::CreatePauseUI()
{
    if (m_PauseUINode) { return; }

    auto visibleSize = _director->getVisibleSize();

    m_PauseUINode = cocos2d::Node::create();
    m_PauseUINode->retain();

    mkF32 zoomScale = 1.1f;

    // Resume Button
    {
        mkF32 buttonPositionX = visibleSize.height * 0.6f;
        mkF32 buttonPositionY = visibleSize.height * 0.5f;
        mkF32 buttonHeight = visibleSize.height * 0.5f;
        mkString buttonNormalTexture = "Textures/UI/Buttons/Play/Button_Normal.png";
        mkString buttonSelectedTexture = "Textures/UI/Buttons/Play/Button_Selected.png";
        mkString buttonDisabledTexture = "Textures/UI/Buttons/Play/Button_Disabled.png";

        cocos2d::ui::Button* button = MKUIHelper::CreateButton(
            "",
            0.0f,
            MKUIHelper::DEFAULT_FONT,
            buttonNormalTexture,
            buttonSelectedTexture,
            buttonDisabledTexture,
            zoomScale,
            [=](Ref*) -> void { ResumeGame(); }
        );

        button->setPosition(Vec2(buttonPositionX, buttonPositionY));
        button->setScale(buttonHeight / button->getContentSize().height);

        m_PauseUINode->addChild(button);
    }

    // Quit Button
    {
        mkF32 buttonPositionX =  visibleSize.width - visibleSize.height * 0.6f;
        mkF32 buttonPositionY = visibleSize.height * 0.5f;
        mkF32 buttonHeight = visibleSize.height * 0.5f;
        mkString buttonNormalTexture = "Textures/UI/Buttons/Quit/Button_Normal.png";
        mkString buttonSelectedTexture = "Textures/UI/Buttons/Quit/Button_Selected.png";
        mkString buttonDisabledTexture = "Textures/UI/Buttons/Quit/Button_Disabled.png";

        cocos2d::ui::Button* button = MKUIHelper::CreateButton(
            "",
            0.0f,
            MKUIHelper::DEFAULT_FONT,
            buttonNormalTexture,
            buttonSelectedTexture,
            buttonDisabledTexture,
            zoomScale,
            [](Ref*) -> void
            {
                MKSceneManager::GetInstance()->ReplaceScene("MainMenu");
            }
        );

        button->setPosition(Vec2(buttonPositionX, buttonPositionY));
        button->setScale(buttonHeight / button->getContentSize().height);

        m_PauseUINode->addChild(button);
    }

    m_UINode->addChild(m_PauseUINode);
}

void Game::DeletePauseUI()
{
    if (m_PauseUINode == nullptr) { return; }

    m_UINode->removeChild(m_PauseUINode);
    m_PauseUINode = nullptr;
}

void Game::CreateGameOverUI()
{
    if (m_GameOverUINode) { return; }

    auto visibleSize = _director->getVisibleSize();

    m_GameOverUINode = cocos2d::Node::create();
    m_GameOverUINode->retain();

    mkF32 fontSize = visibleSize.height * 0.05f;

    // Current Score
    {
        cocos2d::Label* label = MKUIHelper::CreateLabel("SCORE: " + StringUtils::toString(m_Player->GetScore()), fontSize);
        label->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.9f);
        label->setTextColor(cocos2d::Color4B::BLACK);
        m_GameOverUINode->addChild(label);
    }

    // Highscore
    {
        cocos2d::Label* label = MKUIHelper::CreateLabel("", fontSize);

        MKPlayerData* playerData = MKGameDataLoader::GetInstance()->GetGameData<MKPlayerData>();
        if (playerData->GetHighscore() < m_Player->GetScore())
        {
            label->setString("[NEW!] HIGHSCORE: " + StringUtils::toString(m_Player->GetScore()));
            label->setTextColor(cocos2d::Color4B::RED);
        }
        else
        {
            label->setString("HIGHSCORE: " + StringUtils::toString(playerData->GetHighscore()));
            label->setTextColor(cocos2d::Color4B::BLACK);
        }
        
        label->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.8f);
        m_GameOverUINode->addChild(label);
    }

    mkF32 zoomScale = 1.1f;

    // Retry Button
    {
        mkF32 buttonPositionX = visibleSize.height * 0.6f;
        mkF32 buttonPositionY = visibleSize.height * 0.5f;
        mkF32 buttonHeight = visibleSize.height * 0.5f;
        mkString buttonNormalTexture = "Textures/UI/Buttons/Retry/Button_Normal.png";
        mkString buttonSelectedTexture = "Textures/UI/Buttons/Retry/Button_Selected.png";
        mkString buttonDisabledTexture = "Textures/UI/Buttons/Retry/Button_Disabled.png";

        cocos2d::ui::Button* button = MKUIHelper::CreateButton(
            "",
            0.0f,
            MKUIHelper::DEFAULT_FONT,
            buttonNormalTexture,
            buttonSelectedTexture,
            buttonDisabledTexture,
            zoomScale,
            [](Ref*) -> void
            {
                MKSceneManager::GetInstance()->ReplaceScene("Game");
            }
        );

        button->setPosition(Vec2(buttonPositionX, buttonPositionY));
        button->setScale(buttonHeight / button->getContentSize().height);

        m_GameOverUINode->addChild(button);
    }

    // Quit Button
    {
        mkF32 buttonPositionX = visibleSize.width - visibleSize.height * 0.6f;
        mkF32 buttonPositionY = visibleSize.height * 0.5f;
        mkF32 buttonHeight = visibleSize.height * 0.5f;
        mkString buttonNormalTexture = "Textures/UI/Buttons/Quit/Button_Normal.png";
        mkString buttonSelectedTexture = "Textures/UI/Buttons/Quit/Button_Selected.png";
        mkString buttonDisabledTexture = "Textures/UI/Buttons/Quit/Button_Disabled.png";

        cocos2d::ui::Button* button = MKUIHelper::CreateButton(
            "",
            0.0f,
            MKUIHelper::DEFAULT_FONT,
            buttonNormalTexture,
            buttonSelectedTexture,
            buttonDisabledTexture,
            zoomScale,
            [](Ref*) -> void
            {
                MKSceneManager::GetInstance()->ReplaceScene("MainMenu");
            }
        );

        button->setPosition(Vec2(buttonPositionX, buttonPositionY));
        button->setScale(buttonHeight / button->getContentSize().height);

        m_GameOverUINode->addChild(button);
    }

    m_UINode->addChild(m_GameOverUINode);
}

void Game::DeleteGameOverUI()
{
    if (m_GameOverUINode == nullptr) { return; }

    m_UINode->removeChild(m_GameOverUINode);
    m_GameOverUINode = nullptr;
}

// Update UI
void Game::UpdateScoreUI()
{
    if (m_ScoreLabel == nullptr) { return; }

    m_ScoreLabel->setString("Score: " + StringUtils::toString(m_Player->GetScore()));
}

// Callbacks
void Game::OnPlayerDie()
{
    m_Background->pause();
    m_Spawner->pause();

    DeleteScoreUI();
    DeleteGameplayUI();
    CreateGameOverUI();

    // Save Game
    MKPlayerData* playerData = MKGameDataLoader::GetInstance()->GetGameData<MKPlayerData>();
    if (m_Player->GetScore() > playerData->GetHighscore())
    {
        playerData->SetHighscore(m_Player->GetScore());
    }
    playerData->AddCoins(m_Player->GetScore());

    playerData->SaveData(playerData->GetWritablePath());
}

// Others
void Game::PauseGame()
{
    pause();

    for (Vector<Node*>::iterator iter = _children.begin(); iter != _children.end(); ++iter)
    {
        (*iter)->pause();
    }

    DeleteGameplayUI();
    CreatePauseUI();

    PauseBGM();
}

void Game::ResumeGame()
{
    resume();

    for (Vector<Node*>::iterator iter = _children.begin(); iter != _children.end(); ++iter)
    {
        (*iter)->resume();
    }

    DeletePauseUI();
    CreateGameplayUI();

    ResumeBGM();
}

// Input Callbacks
void Game::OnButton(EventCustom * _event)
{
    MKInputButton* input = static_cast<MKInputButton*>(_event->getUserData());
}

void Game::OnClick(EventCustom * _event)
{
    MKInputClick* input = static_cast<MKInputClick*>(_event->getUserData());
}

void Game::OnAxis(EventCustom * _event)
{
    MKInputAxis* input = static_cast<MKInputAxis*>(_event->getUserData());
}

void Game::StartBGM()
{
    if (m_BGMSoundId == MKAudioManager::INVALID_SOUND_ID)
    {
        m_BGMSoundId = MKAudioManager::GetInstance()->PlaySound(m_BGMSoundName, MKSound::SoundType::BGM, 1.0f, true);
    }
}

void Game::PauseBGM()
{
    if (m_BGMSoundId != MKAudioManager::INVALID_SOUND_ID)
    {
        MKAudioManager::GetInstance()->PauseSound(m_BGMSoundId);
    }
}

void Game::ResumeBGM()
{
    if (m_BGMSoundId != MKAudioManager::INVALID_SOUND_ID)
    {
        MKAudioManager::GetInstance()->ResumeSound(m_BGMSoundId);
    }
}

void Game::StopBGM()
{
    if (m_BGMSoundId != MKAudioManager::INVALID_SOUND_ID)
    {
        MKAudioManager::GetInstance()->StopSound(m_BGMSoundId);
        m_BGMSoundId = MKAudioManager::INVALID_SOUND_ID;
    }
}