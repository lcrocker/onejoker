/* OneJoker library <https://github.com/lcrocker/OneJoker>
 *
 * Native interface for Card class.
 */

#include <stdio.h>
#include <jni.h>

#include "onejoker.h"
#include "com_onejoker_onejoker_Card.h"

JNIEXPORT jint JNICALL Java_com_onejoker_onejoker_Card_seed
(JNIEnv *env, jclass cls, jint s) {
    return ojr_seed(s);
}

JNIEXPORT jint JNICALL Java_com_onejoker_onejoker_Card_rand
(JNIEnv *env, jclass cls, jint lim) {
    return ojr_rand(lim);
}

