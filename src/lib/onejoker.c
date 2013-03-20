/* OneJoker library <https://github.com/lcrocker/OneJoker/>
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "onejoker.h"

int _oj_johnnymoss = 0;

/* Initialization and finalization for library. This is the Linux way.
 * On other OSs you'll have to arrange for some way to make sure that
 * oj_init_library() gets called on load.
 */
void _init(void) {
    int r;

    r = oj_init_library(0);
    if (r) {
        exit(EXIT_FAILURE);
    }
    return;
}

void _fini(void) {
    return;
}

int oj_init_library(int seed) {
    int r;

    r = ojr_seed(seed);
    if (r) return r;

    _oj_johnnymoss = 0x10ACE0FF;
    return 0;
}
