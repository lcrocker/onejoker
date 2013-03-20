/* OneJoker library <https://github.com/lcrocker/OneJoker>
 */

#ifndef _ONEJOKER_H
#define _ONEJOKER_H

#include <stdint.h>

extern int _oj_johnnymoss; /* Initialization check. */

/* Deck types. Numbers are assigned here because they are used
 * to index the internal deck information structure array.
 */
typedef enum _oj_deck_type {
    oj_dt_standard = 0, oj_dt_1joker = 1, oj_dt_2jokers = 2,
    oj_dt_stripped32 = 3, oj_dt_stripped40 = 4, oj_dt_stripped40j = 5
} oj_deck_type_t;

typedef struct _oj_deck_info {
    int size;
    int *cards;
} oj_deck_info_t;

/* Defined in decktypes.c */
extern oj_deck_info_t oj_deck_info[];

/* This is the sequence type used by the client. Mostly just a
 * typical array-like thing. Client is responsible for allocating
 * memory and initializing, and we check for this in several places.
 */
typedef struct _oj_sequence {
    int _johnnymoss;  /* Initialization check. */
    int allocation;
    int length;
    int *cards;
} oj_sequence_t;

/* Structure that maintains the internal state of an iterator.
 */
typedef struct _oj_iterator {
    int _johnnymoss; /* Initialization check */
    int k;
    int *a;
    long long total, remaining;
    oj_sequence_t *deck;
    oj_sequence_t *hand;
    int deck_invert[55];
} oj_iterator_t;

/* Information needed to display value of poker hand.
 */
typedef struct _oj_poker_hand_info {
    int val, group;
    char *group_name;
    int nranks;
    int ranks[5];
} oj_poker_hand_info_t;

/* General library functions */

extern int oj_init_library(int seed);

/* PRNG functions */

extern int ojr_seed(int seed);
extern uint16_t ojr_next16(void);
extern uint32_t ojr_next32(void);
extern int ojr_rand(int limit);

/* Sequences */

extern int ojs_new(oj_sequence_t *sp, int size, int *buf);
extern int ojs_append(oj_sequence_t *sp, int card);
extern int ojs_extend(oj_sequence_t *destp, oj_sequence_t *srcp, int count);
extern int ojs_delete(oj_sequence_t *sp, int index);
extern int ojs_insert(oj_sequence_t *sp, int index, int card);
extern int ojs_pop(oj_sequence_t *sp);
extern int ojs_remove(oj_sequence_t *sp, int card);
extern int ojs_index(oj_sequence_t *sp, int card);
extern void ojs_sort(oj_sequence_t *sp);
extern void ojs_reverse(oj_sequence_t *sp);
extern int ojs_equal(oj_sequence_t *sp1, oj_sequence_t *sp2);
extern int ojs_fill(oj_sequence_t *sp, int count, oj_deck_type_t dt);
extern void ojs_shuffle(oj_sequence_t *sp);

/* Combinatorics */

extern long long ojc_binomial(int n, int k);
extern long long ojc_iter_new(oj_iterator_t *iter, oj_sequence_t *deck,
    oj_sequence_t *hand, int k, int *hbuf, long long count);
extern int ojc_iter_next(oj_iterator_t *iter);
extern int ojc_iter_next_random(oj_iterator_t *iter);
extern long long ojc_colex_rank(oj_sequence_t *hand, oj_iterator_t *iter);
extern int ojc_colex_hand_at(oj_sequence_t *hand, oj_iterator_t *iter, long long rank);

/* Poker functions */

extern int ojp_eval5(oj_sequence_t *sp);
extern int ojp_hand_info(oj_poker_hand_info_t *ip, int val);

/* Blackjack functions */

#endif /* _ONEJOKER_H */
