#include "Common/Store.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

USING_NS_CC;

void Store::openReviewPage() {
    JniMethodInfo jmi;
    if (!JniHelper::getStaticMethodInfo(jmi, "org.cocos2dx.cpp.Cocos2dxActivity", "openReviewPage", "()V")) {
        return;
    }
    jmi.env->CallStaticVoidMethod(jmi.classID, jmi.methodID);
    jmi.env->DeleteLocalRef(jmi.classID);
}

