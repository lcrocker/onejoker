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

extern char *oj_cardname(int c);
extern char *oj_rankname(int r);
extern char *oj_suitname(int s);
extern char *oj_cardname_long(int c, char *buf, int size);
extern char *ojs_text(oj_sequence_t *sp, char *buf, int size);
extern int oj_cardval(char *str);
extern int oj_cardvals(char *str, int *arr, int size);

int text_functions() {
    int v, arr[20];
    char t[60];
    oj_sequence_t seq;

    if (0 != strcmp("Jd",
        oj_cardname(OJ_CARD(OJR_JACK, OJS_DIAMOND)))) return 1;    
    if (0 != strcmp("Tc",
        oj_cardname(OJ_CARD(OJR_TEN, OJS_CLUB)))) return 2;    
    if (0 != strcmp("5h",
        oj_cardname(OJ_CARD(OJR_FIVE, OJS_HEART)))) return 3;    
    if (0 != strcmp("JK", oj_cardname(OJ_JOKER))) return 4;

    if (OJ_CARD(OJR_DEUCE, OJS_SPADE) != oj_cardval("2s")) return 5;
    if (OJ_CARD(OJR_FOUR, OJS_CLUB) != oj_cardval(" 4 c")) return 6;
    if (OJ_CARD(OJR_SIX, OJS_DIAMOND) != oj_cardval("**6d")) return 7;
    if (OJ_CARD(OJR_QUEEN, OJS_SPADE) != oj_cardval("qS")) return 8;
    if (OJ_JOKER2 != oj_cardval("J2")) return 9;

    ojs_new(&seq, 20, arr);
    ojs_append(&seq, oj_cardval("3s"));
    ojs_append(&seq, oj_cardval("Ad"));
    ojs_append(&seq, oj_cardval("Jc"));
    ojs_append(&seq, oj_cardval("7d"));
    ojs_append(&seq, oj_cardval("10s"));

    ojs_text(&seq, t, 60);
    if (0 != strcmp("(3s Ad Jc 7d Ts)", t)) return 10;
    v = oj_cardvals("KsKc5c9d2hQcJk", arr, 20);
    if (7 != v) return 11;
    if (OJ_CARD(OJR_KING, OJS_SPADE) != arr[0]) return 12;
    if (OJ_CARD(OJR_QUEEN, OJS_CLUB) != arr[5]) return 13;
    ojs_text(&seq, t, 60);
    if (0 != strcmp("(Ks Kc 5c 9d 2h)", t)) return 14;

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
