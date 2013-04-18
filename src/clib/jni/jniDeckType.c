/* OneJoker library <http://lcrocker.github.io/OneJoker>
 */

#include <stdio.h>
#include <jni.h>

#include "onejoker.h"
#include "com_onejoker_onejoker_DeckType.h"

JNIEXPORT jint JNICALL Java_com_onejoker_onejoker_DeckType_nTypes
(JNIEnv *env, jclass cls) {
    return ojd_ntypes();
}

JNIEXPORT jint JNICALL Java_com_onejoker_onejoker_DeckType_nCards
(JNIEnv *env, jclass cls, jint t) {
    return ojd_size(t);
}
