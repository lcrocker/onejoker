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

/* Poker games. A "game" in this sense is just a hand evaluation method, not a
 * type of game in the sense of order of play or betting methods and so on.
 * Just how is one hand judged better than another.
 */
typedef enum _oj_poker_game {
    oj_pg_standard = 0, oj_pg_acetofive = 1, oj_pg_deucetoseven = 2,
    oj_pg_acetosix = 3, oj_pg_badugi = 4
} oj_poker_game_t;

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
    long long total, count, remaining;
    oj_sequence_t *deck;
    oj_sequence_t *hand;
    int deck_invert[64];
} oj_iterator_t;

/* Information needed to display value of poker hand.
 */
typedef struct _oj_poker_hand_info {
    int _johnnymoss;
    int val, group;
    int nranks;
    int ranks[5];
} oj_poker_hand_info_t;

/* General library functions */

#define OJ_RANK(c) (((c)-1)>>2)
#define OJ_SUIT(c) ((c)&3)

#define OJR_DEUCE 0
#define OJR_TREY 1
#define OJR_FOUR 2
#define OJR_FIVE 3
#define OJR_SIX 4
#define OJR_SEVEN 5
#define OJR_EIGHT 6
#define OJR_NINE 7
#define OJR_TEN 8
#define OJR_JACK 9
#define OJR_QUEEN 10
#define OJR_KING 11
#define OJR_ACE 12
#define OJR_JOKER 13

#define OJS_CLUB 0
#define OJS_DIAMOND 1
#define OJS_HEART 2
#define OJS_SPADE 3

extern int oj_init_library(int seed);
extern char *oj_cardname(int c);
extern char *oj_rankname(int r);
extern char *oj_suitname(int s);
extern char *oj_cardname_long(int c);

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
extern void _ojs_sort_int_array(int n, int *cp);
extern void ojs_sort(oj_sequence_t *sp);
extern void ojs_reverse(oj_sequence_t *sp);
extern int ojs_equal(oj_sequence_t *sp1, oj_sequence_t *sp2);
extern int ojs_fill(oj_sequence_t *sp, int count, oj_deck_type_t dt);
extern void ojs_shuffle(oj_sequence_t *sp);
extern char *ojs_text(oj_sequence_t *sp);

/* Combinatorics */

extern long long ojc_binomial(int n, int k);
extern long long ojc_iter_new(oj_iterator_t *iter, oj_sequence_t *deck,
    oj_sequence_t *hand, int k, int *hbuf, long long count);
extern int ojc_iter_next(oj_iterator_t *iter);
extern int ojc_iter_next_random(oj_iterator_t *iter);
extern long long ojc_rank(oj_sequence_t *hand, oj_iterator_t *iter);
extern void ojc_hand_at(long long rank, oj_sequence_t *hand, oj_iterator_t *iter);

/* Poker functions */

extern int ojp_eval5(oj_sequence_t *sp);
extern int ojp_best5(oj_sequence_t *sp, oj_sequence_t *bh);
extern int ojp_hand_info(oj_poker_hand_info_t *ip, oj_sequence_t *sp, int val);
extern char *ojp_hand_description(oj_poker_hand_info_t *pi);

/* Blackjack functions */

#endif /* _ONEJOKER_H */
