/* Primary declarations for the OneJoker library.
 */

#ifndef _ONEJOKER_H
#define _ONEJOKER_H

#include <stdint.h>

typedef enum _oj_deck_type {
    oj_dt_standard, oj_dt_1joker, oj_dt_2jokers,
    oj_dt_stripped32, oj_dt_stripped40, oj_dt_stripped40j,
} oj_deck_type_t;

typedef enum _oj_card_style {
    oj_cs_ascii2, oj_cs_ascii3, oj_cs_unicode2, oj_cs_unicode3,
    oj_cs_fulltext, oj_cs_html, oj_cs_htmlcolor,
} oj_card_style_t;

typedef struct _oj_deck_info {
    int size;
    int *cards;
} oj_deck_info_t;

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
extern oj_deck_info_t *oj_deck_info(oj_deck_type_t dt);

/* PRNG functions */

extern int oj_seed_prng(int seed);
extern uint16_t oj_prng_next16(void);
extern uint32_t oj_prng_next32(void);
extern int oj_rand(int limit);

/* Card names */

extern int oj_card_from_name(char *name, char **next);
extern char *oj_name_of_card(int card, oj_card_style_t style);
extern int oj_seq_name(oj_sequence_t *sp, char *name, int size, char sep);
extern int oj_add_by_name(oj_sequence_t *sp, char *names);

/* Sequences */

extern int oj_seq_new(oj_sequence_t *sp, int size, int *buf);
extern void oj_seq_clear(oj_sequence_t *sp);
extern int oj_seq_deal_from_end(oj_sequence_t *sp);
extern int oj_seq_deal_to_end(oj_sequence_t *sp, int card);
extern int oj_seq_deal_from_head(oj_sequence_t *sp);
extern int oj_seq_deal_to_head(oj_sequence_t *sp, int card);
extern int oj_seq_pick(oj_sequence_t *sp, int card);
extern int oj_seq_move(oj_sequence_t *destp, oj_sequence_t *srcp, int count);
extern int oj_seq_copy(oj_sequence_t *destp, oj_sequence_t *srcp);
extern int oj_seq_fill(oj_sequence_t *sp, int count, oj_deck_type_t dt);
extern void oj_seq_sort(oj_sequence_t *sp);
extern void oj_seq_shuffle(oj_sequence_t *sp);

/* Various combinatorics */

extern unsigned long oj_binomial(int n, int k);
extern unsigned long oj_iter_combinations(oj_iterator_t *iter, int length, oj_sequence_t *deck);
extern unsigned long oj_iter_montecarlo(oj_iterator_t *iter, int length, oj_sequence_t *deck, long max);
extern unsigned long oj_iter_next(oj_iterator_t *iter);
extern unsigned long oj_iter_count(oj_iterator_t *iter);
extern unsigned long oj_colex_rank(oj_sequence_t *hand, oj_sequence_t *deck);
extern unsigned long oj_hand_at_colex_rank(unsigned long r, int length, oj_sequence_t *deck);

/* Poker functions */

extern int oj_poker_eval5(oj_sequence_t *seq);
extern char *oj_poker_handname(int cat);
extern int oj_poker_handgroup(int val);

#endif /* _ONEJOKER_H */
