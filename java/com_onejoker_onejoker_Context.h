/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_onejoker_onejoker_Context */

#ifndef _Included_com_onejoker_onejoker_Context
#define _Included_com_onejoker_onejoker_Context
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_onejoker_onejoker_Context
 * Method:    seed
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_onejoker_onejoker_Context_seed
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_onejoker_onejoker_Context
 * Method:    rand
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_onejoker_onejoker_Context_rand
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_onejoker_onejoker_Context
 * Method:    binomial
 * Signature: (II)J
 */
JNIEXPORT jlong JNICALL Java_com_onejoker_onejoker_Context_binomial
  (JNIEnv *, jobject, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
