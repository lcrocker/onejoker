/* Test card sequence and name functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#include "onejoker.h"

oj_sequence_t hand4, hand16, deck, shoe;
int hbuf4[4], hbuf16[16], dbuf[54], sbuf[6 * 52];

void initialize(void) {
    int s;

    s = ojs_new(&hand4, 4, hbuf4);
    assert(4 == s);
    s = ojs_new(&hand16, 16, hbuf16);
    assert(16 == s);
    s = ojs_new(&deck, 54, dbuf);
    assert(54 == s);
    s = ojs_new(&shoe, 6 * 52, dbuf);
    assert(6 * 52 == s);
}

void cleanup(void) {
    ojs_clear(&hand4);
    ojs_clear(&hand16);
    ojs_clear(&deck);
    ojs_clear(&shoe);
}

int tcfill(oj_sequence_t *sp, int *tc) {
    int i;

    ojs_clear(sp);
    for (i = 0; i < tc[0]; ++i) {
        ojs_deal_to(sp, tc[i + 1]);
    }
}

int tcequal(oj_sequence_t *sp, int *tc) {
    int i;

    if (*tc != sp->length) return 0;
    for (i = 0; i < sp->length; ++i) {
        if (tc[i + 1] != sp->cards[i]) return 0;
    }
    return 1;
}

void show(oj_sequence_t *sp) {
    int i;
    fprintf(stderr, "(%2d)", sp->length);
    for (i = 0; i < sp->length; ++i) {
        fprintf(stderr, " %2d", sp->cards[i]);
    }
    fprintf(stderr, "\n");
}

int tc001[] = { 2, 21, 45 };
int tc002[] = { 4, 32, 21, 45, 43 };
int tc003[] = { 4, 6, 21, 45, 50 };
int tc004[] = { 6, 14, 32, 21, 45, 43, 8 };
int tc005[] = { 3, 21, 45, 43 };

void add_remove(void) {
    int s, c;

    cleanup();

    s = ojs_deal_to(&hand4, 21);
    assert(1 == s);
    s = ojs_deal_to(&hand4, 45);
    assert(2 == s && tcequal(&hand4, tc001));

    s = ojs_deal_to_head(&hand4, 32);
    assert(3 == s);
    s = ojs_deal_to(&hand4, 43);
    assert(4 == s && tcequal(&hand4, tc002));

    s = ojs_deal_to(&hand4, 14);
    assert(0 == s && tcequal(&hand4, tc002));
    s = ojs_deal_to_head(&hand4, 12);
    assert(0 == s && tcequal(&hand4, tc002));

    c = ojs_deal_from(&hand4);
    assert(43 == c && 3 == hand4.length);
    c = ojs_deal_from_head(&hand4);
    assert(32 == c && 2 == hand4.length);
    s = ojs_deal_to(&hand4, 50);
    assert(3 == s);
    s = ojs_deal_to_head(&hand4, 6);
    assert(4 == s && tcequal(&hand4, tc003));

    tcfill(&hand16, tc002);
    s = ojs_deal_to_head(&hand16, 14);
    s = ojs_deal_to(&hand16, 8);
    assert(tcequal(&hand16, tc004));

    c = ojs_deal_from(&hand16);
    assert(8 == c);
    c = ojs_pick(&hand16, 50);
    assert(0 == c);
    c = ojs_pick(&hand16, 32);
    assert(32 == c);
    c = ojs_deal_from_head(&hand16);
    assert(14 == c && tcequal(&hand16, tc005));

    ojs_deal_from(&hand16);
    c = ojs_pick(&hand16, 43);
    assert(0 == c);
    c = ojs_pick(&hand16, 21);
    assert(21 == c);
    c = ojs_pick(&hand16, 45);
    assert(45 == c && 0 == hand16.length);

    ojs_clear(&hand4);
    c = ojs_deal_from(&hand4);
    assert(0 == c);
    c = ojs_deal_from(&hand4);
    assert(0 == c);
}

int tc006[] = { 4, 1, 2, 3, 4 };
int tc007[] = { 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
int tc008[] = { 14, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 49, 50, 51, 52 };
int tc009[] = { 3, 46, 47, 48 };

void fill_move_copy(void) {
    int s;

    cleanup();

    s = ojs_fill(&hand4, 10, oj_dt_standard);
    assert(4 == s && tcequal(&hand4, tc006));
    s = ojs_fill(&hand16, 10, oj_dt_standard);
    assert(10 == s && tcequal(&hand16, tc007));

    s = ojs_fill(&deck, 52, oj_dt_standard);
    assert(52 == s && 1 == deck.cards[0] && 52 == deck.cards[51]);
    s = ojs_fill(&shoe, 4 * 53, oj_dt_1joker);
    assert(4 * 53 == s && 53 == deck.cards[52] && 1 == deck.cards[53]);

    s = ojs_move(&hand16, &deck, 4);
    assert(4 == s && tcequal(&hand16, tc008));
    s = ojs_copy_all(&shoe, &deck);
    assert(48 == s && 48 == shoe.length);

    ojs_clear(&hand4);
    s = ojs_copy(&hand4, &deck, 3);
    assert(3 == s && tcequal(&hand4, tc009) && 48 == deck.length);

    ojs_clear(&hand16);
    s = ojs_move(&hand16, &hand4, 10);
    assert(3 == s && tcequal(&hand16, tc009) && 0 == hand4.length);
    s = ojs_move(&hand16, &hand4, 2);
    assert(0 == s && tcequal(&hand16, tc009));
    s = ojs_copy(&hand16, &hand4, 2);
    assert(0 == s && tcequal(&hand16, tc009));
    s = ojs_copy_all(&hand16, &hand4);
    assert(0 == s && 0 == hand16.length && 0 == hand4.length);
}

int tc010[] = { 10, 42, 14, 20, 21, 6, 51, 12, 33, 2, 19 };
int tc011[] = { 10, 51, 42, 33, 21, 20, 19, 14, 12, 6, 2 };

void shuffle_sort(void) {
    int s;

    cleanup();

    tcfill(&hand16, tc010);
    ojs_sort(&hand16);
    assert(tcequal(&hand16, tc011));
    ojs_shuffle(&hand16);
    assert(! tcequal(&hand16, tc011)); /* Will fail rarely */
    ojs_sort(&hand16);
    assert(tcequal(&hand16, tc011));
}

#define NSHUFFLES 1000000
static long buckets[52][52] = { 0 };

void shuffle_balance(void) {
    int i, j;
    double d, e, fit = 0.0;

    ojs_fill(&deck, 52, oj_dt_standard);
    for (i = 0; i < NSHUFFLES; ++i) {
        ojs_shuffle(&deck);

        for (j = 0; j < 52; ++j) {
            ++buckets[j][deck.cards[j] - 1];
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
    fit /= (52.0 * NSHUFFLES);
    fprintf(stderr, "Fit: %.3f\n", fit);
    assert(fit < 2.0);
}

int main(int argc, char *argv[]) {
    int r;
    double f;

    initialize();
    add_remove();
    fill_move_copy();
    shuffle_sort();
    fprintf(stderr, "Sequence tests passed.\n");

    shuffle_balance();
    fprintf(stderr, "Done.\n");
    return EXIT_SUCCESS;
}
