#include "Common/LBFileUtils.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

USING_NS_CC;

std::string LBFileUtils::getCachePath() {
    JniMethodInfo t;
    std::string ret("");
    if (JniHelper::getStaticMethodInfo(t, "org.cocos2dx.cpp.Cocos2dxActivity", "getCachePath", "()Ljava/lang/String;")) {
        jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        ret = JniHelper::jstring2string(str);
        t.env->DeleteLocalRef(str);
    }
    if (ret.length() > 0) {
        ret.append("/");
    }
    return ret;
}

