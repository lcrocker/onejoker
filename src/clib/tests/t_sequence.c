/* OneJoker library <http://lcrocker.github.io/OneJoker>
 *
 * Test card sequence and name functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#include "onejoker.h"

oj_sequence_t hand5, hand20, deck, shoe;
int hbuf5[5], hbuf20[20], dbuf[54], sbuf[6 * 52];

void initialize(void) {
    int s;

    s = ojs_new(&hand5, 5, hbuf5);
    assert(5 == s);
    s = ojs_new(&hand20, 20, hbuf20);
    assert(20 == s);
    s = ojs_new(&deck, 54, dbuf);
    assert(54 == s);
    s = ojs_new(&shoe, 6 * 52, sbuf);
    assert(6 * 52 == s);
}

void cleanup(void) {
    ojs_clear(&hand5);
    ojs_clear(&hand20);
    ojs_clear(&deck);
    ojs_clear(&shoe);
}

int t_truncate(void) {
    int v1, v2;

    v1 = ojr_rand(6);
    v2 = ojr_rand(21);
    ojs_truncate(&hand5, v1);
    if (v1 < hand5.length) return 1;
    OJS_TRUNCATE(&hand20, v2);
    if (v2 < hand20.length) return 1;
    return 0;
}

int t_append(void) {
    int v1, v2, h5l, h20l;

    h5l = hand5.length;
    v1 = ojr_rand(52) + 1;
    OJS_APPEND(&hand5, v1);
    if (hand5.length != (h5l + 1)) {
        if (hand5.length != hand5.allocation) return 1;
    } else {
        if (v1 != hand5.cards[hand5.length - 1]) return 1;
    }
    h20l = hand20.length;
    v2 = ojr_rand(52) + 1;
    ojs_append(&hand20, v2);
    if (hand20.length != (h20l + 1)) {
        if (hand20.length != hand20.allocation) return 1;
    } else {
        if (v2 != hand20.cards[hand20.length - 1]) return 1;
    }
    ojs_append(&hand20, ojr_rand(52) + 1);
    return 0;
}

int t_extend(void) {
    int v1, h20l;

    v1 = hand20.length + hand5.length;
    ojs_extend(&hand20, &hand5, 0);
    if (v1 > 20) v1 = 20;
    if (v1 != hand20.length) return 1;

    ojs_append(&hand5, ojr_rand(52) + 1);
    v1 = ojr_rand(hand5.length) + 1;
    ojs_truncate(&hand20, 20 - v1);
    h20l = hand20.length;

    ojs_extend(&hand20, &hand5, v1);
    if (h20l + v1 != hand20.length) return 1;
    if (hand5.cards[0] != hand20.cards[h20l]) return 1;
    return 0;
}

int t_insert(void) {
    int v1, v2, v3, h20l;

    ojs_append(&hand20, ojr_rand(52) + 1);
    ojs_truncate(&hand20, hand20.length - 1);
    h20l = hand20.length;
    v1 = hand20.cards[0];
    v2 = hand20.cards[h20l - 1];
    v3 = ojr_rand(++h20l);

    ojs_insert(&hand20, v3, ojr_rand(52) + 1);
    if (h20l != hand20.length) return 1;

    if (0 != v3) {
        if (v1 != hand20.cards[0]) return 1;
    } else if (v3 < (h20l - 1)) {
        if (v2 != hand20.cards[h20l - 1]) return 1;
    }
    return 0;
}

int t_pop(void) {
    int v1, v2, h5l, h20l;

    h5l = hand5.length;
    v1 = OJS_POP(&hand5);
    if (0 == v1) {
        if (0 != h5l) return 1;
    } else {
        if (hand5.length != (h5l - 1)) return 1;
    }
    h20l = hand20.length;
    v2 = ojs_pop(&hand20);
    if (0 == v2) {
        if (0 != h20l) return 1;
    } else {
        if (hand20.length != (h20l - 1)) return 1;
    }
    return 0;
}

int t_delete(void) {
    int v1, v2, v3, h20l;

    h20l = hand20.length;
    ojs_delete(&hand20, 0);
    if (h20l != hand20.length + 1) {
        if (0 != h20l) return 1;
    }
    ojs_append(&hand20, ojr_rand(52) + 1);
    h20l = hand20.length;
    v1 = ojr_rand(h20l);
    v2 = hand20.cards[v1];

    v3 = ojs_delete(&hand20, v1);
    if (v2 != v3) return 1;
    if (h20l != hand20.length + 1) return 1;
    return 0;
}

int t_remove(void) {
    int v1, v2;

    ojs_truncate(&hand20, 19);
    v1 = ojr_rand(52) + 1;
    ojs_append(&hand20, v1);
    ojs_shuffle(&hand20);
    if (-1 == ojs_index(&hand20, v1)) return 1;

    v2 = ojs_remove(&hand20, v1);
    if (v2 != v1) return 1;

    do {
        v2 = ojs_remove(&hand20, v1);
    } while (0 != v2);
    if (-1 != ojs_index(&hand20, v1)) return 1;
    return 0;
}

int t_sort_reverse(void) {
    int j;

    ojs_append(&hand20, ojr_rand(20) + 18);
    OJS_APPEND(&hand20, ojr_rand(20) + 1);
    ojs_sort(&hand20);
    for (j = 1; j < hand20.length; ++j) {
        if (hand20.cards[j] < hand20.cards[j - 1]) return 1;
    }
    ojs_reverse(&hand20);
    for (j = 1; j < hand20.length; ++j) {
        if (hand20.cards[j] > hand20.cards[j - 1]) return 1;
    }
    return 0;
}

int t_equal(void) {
    int j;

    for (j = 0; j < 5; ++j) ojs_append(&hand5, ojr_rand(52) + 1);
    ojs_clear(&hand20);
    ojs_extend(&hand20, &hand5, 0);
    if (! ojs_equal(&hand5, &hand20)) return 1;

    ojs_append(&hand20, ojr_rand(52) + 1);
    if (ojs_equal(&hand5, &hand20)) return 1;

    OJS_POP(&hand20);
    if (! ojs_equal(&hand5, &hand20)) return 1;

    ojs_pop(&hand20);
    OJS_APPEND(&hand20, 53);
    if (ojs_equal(&hand5, &hand20)) return 1;
    return 0;
}

int t_fill(void) {
    int j, v1, v2;

    v1 = 15 + ojr_rand(10);
    v2 = ojs_fill(&hand20, v1, oj_dt_standard);
    if (v2 != v1) {
        if (v2 != 20) return 1;
    }
    for (j = 0; j < hand20.length; ++j) {
        if (j + 1 != hand20.cards[j]) return 1;
    }
    ojs_fill(&shoe, 3 * 52, oj_dt_standard);
    if (1 != shoe.cards[52] || 1 != shoe.cards[104]) return 1;
    return 0;
}

int fuzz(int count) {
    int i, c;

    for (i = 0; i < count; ++i) {
        c = ojr_rand(14) + 1;
        switch (c) {
        case 1:
            ojs_clear(&hand5);
            break;
        case 2:
            OJS_CLEAR(&hand20);
            break;
        case 3:
            ojs_append(&hand5, ojr_rand(52) + 1);
            break;
        case 4:
            ojs_append(&hand20, ojr_rand(52) + 1);
            OJS_APPEND(&hand20, ojr_rand(52) + 1);
            break;
        case 5:
            if (0 != t_truncate()) return c;
            break;
        case 6:
            if (0 != t_append()) return c;
            break;
        case 7:
            if (0 != t_extend()) return c;
            break;
        case 8:
            if (0 != t_insert()) return c;
            break;
        case 9:
            if (0 != t_pop()) return c;
            break;
        case 10:
            if (0 != t_delete()) return c;
            break;
        case 11:
            if (0 != t_remove()) return c;
            break;
        case 12:
            if (0 != t_sort_reverse()) return c;
            break;
        case 13:
            if (0 != t_equal()) return c;
            break;
        case 14:
            if (0 != t_fill()) return c;
            break;
        default:
            assert(0);
        }
    }
    cleanup();
    return 0;
}

int main(int argc, char *argv[]) {
    int r, failed = 0, count = 1000000;

    initialize();
    r = fuzz(count);
    if (r) {
        fprintf(stderr, "Test #%d failed.\n", r);
    }
    failed |= r;

    fprintf(stderr, "Sequence tests ");
    if (failed) {
        fprintf(stderr, "failed. Code = %d", failed);
    } else {
        fprintf(stderr, "passed %d random operations.\n", count);
    }
    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
