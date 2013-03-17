/* Primary declarations for the OneJoker library.
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

extern oj_deck_info_t oj_deck_info[];

/* Styles for rendering cards as text.
 */
typedef enum _oj_card_style {
    oj_cs_ascii2, oj_cs_ascii3, oj_cs_unicode2, oj_cs_unicode3,
    oj_cs_fulltext, oj_cs_html, oj_cs_htmlcolor,
} oj_card_style_t;

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
    int _sorted;
    int k;
    int *a;
    long long rank, remaining;
    oj_sequence_t *deck;
    oj_sequence_t *hand;
} oj_iterator_t;

/* Blackjack game rules.
 */
typedef struct _oj_blackjack_rules {
    int soft17;
} oj_blackjack_rules_t;

/* General library functions */

extern int oj_init_library(int seed);

/* Card names */

extern int oj_card_from_text(char *text, char **next);
extern char *oj_text_of_card(int card, oj_card_style_t style);
extern int ojs_text(oj_sequence_t *sp, char *text, int size, char sep);
extern int ojs_add_by_text(oj_sequence_t *sp, char *text);

/* PRNG functions */

extern int ojr_seed(int seed);
extern uint16_t ojr_next16(void);
extern uint32_t ojr_next32(void);
extern int ojr_rand(int limit);

/* Sequences */

extern int ojs_new(oj_sequence_t *sp, int size, int *buf);
extern void ojs_clear(oj_sequence_t *sp);
extern int ojs_deal_from(oj_sequence_t *sp);
extern int ojs_deal_to(oj_sequence_t *sp, int card);
extern int ojs_deal_from_head(oj_sequence_t *sp);
extern int ojs_deal_to_head(oj_sequence_t *sp, int card);
extern int ojs_pick(oj_sequence_t *sp, int card);
extern int ojs_move(oj_sequence_t *destp, oj_sequence_t *srcp, int count);
extern int ojs_copy(oj_sequence_t *destp, oj_sequence_t *srcp, int count);
extern int ojs_copy_all(oj_sequence_t *destp, oj_sequence_t *srcp);
extern int ojs_fill(oj_sequence_t *sp, int count, oj_deck_type_t dt);
extern void ojs_sort(oj_sequence_t *sp);
extern void ojs_shuffle(oj_sequence_t *sp);
extern int ojs_equal(oj_sequence_t *sp1, oj_sequence_t *sp2);
extern int ojs_truncate(oj_sequence_t *sp, int size);

/* Combinatorics */

long long ojc_binomial(int n, int k);
long long ojc_iter_new(oj_iterator_t *iter, oj_sequence_t *deck,
    oj_sequence_t *hand, int k, int *buf);
int ojc_iter_next(oj_iterator_t *iter);
void ojc_iter_next_random(oj_iterator_t *iter);

/* Poker functions */

extern int ojp_eval5(oj_sequence_t *sp);
extern int ojp_handgroup(int val);
extern char *ojp_handname(int cat);

/* Blackjack functions */

/* extern int ojb_val(oj_sequence_t *sp); */

#endif /* _ONEJOKER_H */
