#include "Common/Purchase.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

using namespace std;
USING_NS_CC;

static function<void(string)> purchaseCallback;

extern "C" {
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Cocos2dxActivity_onPurchase(JNIEnv * env, jobject obj, jstring result) {
        purchaseCallback(JniHelper::jstring2string(result));
    }
}

void Purchase::purchase(const char* productID, function<void(string)> callback) {
    purchaseCallback = callback;
    JniMethodInfo jmi;
    if (!JniHelper::getStaticMethodInfo(jmi, "org.cocos2dx.cpp.Cocos2dxActivity", "purchase", "(Ljava/lang/String;)V")) {
        return;
    }
    jstring productIDArg = jmi.env->NewStringUTF(productID);
    jmi.env->CallStaticVoidMethod(jmi.classID, jmi.methodID, productIDArg);
    jmi.env->DeleteLocalRef(productIDArg);
    jmi.env->DeleteLocalRef(jmi.classID);
}

