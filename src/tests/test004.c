/* OneJoker library <https://github.com/lcrocker/OneJoker>
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "onejoker.h"

oj_sequence_t gdeck;
int gdbuf[60];

int poker_eval() {
    int v1, v2, hbuf[8], ibuf[8];
    oj_sequence_t hand;
    oj_iterator_t iter;

    ojs_new(&gdeck, 60, gdbuf);
    ojs_fill(&gdeck, 52, oj_dt_standard);

    ojs_new(&hand, 8, hbuf);
    ojc_iter_new(&iter, &gdeck, &hand, 5, ibuf, 0LL);

    while (ojc_iter_next(&iter)) {
        v1 = ojp_eval(&hand);

        // if (0 == (0x3FFFF & iter.remaining)) {
        //     printf("%10lld %d\n", iter.total - (iter.remaining + 1), v1);
        // }
    }
}

int main(int argc, char *argv[]) {
    int i;

    for (i = 0; i < 100; ++i) {
        poker_eval();
    }
    return EXIT_SUCCESS;
}
