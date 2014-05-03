#include "Common/LBAnalytics.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

using namespace std;
USING_NS_CC;

jobject createJavaMapObject(JNIEnv* env, map<string, string>* paramMap) {
    jclass class_Hashtable = env->FindClass("java/util/Hashtable");
    jmethodID construct_method = env->GetMethodID( class_Hashtable, "<init>","()V");
    jobject obj_Map = env->NewObject( class_Hashtable, construct_method, "");
    if (paramMap != NULL) {
        jmethodID add_method= env->GetMethodID( class_Hashtable,"put","(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
        for (auto it = paramMap->begin(); it != paramMap->end(); ++it) {
            env->CallObjectMethod(obj_Map, add_method, env->NewStringUTF(it->first.c_str()), env->NewStringUTF(it->second.c_str()));
        }
    }
    env->DeleteLocalRef(class_Hashtable);
    return obj_Map;
}

void logEvent_(const char* method, const char* event, map<string, string> param) {
    JniMethodInfo jmi;
    if (!JniHelper::getStaticMethodInfo(jmi, "org.cocos2dx.cpp.Cocos2dxActivity", method, "(Ljava/lang/String;Ljava/util/Hashtable;)V")) {
        return;
    }
    jstring jeventId = jmi.env->NewStringUTF(event);
    jobject obj_Map = createJavaMapObject(jmi.env, &param);
    jmi.env->CallStaticVoidMethod(jmi.classID, jmi.methodID, jeventId, obj_Map);
    jmi.env->DeleteLocalRef(jeventId);
    jmi.env->DeleteLocalRef(obj_Map);
    jmi.env->DeleteLocalRef(jmi.classID);
}

void LBAnalytics::logEvent(const char* event, map<string, string> param) {
    logEvent_("logEvent", event, param);
}

void LBAnalytics::startEvent(const char* event, map<string, string> param) {
    logEvent_("startEvent", event, param);
}

void LBAnalytics::endEvent(const char* event, map<string, string> param) {
    logEvent_("endEvent", event, param);
}

