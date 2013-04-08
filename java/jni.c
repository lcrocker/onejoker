#include <stdio.h>
#include <jni.h>

#include "onejoker.h"
#include "com_onejoker_onejoker_Context.h"
#include "com_onejoker_onejoker_Sequence.h"
#include "com_onejoker_onejoker_Iterator.h"
#include "com_onejoker_onejoker_Game.h"

JNIEXPORT jint JNICALL Java_com_onejoker_onejoker_Context_seed
(JNIEnv *env, jobject obj, jint s) {
    return ojr_seed(s);
}

JNIEXPORT jint JNICALL Java_com_onejoker_onejoker_Context_rand
(JNIEnv *env, jobject obj, jint limit) {
    return ojr_rand(limit);
}

JNIEXPORT jlong JNICALL Java_com_onejoker_onejoker_Context_binomial
(JNIEnv *env, jobject obj, jint n, jint k) {
    return ojc_binomial(n, k);
}

JNIEXPORT void JNICALL Java_com_onejoker_onejoker_Iterator__1new
(JNIEnv *env, jobject obj, jobject d, jobject h, jintArray buf) {
}

JNIEXPORT void JNICALL Java_com_onejoker_onejoker_Sequence__1new
(JNIEnv *env, jobject obj, jintArray a) {
}
