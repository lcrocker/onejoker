/*
 * Fast 5-card evaluator by "Cactus" Kevin Suffecool and Paul Senzee.
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

int _ps_eval5(int c1, int c2, int c3, int c4, int c5) {
    int s, q = (c1 | c2 | c3 | c4 | c5) >> 16;

    if (c1 & c2 & c3 & c4 & c5 & 0xf000) return _ck_flushes[q];
    if ((s = _ck_unique5[q])) return s;
    return _ps_hash_values[_ps_find((c1 & 0xff) * (c2 & 0xff) *
    	(c3 & 0xff) * (c4 & 0xff) * (c5 & 0xff))];
}

int oj_poker_eval5(int c1, int c2, int c3, int c4, int c5) {
    return _ps_eval5( _ck_cardvals[c1], _ck_cardvals[c2],
        _ck_cardvals[c3], _ck_cardvals[c4], _ck_cardvals[c5] );
}

static char *_oj_handgroup_names[] = {
	NULL, "No Pair", "One Pair", "Two Pair", "Three of a Kind",	"Straight",
	"Flush", "Full House", "Four of a Kind", "Straight Flush"
};

char *oj_poker_handname(int cat) {
	assert(cat > 0);
	assert(cat < 10);
	return _oj_handgroup_names[cat];
}

int oj_poker_handgroup(int val) {
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
