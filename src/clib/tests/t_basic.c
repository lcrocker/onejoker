/* Test card sequence and name functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#include "onejoker.h"

int cards_and_decks() {
    oj_sequence_t *dp;

    if (13 != OJ_CARD(OJR_FIVE, OJS_CLUB)) return 1;
    if (30 != OJ_CARD(OJR_NINE, OJS_DIAMOND)) return 2;
    if (43 != OJ_CARD(OJR_QUEEN, OJS_HEART)) return 3;
    if (52 != OJ_CARD(OJR_ACE, OJS_SPADE)) return 4;
    if (54 != OJ_JOKER2) return 5;

    if (OJR_TREY != OJ_RANK(7)) return 6;
    if (OJS_DIAMOND != OJ_SUIT(26)) return 7;
    if (OJR_JACK != OJ_RANK(38)) return 8;
    if (OJS_SPADE != OJ_SUIT(48)) return 9;
    if (OJR_JOKER != OJ_RANK(54)) return 10;

    if (ojd_ntypes() < 6) return 11;
    if (52 != ojd_ncards(OJD_STANDARD)) return 12;
    if (54 != ojd_ncards(OJD_TWOJOKERS)) return 13;
    if (40 != ojd_ncards(OJD_STRIPPED40)) return 14;

    dp = ojd_deck(OJD_ONEJOKER);
    if (53 != dp->length) return 15;
    if (OJ_CARD(OJR_DEUCE, OJS_CLUB) != dp->cards[0]) return 16;
    if (OJ_JOKER != dp->cards[52]) return 17;

    dp = ojd_deck_by_name("STRIPPED32");
    if (32 != dp->length) return 18;
    if (OJ_CARD(OJR_SEVEN, OJS_CLUB) != dp->cards[0]) return 19;
    if (OJ_CARD(OJR_ACE, OJS_SPADE) != dp->cards[31]) return 20;

    dp = ojd_deck(OJD_STRIPPED40J);
    if (41 != dp->length) return 21;
    if (OJ_CARD(OJR_SEVEN, OJS_SPADE) != dp->cards[23]) return 22;
    if (OJ_CARD(OJR_JACK, OJS_CLUB) != dp->cards[24]) return 23;
    if (OJ_JOKER != dp->cards[40]) return 24;

    return 0;
}

int text_functions() {
    return 0;
}

int main(int argc, char *argv[]) {
    int failed = 0;

    failed = cards_and_decks();
    failed = 100 * failed + text_functions();

    fprintf(stderr, "Basic C tests ");
    if (failed) {
        fprintf(stderr, "failed (code = %d).\n", failed);
    } else {
        fprintf(stderr, "passed.\n");
    }
    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
