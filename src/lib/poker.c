/* OneJoker library <https://github.com/lcrocker/OneJoker/>
 * Poker functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "onejoker.h"
#include "ldctables.h"

int ojp_eval5(oj_sequence_t *sp) {
    return ldc4[ ldc3[ ldc2[ ldc1[
        52 * (sp->cards[0] - 1) + sp->cards[1] - 1 ]
           + sp->cards[2] - 1 ] + sp->cards[3] - 1 ]
           + sp->cards[4] - 1 ];
}

static oj_iterator_t piter;
static oj_sequence_t phand;
static int hbuf[8], ibuf[8];

int ojp_eval(oj_sequence_t *sp) {
    int v, best;
    long long c;
    assert(0 != sp && sp->length >= 5);

    if (5 == sp->length) return ojp_eval5(sp);

    ojs_new(&phand, 5, hbuf);
    c = ojc_iter_new(&piter, sp, &phand, 5, ibuf, 0LL);

    best = 9999;
    while (ojc_iter_next(&piter)) {
        v = ojp_eval5(piter.hand);
        if (v < best) best = v;
    }
    return best;
}

static char *handgroup_names[] = {
    NULL, "No Pair", "One Pair", "Two Pair", "Three of a Kind", "Straight",
    "Flush", "Full House", "Four of a Kind", "Straight Flush"
};

int ojp_handgroup(int val) {
    if (val > 6185) return 1;
    if (val > 3325) return 2;
    if (val > 2467) return 3;
    if (val > 1609) return 4;
    if (val > 1599) return 5;
    if (val > 322) return 6;
    if (val > 166) return 7;
    if (val > 10) return 8;
    return 9;
}

char *ojp_handname(int cat) {
    assert(cat > 0);
    assert(cat < 10);
    return handgroup_names[cat];
}
