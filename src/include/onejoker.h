/* Primary declarations for the OneJoker library.
 */

#ifndef _ONEJOKER_H
#define _ONEJOKER_H

#include <stdint.h>

typedef enum _oj_decktype {
	oj_dt_standard, oj_dt_1joker, oj_dt_2jokers,
    oj_dt_stripped32, oj_dt_stripped40, oj_dt_stripped40j,
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
extern int *oj_deck_info(oj_decktype_t dt);

/* PRNG functions */

extern int oj_seed_prng(int seed);
extern uint16_t oj_prng_next16(void);
extern uint32_t oj_prng_next32(void);
extern int oj_rand(int limit);

/* Card names */

extern int oj_card_from_name(char *name, char **next);
extern char *oj_name_of_card(int card, oj_cardstyle_t style);
extern int oj_seq_name(oj_sequence_t *sp, char *name, int size, char sep);
extern int oj_add_by_name(oj_sequence_t *sp, char *names);

/* Sequences */

extern int oj_seq_new(oj_sequence_t *sp, int size, int *buf);
extern int oj_seq_deal_from_end(oj_sequence_t *sp);
extern int oj_seq_deal_to_end(oj_sequence_t *sp, int card);
extern int oj_seq_deal_from_head(oj_sequence_t *sp);
extern int oj_seq_deal_to_head(oj_sequence_t *sp, int card);
extern int oj_pick(oj_sequence_t *sp, int card);
extern int oj_seq_move(oj_sequence_t *destp, oj_sequence_t *srcp, int count);
extern int oj_seq_copy(oj_sequence_t *destp, oj_sequence_t *srcp);
extern int oj_seq_fill(oj_sequence_t *sp, int count, oj_decktype_t dt);
extern int oj_seq_shuffle(oj_sequence_t *sp);

/* Various combinatorics */

long oj_binomial(int n, int k);
long oj_iter_combinations(oj_iterator_t *iter, int length, oj_sequence_t *deck);
long oj_iter_montecarlo(oj_iterator_t *iter, int length, oj_sequence_t *deck, long max);
long oj_iter_next(oj_iterator_t *iter);
long oj_iter_count(oj_iterator_t *iter);
long oj_colex_rank(oj_sequence_t *hand, oj_sequence_t *deck);
long oj_hand_at_colex_rank(long r, int length, oj_sequence_t *deck);

/* Poker functions */

extern int oj_poker_eval5(oj_sequence_t *seq);
extern char *oj_poker_handname(int cat);
extern int oj_poker_handgroup(int val);

#endif /* _ONEJOKER_H */
