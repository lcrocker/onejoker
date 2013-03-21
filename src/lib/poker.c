/* OneJoker library <https://github.com/lcrocker/OneJoker/>
 * 5-card evaluator by "Cactus" Kevin Suffecool and Paul Senzee.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "onejoker.h"
#include "pokertables.h"

static unsigned _ps_find(unsigned u) {
    unsigned a, b, r;

    u += 0xe91aaa35;
    u ^= u >> 16;
    u += u << 8;
    u ^= u >> 4;
    b  = (u >> 8) & 0x1ff;
    a  = (u + (u << 2)) >> 19;
    r  = a ^ _ps_hash_adjust[b];
    return r;
}

int ojp_eval5(oj_sequence_t *sp) {
    int s, q, c0, c1, c2, c3, c4;
    assert(0 != sp && 5 == sp->length);

    c0 = _ck_cardvals[sp->cards[0]];
    c1 = _ck_cardvals[sp->cards[1]];
    c2 = _ck_cardvals[sp->cards[2]];
    c3 = _ck_cardvals[sp->cards[3]];
    c4 = _ck_cardvals[sp->cards[4]];

    q = (c0 | c1 | c2 | c3 | c4) >> 16;
    if (c0 & c1 & c2 & c3 & c4 & 0xf000) return _ck_flushes[q];
    if ((s = _ck_unique5[q])) return s;
    return _ps_hash_values[_ps_find((c0 & 0xff) * (c1 & 0xff) *
        (c2 & 0xff) * (c3 & 0xff) * (c4 & 0xff))];
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
