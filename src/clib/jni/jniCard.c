/* OneJoker library <http://lcrocker.github.io/OneJoker>
 *
 * Native interface for Card class.
 */

#include <stdio.h>
#include <jni.h>

#include "onejoker.h"
#include "com_onejoker_onejoker_Card.h"

JNIEXPORT void JNICALL Java_com_onejoker_onejoker_Card_seed
(JNIEnv *env, jclass cls, jint seed) {
	ojr_seed(seed);
}

JNIEXPORT jint JNICALL Java_com_onejoker_onejoker_Card_rand
(JNIEnv *env, jclass cls, jint limit) {
	return ojr_rand(limit);
}
