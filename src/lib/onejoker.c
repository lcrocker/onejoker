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

static int *_oj_deck_fills[] = {
        /* standard */
    {   52, 1,2,3,4,5,6,7,8,9,10,11,12,13,
        14,15,16,17,18,19,20,21,22,23,24,25,26,
        27,28,29,30,31,32,33,34,35,36,37,38,39,
        40,41,42,43,44,45,46,47,48,49,50,51,52 },
        /* 1joker */
    {   53, 1,2,3,4,5,6,7,8,9,10,11,12,13,
        14,15,16,17,18,19,20,21,22,23,24,25,26,
        27,28,29,30,31,32,33,34,35,36,37,38,39,
        40,41,42,43,44,45,46,47,48,49,50,51,52,53 },
        /* 2jokers */
    {   54, 1,2,3,4,5,6,7,8,9,10,11,12,13,
        14,15,16,17,18,19,20,21,22,23,24,25,26,
        27,28,29,30,31,32,33,34,35,36,37,38,39,
        40,41,42,43,44,45,46,47,48,49,50,51,52,53,54 },
        /* stripped32 */
    {   32, 6,7,8,9,10,11,12,13,
        19,20,21,22,23,24,25,26,
        32,33,34,35,36,37,38,39,
        45,46,47,48,49,50,51,52 },
        /* stripped40 */
    {   40, 1,2,3,4,5,6,10,11,12,13,
        14,15,16,17,18,19,23,24,25,26,
        27,28,29,30,31,32,36,37,38,39,
        40,41,42,43,44,45,49,50,51,52 },
        /* stripped40 */
    {   41, 1,2,3,4,5,6,10,11,12,13,
        14,15,16,17,18,19,23,24,25,26,
        27,28,29,30,31,32,36,37,38,39,
        40,41,42,43,44,45,49,50,51,52 }
};

int *oj_deck_info(oj_decktype_t dt) {
    switch (dt) {
    case oj_dt_standard:    return _oj_deck_fills[0];
    case oj_dt_1joker:      return _oj_deck_fills[1];
    case oj_dt_2jokers:     return _oj_deck_fills[2];
    case oj_dt_stripped32:  return _oj_deck_fills[3];
    case oj_dt_stripped40:  return _oj_deck_fills[4];
    case oj_dt_stripped40j: return _oj_deck_fills[5];
    default:
        assert(0);
    }
}
