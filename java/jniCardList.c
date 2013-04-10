/* OneJoker library <https://github.com/lcrocker/OneJoker>
 *
 * Native interface for CardList class.
 */

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
    jbyte *ptr = (*env)->GetDirectBufferAddress(env, b);
    return ojs_delete((oj_sequence_t *)ptr, i);
}

JNIEXPORT jboolean JNICALL Java_com_onejoker_onejoker_CardList_nEquals
(JNIEnv *env, jclass cls, jobject b, jobject c) {
    jbyte *ptr = (*env)->GetDirectBufferAddress(env, b);
    jbyte *cmp = (*env)->GetDirectBufferAddress(env, c);
    return ojs_equal((oj_sequence_t *)ptr, (oj_sequence_t *)cmp);
}

JNIEXPORT jboolean JNICALL Java_com_onejoker_onejoker_CardList_nExtend
(JNIEnv *env, jclass cls, jobject d, jobject s, jint c) {
    jbyte *dp = (*env)->GetDirectBufferAddress(env, d);
    jbyte *sp = (*env)->GetDirectBufferAddress(env, s);
    return 0 != ojs_extend((oj_sequence_t *)dp, (oj_sequence_t *)sp, c);
}

JNIEXPORT jint JNICALL Java_com_onejoker_onejoker_CardList_nFill
(JNIEnv *env, jclass cls, jobject b, jint c, jint t) {
    jbyte *ptr = (*env)->GetDirectBufferAddress(env, b);
    return ojs_fill((oj_sequence_t *)ptr, c, t);
}

JNIEXPORT jint JNICALL Java_com_onejoker_onejoker_CardList_nFNVHash
(JNIEnv *env, jclass cls, jobject b) {
    jbyte *ptr = (*env)->GetDirectBufferAddress(env, b);
    return ojs_fnv_hash((oj_sequence_t *)ptr);
}

JNIEXPORT jint JNICALL Java_com_onejoker_onejoker_CardList_nIndex
(JNIEnv *env, jclass cls, jobject b, jint c) {
    jbyte *ptr = (*env)->GetDirectBufferAddress(env, b);
    return ojs_index((oj_sequence_t *)ptr, c);
}

JNIEXPORT jboolean JNICALL Java_com_onejoker_onejoker_CardList_nInsert
(JNIEnv *env, jclass cls, jobject b, jint i, jint c) {
    jbyte *ptr = (*env)->GetDirectBufferAddress(env, b);
    return 0 != ojs_insert((oj_sequence_t *)ptr, i, c);
}

JNIEXPORT void JNICALL Java_com_onejoker_onejoker_CardList_nNew
(JNIEnv *env, jclass cls, jobject b, jint s) {
    jbyte *ptr = (*env)->GetDirectBufferAddress(env, b);
    ojs_new((oj_sequence_t *)ptr, s, (int *)(ptr + sizeof(oj_sequence_t)));
}

JNIEXPORT jint JNICALL Java_com_onejoker_onejoker_CardList_nPop
(JNIEnv *env, jclass cls, jobject b) {
    jbyte *ptr = (*env)->GetDirectBufferAddress(env, b);
    return ojs_pop((oj_sequence_t *)ptr);
}

JNIEXPORT jboolean JNICALL Java_com_onejoker_onejoker_CardList_nRemove
(JNIEnv *env, jclass cls, jobject b, jint c) {
    jbyte *ptr = (*env)->GetDirectBufferAddress(env, b);
    return ojs_remove((oj_sequence_t *)ptr, c);
}

JNIEXPORT void JNICALL Java_com_onejoker_onejoker_CardList_nReverse
(JNIEnv *env, jclass cls, jobject b) {
    jbyte *ptr = (*env)->GetDirectBufferAddress(env, b);
    ojs_reverse((oj_sequence_t *)ptr);
}

JNIEXPORT void JNICALL Java_com_onejoker_onejoker_CardList_nShuffle
(JNIEnv *env, jclass cls, jobject b) {
    jbyte *ptr = (*env)->GetDirectBufferAddress(env, b);
    ojs_shuffle((oj_sequence_t *)ptr);
}

JNIEXPORT void JNICALL Java_com_onejoker_onejoker_CardList_nSort
(JNIEnv *env, jclass cls, jobject b) {
    jbyte *ptr = (*env)->GetDirectBufferAddress(env, b);
    ojs_sort((oj_sequence_t *)ptr);
}
