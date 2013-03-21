/* OneJoker library <https://github.com/lcrocker/OneJoker>
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "onejoker.h"

oj_sequence_t deck, hand1, hand2;
oj_iterator_t iter1, iter2;
int dbuf[54], hbuf1[16], hbuf2[16], cbuf1[16], cbuf2[16];

void initialize(void) {
    ojs_new(&deck, 52, dbuf);
    ojs_fill(&deck, 52, oj_dt_standard);

    ojs_new(&hand1, 16, hbuf1);
    ojs_new(&hand2, 16, hbuf2);
}

void dump(oj_sequence_t *sp) {
    int i;
    fprintf(stderr, "[");
    for (i = 0; i < sp->length; ++i) {
        fprintf(stderr, "%s%2d", ((0==i)?"":" "), sp->cards[i]);
    }
    fprintf(stderr, "] ");
    fflush(stderr);
}

void run_iterations(void) {
}

void run_montecarlo(void) {
}

void test_combo_functions(void) {
    long long t;
    assert(0x10ACE0FF == deck._johnnymoss);

    ojs_shuffle(&deck);
    deck.length = 4;
    t = ojc_iter_new(&iter1, &deck, &hand1, 2, cbuf1, 0LL);
    // assert(1326 == t);

    dump(&deck);
    fprintf(stderr, "\n");

    while (ojc_iter_next(&iter1)) {
        dump(&hand1);
        fprintf(stderr, "(%lld) ", iter1.total - (iter1.remaining + 1));

        t = ojc_rank(&hand1, &iter1);
        fprintf(stderr, "(%lld) ", t);

        ojc_hand_at(t, &hand2, &iter1);
        dump(&hand2);
        fprintf(stderr, "\n");
    }
}

int main(int argc, char *argv[]) {

    initialize();
    run_iterations();
    run_montecarlo();
    test_combo_functions();

    return EXIT_SUCCESS;
}
