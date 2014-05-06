LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/Common/RandomImpl.cpp \
                   ../../Classes/Scene/Title/TitleScene.cpp \
                   ../../Classes/Scene/Game/GameScene.cpp \
                   ../../Classes/Scene/Game/Mana.cpp \
                   ../../Classes/Scene/Game/Burger.cpp \
                   ../../Classes/Scene/Result/ResultScene.cpp \
                   hellocpp/Purchase.cpp \
                   hellocpp/Store.cpp \
                   hellocpp/GamePlatform.cpp \
                   hellocpp/LBSocial.cpp \
                   hellocpp/LBAnalytics.cpp \
                   hellocpp/LBFileUtils.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/Common
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/Scene

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../cocos2d/cocos/ui
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../cocos2d/cocos/editor-support

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_ui_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,2d)
$(call import-module,audio/android)
$(call import-module,Box2D)
$(call import-module,editor-support/cocosbuilder)
$(call import-module,editor-support/cocostudio)
$(call import-module,editor-support/spine)
$(call import-module,ui)
