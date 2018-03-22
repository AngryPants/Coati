// Include MK
#include "MK/SceneManagement/MKSceneManager.h"
#include "MK/UI/MKUIHelper.h"

// Include STL
#include <cmath>

// Include Header
#include "Settings.h"

bool Settings::init()
{
	if (!Super::init()) { return false; }

    InitialiseBackground();
    InitialiseBanner();
    InitialiseBackButton();
    InitialiseTitle("Settings");
    InitialiseVolumeControls();

    return true;
}

void Settings::onEnter()
{
    Super::onEnter();

    // Play Sounds
    // m_BGMSoundId = MKAudioManager::GetInstance()->PlaySound(m_BGMSoundName, MKSound::SoundType::BGM, 1.0f, true, [](mkS32 _soundId, const mkString& _fileLocation) -> void { CCLOG("Setttings BGM Finish Playing Callback"); });
    // m_SFXSoundId = MKAudioManager::GetInstance()->PlaySound(m_SFXSoundName, MKSound::SoundType::SFX, 1.0f, true, [](mkS32 _soundId, const mkString& _fileLocation) -> void { CCLOG("Setttings SFX Finish Playing Callback"); });
}

void Settings::onExit()
{
    Super::onExit();

    // Stop Sounds
    // MKAudioManager::GetInstance()->StopSound(m_BGMSoundId);
    // m_BGMSoundId = MKAudioManager::INVALID_SOUND_ID;
    // MKAudioManager::GetInstance()->StopSound(m_SFXSoundId);
    // m_SFXSoundId = MKAudioManager::INVALID_SOUND_ID;
    MKAudioManager::GetInstance()->SaveVolumes();
}

