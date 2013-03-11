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

void dump(oj_sequence_t *sp) {
    int i;
    fprintf(stderr, "%d ", sp->length);
    for (i = 0; i < sp->length; ++i) {
        fprintf(stderr, "(%d,%d) ", i, sp->cards[i]);
    }
    fprintf(stderr, "\n");
}

void move_and_copy(void) {
}

unsigned long long binomial_table[53][11];

int main(int argc, char *argv[]) {
	int r;
    int n, k;
    unsigned long long bc;

    initialize();
	adds();
    removes();
    fills();
    move_and_copy();
    fprintf(stderr, "Sequence tests passed.\n");

    binomial_table[0][0] = 1;
    for (n = 3; n <= 54; ++n) {
        binomial_table[n-2][0] = (n - 1) + binomial_table[n-3][0];
        for (k = 3; k <= 12; ++k) {
            binomial_table[n-2][k-2] = binomial_table[n-3][k-3] +
                binomial_table[n-3][k-2];
        }
    }
    printf("table = {\n");
    for (n = 2; n <= 54; ++n) {
        printf("    { %llu", binomial_table[n-2][0]);
        for (k = 3; k <= 12; ++k) {
            bc = binomial_table[n-2][k-2];
            if (bc > ULONG_MAX) printf(", 0");
            else printf(", %llu", bc);
        }
        printf(" },\n");
    }
    printf("};\n");

	fprintf(stderr, "Done.\n");
	return EXIT_SUCCESS;
}
