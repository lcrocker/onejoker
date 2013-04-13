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

/* Put a string representation of the card sequence into the given buffer,
 * clipping if necessary. Return the buffer, or NULL if it was too small for
 * anything useful.
 */
char *ojs_text(oj_sequence_t *sp, char *buf, int bsize) {
    int i, sl = sp->length, last = 0, clipat = sl + 1;
    char *cn, *bp = buf;
    assert(0 != sl && 0 != bsize);

    if ((sl <= 2 && bsize < (3 * sl + 2)) ||
        (sl > 2 && bsize < 10)) return NULL;
    if (bsize < (3 * sl + 2)) clipat = (bsize - 7) / 3;

    last = 0;
    for (i = 0; i < sl; ++i) {
        last = (i == (sl - 1));
        if ((!last) && (i >= clipat)) continue;

        if (0 == i) {
            *bp++ = '(';
        } else if (last && i >= clipat) {
            *bp++ = '.'; *bp++ = '.'; *bp++ = '.';
        } else {
            *bp++ = ' ';
        }
        cn = oj_cardname(sp->cards[i]);
        *bp++ = *cn++;
        *bp++ = *cn;
    }
    *bp++ = ')';
    *bp = 0;
    assert((bp - buf) < bsize);
    return buf;
}
