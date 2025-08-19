#include "JNIHelp.h"

int jniRegisterNativeMethods(JNIEnv* env,
                             const char* className,
                             const JNINativeMethod* gMethods,
                             int numMethods)
{
    jclass c = env->FindClass(className);

    int result = env->RegisterNatives(c, gMethods, numMethods);

    if (result < 0) {
        env->FatalError("Registering native methods failed, aborting...");
    }

    env->DeleteLocalRef(c);

    return 0;
}
