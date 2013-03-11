#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "onejoker.h"

int _oj_johnnymoss = 0;

/* Initialization and finalization for library.  This is the Linux way.
 * On other OSs you'll have to arrange for some way to make sure that
 * oj_init_library() gets called.
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
static int deck1[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,
        14,15,16,17,18,19,20,21,22,23,24,25,26,
        27,28,29,30,31,32,33,34,35,36,37,38,39,
        40,41,42,43,44,45,46,47,48,49,50,51,52 };

/* 1joker */
static int deck2[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,
        14,15,16,17,18,19,20,21,22,23,24,25,26,
        27,28,29,30,31,32,33,34,35,36,37,38,39,
        40,41,42,43,44,45,46,47,48,49,50,51,52,53 };

/* 2jokers */
static int deck3[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,
        14,15,16,17,18,19,20,21,22,23,24,25,26,
        27,28,29,30,31,32,33,34,35,36,37,38,39,
        40,41,42,43,44,45,46,47,48,49,50,51,52,53,54 };


/* stripped32 */
static int deck4[] = { 6,7,8,9,10,11,12,13,
        19,20,21,22,23,24,25,26,
        32,33,34,35,36,37,38,39,
        45,46,47,48,49,50,51,52 };

/* stripped40 */
static int deck5[] = { 1,2,3,4,5,6,10,11,12,13,
        14,15,16,17,18,19,23,24,25,26,
        27,28,29,30,31,32,36,37,38,39,
        40,41,42,43,44,45,49,50,51,52 };

/* stripped40j */
static int deck6[] = { 1,2,3,4,5,6,10,11,12,13,
        14,15,16,17,18,19,23,24,25,26,
        27,28,29,30,31,32,36,37,38,39,
        40,41,42,43,44,45,49,50,51,52,53 };

static oj_deck_info_t deck_info[] = {
    { 52, deck1 }, { 53, deck2 }, { 54, deck3 },
    { 32, deck4 }, { 40, deck5 }, { 41, deck6 }
};

oj_deck_info_t *oj_deck_info(oj_deck_type_t dt) {
    switch (dt) {
    case oj_dt_standard:    return &deck_info[0];
    case oj_dt_1joker:      return &deck_info[1];
    case oj_dt_2jokers:     return &deck_info[2];
    case oj_dt_stripped32:  return &deck_info[3];
    case oj_dt_stripped40:  return &deck_info[4];
    case oj_dt_stripped40j: return &deck_info[5];
    default:
        assert(0);
    }
}
