#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "onejoker.h"

int _oj_johnnymoss = 0;

/* Initialization and finalization for library.  This is the Linux way.
 * On other OSs you'll have to arrange for some way to make sure that
 * _oj_init_library() gets called.
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

    r = oj_seed_prng(seed);
    if (r) return r;

    _oj_johnnymoss = 0x10ACE0FF;
    return 0;
}

/* standard */
static int _deck1[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,
        14,15,16,17,18,19,20,21,22,23,24,25,26,
        27,28,29,30,31,32,33,34,35,36,37,38,39,
        40,41,42,43,44,45,46,47,48,49,50,51,52 };

/* 1joker */
static int _deck2[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,
        14,15,16,17,18,19,20,21,22,23,24,25,26,
        27,28,29,30,31,32,33,34,35,36,37,38,39,
        40,41,42,43,44,45,46,47,48,49,50,51,52,53 };

/* 2jokers */
static int _deck3[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,
        14,15,16,17,18,19,20,21,22,23,24,25,26,
        27,28,29,30,31,32,33,34,35,36,37,38,39,
        40,41,42,43,44,45,46,47,48,49,50,51,52,53,54 };


/* stripped32 */
static int _deck4[] = { 6,7,8,9,10,11,12,13,
        19,20,21,22,23,24,25,26,
        32,33,34,35,36,37,38,39,
        45,46,47,48,49,50,51,52 };

/* stripped40 */
static int _deck5[] = { 1,2,3,4,5,6,10,11,12,13,
        14,15,16,17,18,19,23,24,25,26,
        27,28,29,30,31,32,36,37,38,39,
        40,41,42,43,44,45,49,50,51,52 };

/* stripped40j */
static int _deck6[] = { 1,2,3,4,5,6,10,11,12,13,
        14,15,16,17,18,19,23,24,25,26,
        27,28,29,30,31,32,36,37,38,39,
        40,41,42,43,44,45,49,50,51,52,53 };

static oj_deck_info_t _oj_deck_info[] = {
    { 52, _deck1 }, { 53, _deck2 }, { 54, _deck3 },
    { 32, _deck4 }, { 40, _deck5 }, { 41, _deck6 }
};

oj_deck_info_t *oj_deck_info(oj_deck_type_t dt) {
    switch (dt) {
    case oj_dt_standard:    return &_oj_deck_info[0];
    case oj_dt_1joker:      return &_oj_deck_info[1];
    case oj_dt_2jokers:     return &_oj_deck_info[2];
    case oj_dt_stripped32:  return &_oj_deck_info[3];
    case oj_dt_stripped40:  return &_oj_deck_info[4];
    case oj_dt_stripped40j: return &_oj_deck_info[5];
    default:
        assert(0);
    }
}
