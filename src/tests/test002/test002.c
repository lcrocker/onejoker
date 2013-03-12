/* Test card sequence and name functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#include "onejoker.h"

static oj_sequence_t g_hand1, g_hand2, g_deck;
static int g_hbuf1[4], g_hbuf2[16], g_dbuf[120];

static void dump(oj_sequence_t *sp) {
    int i;
    fprintf(stderr, "(%2d)", sp->length);
    for (i = 0; i < sp->length; ++i) {
        fprintf(stderr, " %2d", sp->cards[i]);
    }
    fprintf(stderr, "\n");
}

void initialize(void) {
    int s;

    s = oj_seq_new(&g_hand1, 4, g_hbuf1);
    assert(4 == s);
    s = oj_seq_new(&g_hand2, 16, g_hbuf2);
    assert(16 == s);
    s = oj_seq_new(&g_deck, 120, g_dbuf);
    assert(120 == s);
}

void adds(void) {
    int s;

    s = oj_seq_deal_to_end(&g_hand1, 21);
    assert(1 == s);

    s = oj_seq_deal_to_head(&g_hand1, 32);
    assert(2 == s);
    s = oj_seq_deal_to_end(&g_hand1, 43);
    assert(3 == s);
    s = oj_seq_deal_to_head(&g_hand1, 14);
    assert(4 == s && 4 == g_hand1.length && 14 == g_hand1.cards[0] &&
        32 == g_hand1.cards[1] && 21 == g_hand1.cards[2] && 43 == g_hand1.cards[3]);

    s = oj_seq_deal_to_end(&g_hand1, 5);
    assert(0 == s);
    s = oj_seq_deal_to_head(&g_hand1, 6);
    assert(0 == s && 4 == g_hand1.length && 14 == g_hand1.cards[0] &&
        32 == g_hand1.cards[1] && 21 == g_hand1.cards[2] && 43 == g_hand1.cards[3]);
}

void removes(void) {
    int c, s;

    /* Assumes conditions after sequence_adds */
    assert(4 == g_hand1.length && 14 == g_hand1.cards[0] &&
        32 == g_hand1.cards[1] && 21 == g_hand1.cards[2] && 43 == g_hand1.cards[3]);

    c = oj_seq_deal_from_end(&g_hand1);
    assert(43 == c && 3 == g_hand1.length);
    c = oj_seq_pick(&g_hand1, 43);
    assert(0 == c);
    s = oj_seq_deal_to_end(&g_hand1, 5);
    assert(4 == s);

    c = oj_seq_deal_from_head(&g_hand1);
    assert(14 == c && 3 == g_hand1.length);
    c = oj_seq_pick(&g_hand1, 21);
    assert(21 == c && 2 == g_hand1.length && 32 == g_hand1.cards[0] &&
        5 == g_hand1.cards[1]);

    oj_seq_clear(&g_hand1);
    assert(0 == g_hand1.length);
    c = oj_seq_deal_from_end(&g_hand1);
    assert(0 == c);
    c = oj_seq_deal_from_head(&g_hand1);
    assert(0 == c && 0 == g_hand1.length);
}

void fills(void) {
    int s;

    s = oj_seq_fill(&g_hand1, 10, oj_dt_standard);
    assert(4 == s && 1 == g_hand1.cards[0] && 4 == g_hand1.cards[3]);

    s = oj_seq_fill(&g_deck, 104, oj_dt_standard);
    assert(104 == s && 1 == g_deck.cards[0] && 52 == g_deck.cards[51] &&
        1 == g_deck.cards[52] && 2 == g_deck.cards[53] && 52 == g_deck.cards[103]);
    s = oj_seq_fill(&g_deck, 52, oj_dt_standard);
    assert(52 == s && 52 == g_deck.length && 1 == g_deck.cards[0] &&
        33 == g_deck.cards[32] && 52 == g_deck.cards[51]);
}

void move_and_copy(void) {
    int s;

    s = oj_seq_fill(&g_deck, 20, oj_dt_standard);
    oj_seq_sort(&g_deck);
    assert(20 == s && 20 == g_deck.cards[0]);

    oj_seq_clear(&g_hand1);
    oj_seq_deal_to_end(&g_hand1, 50);
    s = oj_seq_move(&g_hand1, &g_deck, 10);
    assert(3 == s && 4 == g_hand1.length && 50 == g_hand1.cards[0] &&
        3 == g_hand1.cards[1] && 1 == g_hand1.cards[3]);

    s = oj_seq_copy(&g_deck, &g_hand1);
    assert(4 == s && 4 == g_hand1.length && 50 == g_hand1.cards[0] &&
        3 == g_hand1.cards[1] && 1 == g_hand1.cards[3]);
    assert(4 == g_deck.length && 50 == g_deck.cards[0] &&
        3 == g_deck.cards[1] && 1 == g_deck.cards[3]);
}

