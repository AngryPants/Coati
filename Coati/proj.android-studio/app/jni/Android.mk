LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \

$(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \

$(LOCAL_PATH)/../../../Classes/MK/Audio/MKAudioManager.cpp \
$(LOCAL_PATH)/../../../Classes/MK/Audio/MKSound.cpp \

$(LOCAL_PATH)/../../../Classes/MK/Common/MKMathsHelper.cpp \

$(LOCAL_PATH)/../../../Classes/MK/GameData/MKAudioVolumeData.cpp \
$(LOCAL_PATH)/../../../Classes/MK/GameData/MKAudioData.cpp \

$(LOCAL_PATH)/../../../Classes/MK/Input/MKTouchHandler.cpp \
$(LOCAL_PATH)/../../../Classes/MK/Input/MKKeyboardHandler.cpp \
$(LOCAL_PATH)/../../../Classes/MK/Input/MKInputManager.cpp \
$(LOCAL_PATH)/../../../Classes/MK/Input/MKInputDefinition.cpp \

$(LOCAL_PATH)/../../../Classes/MK/SceneManagement/MKSceneManager.cpp \

$(LOCAL_PATH)/../../../Classes/MK/Sprite/MKSprite.cpp \

$(LOCAL_PATH)/../../../Classes/MK/Sprite/SpriteAnimation/MKSpriteAnimationState.cpp \
$(LOCAL_PATH)/../../../Classes/MK/Sprite/SpriteAnimation/MKSpriteAnimation.cpp \

$(LOCAL_PATH)/../../../Classes/MK/UI/MKUIHelper.cpp \

$(LOCAL_PATH)/../../../Classes/Game/Spawner/MKSpawner.cpp \

$(LOCAL_PATH)/../../../Classes/Game/Powerups/MKShield.cpp \

$(LOCAL_PATH)/../../../Classes/Game/Player/MKPlayer.cpp \

$(LOCAL_PATH)/../../../Classes/Game/Obstacles/MKSpike.cpp \
$(LOCAL_PATH)/../../../Classes/Game/Obstacles/MKSaw.cpp \
$(LOCAL_PATH)/../../../Classes/Game/Obstacles/MKMissileWarning.cpp \
$(LOCAL_PATH)/../../../Classes/Game/Obstacles/MKMissle.cpp \
$(LOCAL_PATH)/../../../Classes/Game/Obstacles/MKLaser.cpp \

$(LOCAL_PATH)/../../../Classes/Game/GameData/MKPlayerData.cpp \
$(LOCAL_PATH)/../../../Classes/Game/GameData/MKHeroData.cpp \
$(LOCAL_PATH)/../../../Classes/Game/GameData/MKBackgroundData.cpp \

$(LOCAL_PATH)/../../../Classes/Game/Background/MKBackgroundLayer.cpp \
$(LOCAL_PATH)/../../../Classes/Game/Background/MKBackground.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
