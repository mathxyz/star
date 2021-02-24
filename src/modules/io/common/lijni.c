#include "liCommonInner.h"


#ifdef LI_ANDROID

JNIEXPORT jobjectArray JNICALL
Java_lili_jni_NativeFunctions_getFileData(JNIEnv *env, jclass kclass) {

    const char *data[] = {
            "acccc", "bsss", "函数库", "对齐", "不对齐", "转方向", "从文件夹查看", "全屏"};

    int count = sizeof(data) / sizeof(*data);
    jclass stringClass = (*env)->FindClass(env, "java/lang/String");
    jobjectArray array = (*env)->NewObjectArray(env, count, stringClass, 0);
    jsize i;
    for (i = 0; i < count; i++) {
        (*env)->SetObjectArrayElement(env, array, i, (*env)->NewStringUTF(env, data[i]));

    }
    return array;
}


jobjectArray  JNICALL Java_lili_jni_NativeFunctions_ls(JNIEnv *env, jobject obj, jstring cmdIn) {
//	livlog("jniSvmTrain li1 = %d", li1());
    const char *cmd;
    cmd = (*env)->GetStringUTFChars(env, cmdIn, 0);
    livlog("search cmd = %s", cmd);

    int ssl = 0;
    char **ss = 0;
    ss = licmdToArgvN(cmd, &ssl);
    if (ss == 0)
        goto LiXOXO;
////////////////
    int re = 1;
    char **pp = 0;
    int use = 0;
    re = lisearchN(ssl, ss, (void **) &pp, &use);
////////////////////


    int count = use;
    jclass stringClass = (*env)->FindClass(env, "java/lang/String");
    jobjectArray array = (*env)->NewObjectArray(env, count, stringClass, 0);
    jsize i;
    for (i = 0; i < count; i++) {
        (*env)->SetObjectArrayElement(env, array, i, (*env)->NewStringUTF(env, pp[i]));
    }

////////////////////
//    liprintfss(pp, use);
    lifreess3((void **) &pp, use);
    LiXOXO:
    lifreess(ss, ssl);

    // free java object memory
    (*env)->ReleaseStringUTFChars(env, cmdIn, cmd);
    return array;
}

JNIEXPORT jobjectArray JNICALL
Java_lili_jni_NativeFunctions_ls2(JNIEnv *env, jobject this, jstring cmdIn) {

    const char *cmd;
    cmd = (*env)->GetStringUTFChars(env, cmdIn, 0);

    char **pp = 0;
    int use = 0;

    lisearchImagesN((void **) &pp, cmd, &use);
//	liprintfss(pp, use);



    int count = use;
    jclass stringClass = (*env)->FindClass(env, "java/lang/String");
    jobjectArray array = (*env)->NewObjectArray(env, count, stringClass, 0);
    jsize i;
    for (i = 0; i < count; i++) {
        (*env)->SetObjectArrayElement(env, array, i, (*env)->NewStringUTF(env, pp[i]));
    }
    lifreess3((void **) &pp, use);


    (*env)->ReleaseStringUTFChars(env, cmdIn, cmd);
    return array;
}

jobjectArray jniJiShiBenRead(JNIEnv *env, jobject obj, jstring cmdIn) {
    const char *cmd;
    cmd = (*env)->GetStringUTFChars(env, cmdIn, 0);


    int argc = 0;
//    char* cmd2 ="lily -name ho.txt -max 3 -start 0";
    char **argv = lisplitby2N(cmd, " ", 0, -1, &argc, 0);
//	liprintfss(argv,argc);
//    re = main(argc, argv);
    StringArr sa;
    lireadLinesMainN(argc, argv, &sa);
    lifreess(argv, argc);

//    liprintfss(sa.pp, sa.use);


    int count = sa.use;
    jclass stringClass = (*env)->FindClass(env, "java/lang/String");
    jobjectArray array = (*env)->NewObjectArray(env, count, stringClass, 0);
    jsize i;
    for (i = 0; i < count; i++) {
        (*env)->SetObjectArrayElement(env, array, i, (*env)->NewStringUTF(env, sa.pp[i]));
    }


    lifreess3((void **) &sa.pp, sa.use);

    (*env)->ReleaseStringUTFChars(env, cmdIn, cmd);
    return array;
}

jint jniJiShiBenWrite(JNIEnv *env, jobject this, jstring cmdIn) {
    int re, argc = 0;
    const char *cmd;
    cmd = (*env)->GetStringUTFChars(env, cmdIn, 0);
    liJiShiBenWrite("2017.txt", cmd);
    (*env)->ReleaseStringUTFChars(env, cmdIn, cmd);
    return re;
}


static JNINativeMethod gMethodRegistry[] = {
        {"jniJiShiBenWrite",
                "(Ljava/lang/String;)I",                   (void *) jniJiShiBenWrite},
        {"jniJiShiBenRead",
                "(Ljava/lang/String;)[Ljava/lang/String;", (void *) jniJiShiBenRead}
};

int liRegisterNatives(JNIEnv *env) {
    jclass cls = (*env)->FindClass(env, "lili/jni/NativeFunctions");
    if (!cls) {
        lilog("cls not find");
        return 0;
    }
    (*env)->RegisterNatives(env, cls, gMethodRegistry, sizeof(gMethodRegistry)
                                                       / sizeof(gMethodRegistry[0]));
    return 1;
}


#endif// LI_ANDROID