void shuffle_and_sort(void) {
    int s;

    s = oj_seq_fill(&g_deck, 52, oj_dt_standard);
    assert(52 == s);
    oj_seq_shuffle(&g_deck);
    assert(52 == g_deck.length);
    /* This will fail once in a blue moon. */
    assert(! (1 == g_deck.cards[0] && 2 == g_deck.cards[1] && 13 == g_deck.cards[12]
        && 30 == g_deck.cards[29] && 51 == g_deck.cards[50] && 52 == g_deck.cards[51]));

    oj_seq_sort(&g_deck);
    assert(52 == g_deck.cards[0] && 51 == g_deck.cards[1] && 40 == g_deck.cards[12]
        && 23 == g_deck.cards[29] && 2 == g_deck.cards[50] && 1 == g_deck.cards[51]);

    oj_seq_fill(&g_deck, 12, oj_dt_standard);
    oj_seq_fill(&g_hand2, 12, oj_dt_standard);
    assert(g_deck.cards[0] == g_hand2.cards[0] && g_deck.cards[1] == g_hand2.cards[1]
        && g_deck.cards[4] == g_hand2.cards[4] && g_deck.cards[7] == g_hand2.cards[7]
        && g_deck.cards[10] == g_hand2.cards[10] && g_deck.cards[11] == g_hand2.cards[11]);

    oj_seq_shuffle(&g_deck);
    oj_seq_shuffle(&g_hand2);
    /* Again, like all tests of randomness, this will sometimes randomly fail. */
    assert(! (g_deck.cards[0] == g_hand2.cards[0] && g_deck.cards[1] == g_hand2.cards[1]
        && g_deck.cards[4] == g_hand2.cards[4] && g_deck.cards[7] == g_hand2.cards[7]
        && g_deck.cards[10] == g_hand2.cards[10] && g_deck.cards[11] == g_hand2.cards[11]));

    oj_seq_sort(&g_deck);
    oj_seq_sort(&g_hand2);
    assert(g_deck.cards[0] == g_hand2.cards[0] && g_deck.cards[1] == g_hand2.cards[1]
        && g_deck.cards[4] == g_hand2.cards[4] && g_deck.cards[7] == g_hand2.cards[7]
        && g_deck.cards[10] == g_hand2.cards[10] && g_deck.cards[11] == g_hand2.cards[11]);

    oj_seq_shuffle(&g_deck);
    /* Again, like all tests of randomness, this will sometimes randomly fail. */
    assert(! (g_deck.cards[0] == g_hand2.cards[0] && g_deck.cards[1] == g_hand2.cards[1]
        && g_deck.cards[4] == g_hand2.cards[4] && g_deck.cards[7] == g_hand2.cards[7]
        && g_deck.cards[10] == g_hand2.cards[10] && g_deck.cards[11] == g_hand2.cards[11]));

    oj_seq_sort(&g_deck);
    assert(g_deck.cards[0] == g_hand2.cards[0] && g_deck.cards[1] == g_hand2.cards[1]
        && g_deck.cards[4] == g_hand2.cards[4] && g_deck.cards[7] == g_hand2.cards[7]
        && g_deck.cards[10] == g_hand2.cards[10] && g_deck.cards[11] == g_hand2.cards[11]);
}

#define NSHUFFLES 10000000
static long buckets[52][52] = { 0 };

void shuffle_balance(void) {
    int i, j;
    double d, e, fit = 0.0;

    oj_seq_fill(&g_deck, 52, oj_dt_standard);
    for (i = 0; i < NSHUFFLES; ++i) {
        oj_seq_shuffle(&g_deck);

        for (j = 0; j < 52; ++j) {
            ++buckets[j][g_deck.cards[j] - 1];
        }
        if (0 == (i & 0xFFFF)) {
            fprintf(stderr, "%d\r", i);
        }
    }
    fprintf(stderr, "Performed %d shuffles.\n", NSHUFFLES);
    e = (double)NSHUFFLES / 52.0;

    for (i = 0; i < 52; ++i) {
        for (j = 0; j < 52; ++j) {
            d = (double)(buckets[i][j]) - e;
            fit += (d * d);
        }
    }
    fit /= (double)NSHUFFLES;
    fprintf(stderr, "Fit: %.3f\n", fit);
    assert(fit < 100.0);
}

int main(int argc, char *argv[]) {
    int r;
    double f;

    initialize();
    adds();
    removes();
    fills();
    move_and_copy();
    shuffle_and_sort();
    fprintf(stderr, "Sequence tests passed.\n");

    shuffle_balance();
    fprintf(stderr, "Done.\n");
    return EXIT_SUCCESS;
}
