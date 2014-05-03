#include "Common/GamePlatform.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

USING_NS_CC;

void GamePlatform::show() {
    JniMethodInfo jmi;
    if (!JniHelper::getStaticMethodInfo(jmi, "org.cocos2dx.cpp.Cocos2dxActivity", "showGamePlatform", "()V")) {
        return;
    }
    jmi.env->CallStaticVoidMethod(jmi.classID, jmi.methodID);
    jmi.env->DeleteLocalRef(jmi.classID);
}

void GamePlatform::reportScore(int score) {
    JniMethodInfo jmi;
    if (!JniHelper::getStaticMethodInfo(jmi, "org.cocos2dx.cpp.Cocos2dxActivity", "reportScore", "(I)V")) {
        return;
    }
    jmi.env->CallStaticVoidMethod(jmi.classID, jmi.methodID, score);
    jmi.env->DeleteLocalRef(jmi.classID);
}

