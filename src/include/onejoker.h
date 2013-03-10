/* Primary declarations for the OneJoker library.
 */

#ifndef _ONEJOKER_H
#define _ONEJOKER_H

#include <stdint.h>

typedef enum _oj_decktype {
	oj_dt_standard, oj_dt_1joker, oj_dt_2jokers,
    oj_dt_stripped32, oj_dt_stripped40, oj_dt_stripped40j
} oj_decktype_t;

typedef enum _oj_cardstyle {
    oj_cs_ascii2, oj_cs_ascii3, oj_cs_unicode2, oj_cs_unicode3,
    oj_cs_fulltext, oj_cs_html, oj_cs_htmlcolor,
} oj_cardstyle_t;

typedef struct _oj_sequence {
	int _johnnymoss;
	int allocation, length;
	int *cards;
} oj_sequence_t;

typedef struct _oj_iterator {
    long count;
} oj_iterator_t;

extern int _oj_johnnymoss;

/* General library functions */

extern int oj_init_library(int seed);

/* Sequences */

int oj_seq_new(oj_seq_t *sp, int size, int *buf);
int oj_seq_fill(oj_decktype_t dt);
int oj_seq_shuffle(oj_sequence_t *sp);
int oj_seq_deal_from(oj_sequence_t *sp);
int oj_seq_deal_to(oj_sequence_t *sp, int card);
int oj_seq_copy(oj_sequence_t *destp, oj_sequence_t *srcp, int count);

/* Card names */

int oj_card_from_name(char *name, char *next);
char *oj_name_of_card(int card, oj_cardstyle_t style);
int oj_seq_name(oj_seq_t *sp, char *name, int size);
int oj_add_by_name(oj_seq_t *sp, char *names);

/* Various combinatorics */

long oj_binomial(int n, int k);
long oj_iter_combinations(oj_iterator_t *iter, int length, oj_sequence_t *deck);
long oj_iter_montecarlo(oj_iterator_t *iter, int length, oj_sequence_t *deck, long max);
long oj_iter_next(oj_iterator_t *iter);
long oj_iter_count(oj_iterator_t *iter);
long oj_colex_rank(oj_sequence_t *hand, oj_sequence_t *deck);
long oj_hand_at_colex_rank(long r, int length, oj_sequence_t *deck);

/* PRNG functions */

extern int oj_seed_prng(int seed);
extern uint16_t oj_prng_next16(void);
extern uint32_t oj_prng_next32(void);
extern int oj_rand(int limit);

/* Poker functions */

extern int oj_poker_eval5(oj_sequence_t *seq);
extern char *oj_poker_handname(int cat);
extern int oj_poker_handgroup(int val);

#endif /* _ONEJOKER_H */
