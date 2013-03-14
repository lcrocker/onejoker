/* Fast 5-card evaluator by "Cactus" Kevin Suffecool and Paul Senzee.
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
    int s, q, *cp = sp->cards;
    assert(5 == sp->length);

    q = (cp[0] | cp[1] | cp[2] | cp[3] | cp[4]) >> 16;
    if (cp[0] & cp[1] & cp[2] & cp[3] & cp[4] & 0xf000) return _ck_flushes[q];
    if ((s = _ck_unique5[q])) return s;
    return _ps_hash_values[_ps_find((cp[0] & 0xff) * (cp[1] & 0xff) *
        (cp[2] & 0xff) * (cp[3] & 0xff) * (cp[4] & 0xff))];
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