void Settings::InitialiseVolumeControls()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    mkS32 sliderCount = 0;
    mkF32 sliderSpacing = visibleSize.height * 0.18f;

    // Label
    mkF32 fontSize = visibleSize.height * 0.05f;
    cocos2d::Color4B fontColor = cocos2d::Color4B::BLACK;
    cocos2d::Vec2 labelPosition(visibleSize.width * 0.5f, visibleSize.height * 0.65f);

    // Slider
    mkString barTexture = "Textures/UI/Slider/Menu/Slider_Bar_Normal.png";
    mkString progressBarTexture = "Textures/UI/Slider/Menu/Slider_Bar_Progress.png";
    mkString ballNormalTexture = "Textures/UI/Slider/Menu/Slider_Ball_Normal.png";
    mkString ballSelectedTexture = "Textures/UI/Slider/Menu/Slider_Ball_Selected.png";
    mkString ballDisabledTexture = "Textures/UI/Slider/Menu/Slider_Ball_Disabled.png";
    mkF32 sliderHeight = visibleSize.height * 0.05f;
    mkF32 sliderWidth = sliderHeight * 10.0f;
    mkF32 zoomScale = 1.1f;
    cocos2d::Vec2 sliderPosition(visibleSize.width * 0.5f, visibleSize.height * 0.55f);

    // Master Volume Control
    {
        // Offset
        cocos2d::Vec2 positionOffset(0.0f, static_cast<mkF32>(sliderCount++) * -sliderSpacing);

        // Label
        auto label = MKUIHelper::CreateLabel("Master Volume", fontSize);
        label->setPosition(labelPosition + positionOffset);
        label->setTextColor(fontColor);

        m_UINode->addChild(label);

        // Slider
        auto slider = MKUIHelper::CreateSlider(
            barTexture,
            progressBarTexture,
            ballNormalTexture,
            ballSelectedTexture,
            ballDisabledTexture,
            zoomScale,
            [](Ref* _sender, ui::Slider::EventType _type) -> void
        {
            ui::Slider* slider = dynamic_cast<ui::Slider*>(_sender);
            if (_type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
            {
                // Update the master volume.
                MKAudioManager::GetInstance()->SetMasterVolume((mkF32)slider->getPercent() / 100.0f);
            }
        }
        );

        mkF32 sliderPercentage = MKAudioManager::GetInstance()->GetMasterVolume() * 100.0f;
        slider->setPercent((mkS32)sliderPercentage);
        slider->setPosition(sliderPosition + positionOffset);
        slider->setScaleX(sliderWidth / slider->getVirtualRendererSize().width);
        slider->setScaleY(sliderHeight / slider->getVirtualRendererSize().height);

        m_UINode->addChild(slider);
    }

    // BGM Volume Control
    {
        // Offset
        cocos2d::Vec2 positionOffset(0.0f, static_cast<mkF32>(sliderCount++) * -sliderSpacing);

        // Label
        auto label = MKUIHelper::CreateLabel("BGM Volume", fontSize);
        label->setPosition(labelPosition + positionOffset);
        label->setTextColor(fontColor);

        m_UINode->addChild(label);

        // Slider
        auto slider = MKUIHelper::CreateSlider(
            barTexture,
            progressBarTexture,
            ballNormalTexture,
            ballSelectedTexture,
            ballDisabledTexture,
            zoomScale,
            [](Ref* _sender, ui::Slider::EventType _type) -> void
        {
            ui::Slider* slider = dynamic_cast<ui::Slider*>(_sender);
            if (_type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
            {
                // Update the master volume.
                MKAudioManager::GetInstance()->SetVolume(MKSound::SoundType::BGM, (mkF32)slider->getPercent() / 100.0f);
            }
        }
        );

        mkF32 sliderPercentage = MKAudioManager::GetInstance()->GetVolume(MKSound::SoundType::BGM) * 100.0f;
        slider->setPercent((mkS32)sliderPercentage);
        slider->setPosition(sliderPosition + positionOffset);
        slider->setScaleX(sliderWidth / slider->getVirtualRendererSize().width);
        slider->setScaleY(sliderHeight / slider->getVirtualRendererSize().height);

        m_UINode->addChild(slider);
    }

    /*
    // Ambient Volume Control
    {
        // Offset
        cocos2d::Vec2 positionOffset(0.0f, static_cast<mkF32>(sliderCount++) * -sliderSpacing);

        // Label
        auto label = MKUIHelper::CreateLabel("Ambient Volume", fontSize);
        label->setPosition(labelPosition + positionOffset);
        label->setTextColor(fontColor);

        m_UINode->addChild(label);

        // Slider
        auto slider = MKUIHelper::CreateSlider(
            barTexture,
            progressBarTexture,
            ballNormalTexture,
            ballSelectedTexture,
            ballDisabledTexture,
            zoomScale,
            [](Ref* _sender, ui::Slider::EventType _type) -> void
        {
            ui::Slider* slider = dynamic_cast<ui::Slider*>(_sender);
            if (_type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
            {
                // Update the Ambient volume.
                MKAudioManager::GetInstance()->SetVolume(MKSound::SoundType::AMBIENT, (mkF32)slider->getPercent() / 100.0f);
            }
        }
        );

        mkF32 sliderPercentage = MKAudioManager::GetInstance()->GetVolume(MKSound::SoundType::AMBIENT) * 100.0f;
        slider->setPercent((mkS32)sliderPercentage);
        slider->setPosition(sliderPosition + positionOffset);
        slider->setScaleX(sliderWidth / slider->getVirtualRendererSize().width);
        slider->setScaleY(sliderHeight / slider->getVirtualRendererSize().height);

        m_UINode->addChild(slider);
    }
    */

    // SFX Volume Control
    {
        // Offset
        cocos2d::Vec2 positionOffset(0.0f, static_cast<mkF32>(sliderCount++) * -sliderSpacing);

        // Label
        auto label = MKUIHelper::CreateLabel("SFX Volume", fontSize);
        label->setPosition(labelPosition + positionOffset);
        label->setTextColor(fontColor);

        m_UINode->addChild(label);

        // Slider
        auto slider = MKUIHelper::CreateSlider(
            barTexture,
            progressBarTexture,
            ballNormalTexture,
            ballSelectedTexture,
            ballDisabledTexture,
            zoomScale,
            [](Ref* _sender, ui::Slider::EventType _type) -> void
        {
            ui::Slider* slider = dynamic_cast<ui::Slider*>(_sender);
            if (_type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
            {
                // Update the SFX volume.
                MKAudioManager::GetInstance()->SetVolume(MKSound::SoundType::SFX, (mkF32)slider->getPercent() / 100.0f);
            }
        }
        );

        mkF32 sliderPercentage = MKAudioManager::GetInstance()->GetVolume(MKSound::SoundType::SFX) * 100.0f;
        slider->setPercent((mkS32)sliderPercentage);
        slider->setPosition(sliderPosition + positionOffset);
        slider->setScaleX(sliderWidth / slider->getVirtualRendererSize().width);
        slider->setScaleY(sliderHeight / slider->getVirtualRendererSize().height);

        m_UINode->addChild(slider);
    }
}