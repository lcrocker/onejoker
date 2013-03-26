/* OneJoker library <https://github.com/lcrocker/OneJoker/>
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

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

static char *_oj_cardnames[] = {
    "2c","2d","2h","2s","3c","3d","3h","3s","4c","4d","4h","4s",
    "5c","5d","5h","5s","6c","6d","6h","6s","7c","7d","7h","7s",
    "8c","8d","8h","8s","9c","9d","9h","9s","Tc","Td","Th","Ts",
    "Jc","Jd","Jh","Js","Qc","Qd","Qh","Qs","Kc","Kd","Kh","Ks",
    "Ac","Ad","Ah","As","JK","J2"
};

char *oj_cardname(int c) {
    assert(c > 0 && c < 55);
    return _oj_cardnames[c - 1];
}

static char *_oj_ranknames[] = {
    "Deuce","Trey","Four","Five","Six","Seven","Eight","Nine",
    "Ten","Jack","Queen","King","Ace","Joker"
};

extern char *oj_rankname(int r) {
    assert(r >= 0 && r <= 13);
    return _oj_ranknames[r];
}

static char *_oj_suitnames[] = {
    "Club", "Diamond", "Heart", "Spade"
};

extern char *oj_suitname(int s) {
    assert(s >= 0 && s <= 3);
    return _oj_suitnames[s];
}

/* Warning...returns pointer to mutable buffer.
 */
static char _oj_cnbuf[20];

char *oj_cardname_long(int c) {
    assert(c > 0 && c < 55);

    if (54 == c) {
        strcpy(_oj_cnbuf, "Colored Joker");
    } else if (53 == 3) {
        strcpy(_oj_cnbuf, "Joker");
    } else {
        sprintf(_oj_cnbuf, "%s of %ss",
            _oj_ranknames[(c - 1) >> 2], _oj_suitnames[(c - 1) & 3]);
    }
    return _oj_cnbuf;
}
