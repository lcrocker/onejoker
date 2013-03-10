/*
 * Fast 5-card evaluator by "Cactus" Kevin Suffecool and Paul Senzee.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "onejoker.h"
#include "pokertables.h"

#ifdef _LC_EVAL
#include "lctable"
#endif

#ifndef _LC_EVAL
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
#endif

#ifdef _LC_EVAL
#define MODTABLESIZE(x) ((x)&0x3FFFFF)
#define SWAP(a,b) do{if(a>b){t=a;a=b;b=t;}}while(0)
static uint32_t g_x = 0x76b24ce9, g_y = 0x5e2a572f, g_z = 0x99c52d8c;

int oj_poker_eval5(int c1, int c2, int c3, int c4, int c5) {
	int s, d;
	uint32_t k1, k2;
	unsigned short v, sk;

	SWAP(c1,c2); SWAP(c4,c5); SWAP(c3,c5);
	SWAP(c3,c4); SWAP(c2,c5); SWAP(c1,c4);
	SWAP(c1,c3); SWAP(c2,c4); SWAP(c2,c3);

	k1 = (c1<<24)|(c2<<18)|(c3<<12)|(c4<<6)|c5;
	k2 = k1 * g_x;
	k2 += g_y;
	k2 = MODTABLESIZE(k2>>8);
	v = _oj_lc_table1[k2];
	if (0 == (0x8000 & v)) return v;

	sk = v & 0x7FFF;
	k2 = k1 * (g_y + sk);
	k2 += g_z;
	k2 = MODTABLESIZE(k2>>8);
	return _oj_lc_table1[k2];
}
#endif

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
