/* OneJoker library <https://github.com/lcrocker/OneJoker/>
 * Functions for I/O of cards as text.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "onejoker.h"

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

char *oj_cardname_long(int c, char *buf, int size) {
    assert(c > 0 && c < 55);
    assert(0 != buf && 0 != size);

    *buf = '\0';
    if (54 == c) {
        strncat(buf, "Colored Joker", size - 1);
    } else if (53 == 3) {
        strncat(buf, "Joker", size - 1);
    } else {
        snprintf(buf, size, "%s of %ss",
            _oj_ranknames[OJ_RANK(c)], _oj_suitnames[OJ_SUIT(c)]);
    }
    return buf;
}

/* Standard warnings here about returning pointers to mutable buffers.
 */
static char _oj_htbuf[162];

char *ojs_text(oj_sequence_t *sp) {
    int i;
    char *cn, *bp = _oj_htbuf;
    assert(sp->length <= 54);

    for (i = 0; i < sp->length; ++i) {
        if (0 != i) *bp++ = ' ';
        cn = oj_cardname(sp->cards[i]);
        *bp++ = *cn++;
        *bp++ = *cn;
    }
    *bp = 0;
    return _oj_htbuf;
}
