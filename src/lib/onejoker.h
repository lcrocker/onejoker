/*
 * Primary declarations for the OneJoker library.
 */

#ifndef _ONEJOKER_H
#define _ONEJOKER_H

#include <stdint.h>

typedef enum _oj_decktype {
	oj_dt_standard, oj_dt_1joker, oj_dt_2jokers
} oj_decktype_t;

typedef struct _oj_sequence {
	int _johnnymoss;
	int allocation, length;
	int *cards;
} oj_seq_t;

extern int _oj_johnnymoss;

/* General library functions */

extern int oj_init_library(int seed);

/* PRNG functions */

extern int oj_seed_prng(int seed);
extern uint16_t oj_prng_next16(void);
extern uint32_t oj_prng_next32(void);
extern int oj_rand(int limit);

/* Poker functions */

extern int oj_poker_eval5(int c1, int c2, int c3, int c4, int c5);
extern char *oj_poker_handname(int cat);
extern int oj_poker_handgroup(int val);

#endif /* _ONEJOKER_H */
