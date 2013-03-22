#include <stdio.h>
#include <stdlib.h>

#include "../include/onejoker.h"
#include "lctables"

oj_sequence_t deck, hand;
oj_iterator_t iter;
int dbuf[60], hbuf[8], ibuf[8];

int neweval(oj_sequence_t *sp) {
    int v1, v2, v3, v4, r;

    v1 = 52 * (sp->cards[0] - 1) + (sp->cards[1] - 1);
    v2 = lct1[ v1 ];
    v3 = lct2[ v2 + sp->cards[2] - 1 ];
    v4 = lct3[ v3 + sp->cards[3]  - 1];
    r = lct4[ v4 + sp->cards[4] - 1];
    printf("%d %d %d %d %d\n", v1, v2, v3, v4, r);
    fflush(stdout);

    return r;
}

int main(int argc, char *argv[]) {
    int v1, v2;

    ojs_new(&deck, 60, dbuf);
    ojs_fill(&deck, 52, oj_dt_standard);

    ojs_new(&hand, 8, hbuf);
    ojc_iter_new(&iter, &deck, &hand, 5, ibuf, 10LL);

    while (ojc_iter_next_random(&iter)) {
        printf("(%d %d %d %d %d) ", hand.cards[0], hand.cards[1], hand.cards[2], hand.cards[3], hand.cards[4]);
        fflush(stdout);
        v1 = ojp_eval5(&hand);
        v2 = neweval(&hand);

        printf("%8lld %4d %4d\n", ojc_rank(&hand, &iter), v1, v2);
        fflush(stdout);
    }
}
