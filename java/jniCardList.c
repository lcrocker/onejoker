#include <stdio.h>
#include <stddef.h>
#include <jni.h>

#include "onejoker.h"
#include "com_onejoker_onejoker_CardList.h"

JNIEXPORT jint JNICALL Java_com_onejoker_onejoker_CardList_nStructSize
(JNIEnv *env, jclass cls) { return sizeof(oj_sequence_t); }

JNIEXPORT jint JNICALL Java_com_onejoker_onejoker_CardList_nLengthOff
(JNIEnv *env, jclass cls) { return offsetof(oj_sequence_t, length); }

JNIEXPORT jboolean JNICALL Java_com_onejoker_onejoker_CardList_nAppend
(JNIEnv *env, jclass cls, jobject b, jint c) {
    jbyte *ptr = (*env)->GetDirectBufferAddress(env, b);
    return 0 != ojs_append((oj_sequence_t *)ptr, c);
}

JNIEXPORT jint JNICALL Java_com_onejoker_onejoker_CardList_nDelete
(JNIEnv *env, jclass cls, jobject b, jint i) {
}

JNIEXPORT jboolean JNICALL Java_com_onejoker_onejoker_CardList_nEquals
(JNIEnv *env, jclass cls, jobject b, jobject c) {
}

JNIEXPORT jboolean JNICALL Java_com_onejoker_onejoker_CardList_nExtend
(JNIEnv *env, jclass cls, jobject d, jobject s, jint c) {
}

JNIEXPORT jint JNICALL Java_com_onejoker_onejoker_CardList_nFill
(JNIEnv *env, jclass cls, jobject b, jint c, jint t) {
}

JNIEXPORT jint JNICALL Java_com_onejoker_onejoker_CardList_nFNVHash
(JNIEnv *env, jclass cls, jobject b) {
}

JNIEXPORT jint JNICALL Java_com_onejoker_onejoker_CardList_nIndex
(JNIEnv *env, jclass cls, jobject b, jint c) {
}

JNIEXPORT void JNICALL Java_com_onejoker_onejoker_CardList_nNew
(JNIEnv *env, jclass cls, jobject b, jint s) {
    jbyte *ptr = (*env)->GetDirectBufferAddress(env, b);
    ojs_new((oj_sequence_t *)ptr, s, (int *)(ptr + sizeof(oj_sequence_t)));
}

JNIEXPORT jint JNICALL Java_com_onejoker_onejoker_CardList_nPop
(JNIEnv *env, jclass cls, jobject b) {
}

JNIEXPORT jboolean JNICALL Java_com_onejoker_onejoker_CardList_nRemove
(JNIEnv *env, jclass cls, jobject b, jint c) {
}

JNIEXPORT void JNICALL Java_com_onejoker_onejoker_CardList_nReverse
(JNIEnv *env, jclass cls, jobject b) {
}

JNIEXPORT void JNICALL Java_com_onejoker_onejoker_CardList_nShuffle
(JNIEnv *env, jclass cls, jobject b) {
}

JNIEXPORT void JNICALL Java_com_onejoker_onejoker_CardList_nSort
(JNIEnv *env, jclass cls, jobject b) {
    jbyte *ptr = (*env)->GetDirectBufferAddress(env, b);
    ojs_sort((oj_sequence_t *)ptr);
}
