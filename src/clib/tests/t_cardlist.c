/* OneJoker library <http://lcrocker.github.io/OneJoker>
 *
 * Test cardlist functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#include "onejoker.h"

oj_cardlist_t hand5, hand20, deck, shoe;
int hbuf5[5], hbuf20[20], dbuf[54], sbuf[6 * 52];

oj_cardlist_t uhand, rohand;
int ubuf[20], robuf[10] = { 25, 2, 31, 5, 1, };

void initialize(void) {
    int s;

    s = ojl_new(&hand5, hbuf5, 5);
    assert(5 == s);
    s = ojl_new(&hand20, hbuf20, 20);
    assert(20 == s);
    s = ojl_new(&deck, dbuf, 54);
    assert(54 == s);
    s = ojl_new(&shoe, sbuf, 6 * 52);
    assert(6 * 52 == s);

    s = ojl_new(&uhand, ubuf, 20);
    assert(20 == s);
    ojl_set_pflag(&uhand, OJF_UNIQUE);
    s = ojl_new(&rohand, robuf, 10);
    rohand.length = 5;
    ojl_set_pflag(&rohand, OJF_RDONLY);
}

int valid_mask(oj_cardlist_t *sp) {
    int i;
    long long m = 0LL;

    for (i = 0; i < uhand.length; ++i) m |= (1LL << sp->cards[i]);
    return m == sp->mask;
}

int t_truncate(void) {
    int r, o, n;

    n = ojr_rand(6);
    o = ojl_size(&hand5);
    r = ojl_truncate(&hand5, n);
    if (OJE_BADINDEX == r) {
        if (o >= n) return 1;
    } else {
        if (n != ojl_size(&hand5)) return 1;
    }
    n = ojr_rand(21);
    o = ojl_size(&hand20);
    r = ojl_truncate(&hand20, n);
    if (OJE_BADINDEX == r) {
        if (o >= n) return 1;
    } else {
        if (n != ojl_size(&hand20)) return 1;
    }
    r = ojl_truncate(&rohand, 3);
    if (OJE_RDONLY != r) return 1;
    ojl_truncate(&uhand, ojr_rand(21));
    return 0;
}

int t_append(void) {
    int i, r, c, ol, nl;

    ol = ojl_size(&hand5);
    c = ojr_rand(52) + 1;
    r = ojl_append(&hand5, c);
    nl = ojl_size(&hand5);

    if (OJE_FULL == r) {
        if (ol != 5 || nl != 5) return 1;
    } else {
        if (nl != (ol + 1)) return 1;
    }

    ol = ojl_size(&hand20);
    c = ojr_rand(52) + 1;
    r = ojl_append(&hand20, c);
    nl = ojl_size(&hand20);

    if (OJE_FULL == r) {
        if (ol != 20 || nl != 20) return 1;
    } else {
        if (nl != (ol + 1)) return 1;
    }
    ojl_append(&hand20, ojr_rand(52) + 1);

    r = ojl_append(&rohand, ojr_rand(52) + 1);
    if (OJE_RDONLY != r) return 1;

    ol = ojl_size(&uhand);
    c = ojr_rand(52) + 1;
    i = ojl_index(&uhand, c);
    r = ojl_append(&uhand, c);
    nl = ojl_size(&uhand);

    if (OJE_FULL == r) {
        if (ol != 20 || nl != 20) return 1;
    } else if (OJE_DUPLICATE == r) {
        if (OJE_NOTFOUND == i) return 1;
    } else {
        if (nl != (ol + 1)) return 1;
        if (OJE_NOTFOUND != i) return 1;
    }
    return 0;
}

int t_extend(void) {
    int i, r, ol, al, nl;

    ol = ojl_size(&hand20);
    al = ojl_size(&hand5);
    r = ojl_extend(&hand20, &hand5, 0);
    nl = ojl_size(&hand20);

    if (OJE_FULL == r) {
        if (ol + al <= 20) return 1;
    } else {
        if (nl != ol + al) return 1;
    }
    ojl_append(&hand5, ojr_rand(52) + 1);
    al = ojr_rand(ojl_size(&hand5)) + 1;
    ojl_truncate(&hand20, 20 - al);
    ol = ojl_size(&hand20);

    r = ojl_extend(&hand20, &hand5, al);
    nl = ojl_size(&hand20);
    if (r < 0) return 1;

    if (nl != ol + al) return 1;
    if (ojl_get(&hand5, 0) != ojl_get(&hand20, ol)) return 1;

    ojl_truncate(&uhand, 19);
    ol = ojl_size(&uhand);
    r = ojl_extend(&uhand, &hand5, 1);
    nl = ojl_size(&uhand);

    if (OJE_DUPLICATE == r) {
        if (ol != nl) return 1;
    } else {
        if (nl != ol + 1) return 1;
    }
    r = ojl_extend(&rohand, &hand5, 1);
    if (OJE_RDONLY != r) return 1;
    return 0;
}

int t_insert(void) {
    int r, c, oc, ol, nl;

    ojl_append(&hand20, ojr_rand(52) + 1);
    ojl_append(&hand20, ojr_rand(52) + 1);
    ojl_truncate(&hand20, 19);
    ol = ojl_size(&hand20);
    oc = ojl_get(&hand20, 1);
    c = ojr_rand(52) + 1;
    r = ojl_insert(&hand20, 1, c);

    if (oc != ojl_get(&hand20, 2)) return 1;
    if (c != ojl_get(&hand20, 1)) return 1;
    if (ol + 1 != ojl_size(&hand20)) return 1;

    ol = ojl_size(&hand5);
    c = ojr_rand(52) + 1;
    r = ojl_insert(&hand5, 0, c);
    nl = ojl_size(&hand5);

    if (OJE_FULL == r) {
        if (ol != 5 || nl != 5) return 1;
    } else {
        if (nl != (ol + 1)) return 1;
    }
    if (OJE_RDONLY != ojl_insert(&rohand, 0, c)) return 1;
    ojl_insert(&uhand, 0, c);

    return 0;
}

int t_pop(void) {
    int c, ol, nl;

    ol = ojl_size(&hand5);
    c = ojl_pop(&hand5);
    nl = ojl_size(&hand5);

    if (OJE_BADINDEX == c) {
        if (0 != ol) return 1;
    } else {
        if (nl != (ol - 1)) return 1;
    }
    ol = ojl_size(&hand20);
    c = ojl_pop(&hand20);
    nl = ojl_size(&hand20);

    if (OJE_BADINDEX == c) {
        if (0 != ol) return 1;
    } else {
        if (nl != (ol - 1)) return 1;
    }
    c = ojl_pop(&rohand);
    if (OJE_RDONLY != c) return 1;

    c = ojl_pop(&uhand);
    return 0;
}

int t_delete(void) {
    int c, oc, fc, ol, nl;

    ol = ojl_size(&hand5);
    if (0 == ojl_size(&hand5)) oc = OJE_BADINDEX;
    else oc = ojl_get(&hand5, 0);
    c = ojl_delete(&hand5, 0);
    nl = ojl_size(&hand5);

    if (OJE_BADINDEX == c) {
        if (0 != ol) return 1;
    } else {
        if (nl != (ol - 1)) return 1;
        if (c != oc) return 1;
    }
    ojl_append(&hand20, ojr_rand(52) + 1);
    ojl_append(&hand20, ojr_rand(52) + 1);
    ojl_append(&hand20, ojr_rand(52) + 1);
    ol = ojl_size(&hand20);
    oc = ojl_get(&hand20, 1);
    fc = ojl_get(&hand20, 2);
    c = ojl_delete(&hand20, 1);
    nl = ojl_size(&hand20);

    if ((c != oc) || (fc != ojl_get(&hand20, 1)) || (nl != ol - 1))
        return 1;

    c = ojl_delete(&rohand, 0);
    if (OJE_RDONLY != c) return 1;
    c = ojl_delete(&uhand, 0);
    return 0;
}

int t_remove(void) {
    int i, r, c, rc, ol, nl;

    ojl_truncate(&hand20, 19);
    c = ojr_rand(52) + 1;
    ojl_append(&hand20, c);
    ojl_shuffle(&hand20);

    ol = ojl_size(&hand20);
    i = ojl_index(&hand20, c);
    if (i < 0) return 1;

    rc = ojl_remove(&hand20, c);
    if (c != rc) return 1;
    if (ojl_size(&hand20) != ol - 1) return 1;

    do {
        rc = ojl_remove(&hand20, c);
    } while (c == rc);
    if (-1 != ojl_index(&hand20, c)) return 1;

    if (OJE_RDONLY != ojl_remove(&rohand, c)) return 1;
    ojl_remove(&uhand, c);
    return 0;
}

int t_sort_reverse(void) {
    int j;

    ojl_append(&hand20, ojr_rand(20) + 18);
    ojl_append(&hand20, ojr_rand(20) + 1);
    ojl_sort(&hand20);
    for (j = 1; j < ojl_size(&hand20); ++j) {
        if (hand20.cards[j] < hand20.cards[j - 1]) return 1;
    }
    ojl_reverse(&hand20);
    for (j = 1; j < ojl_size(&hand20); ++j) {
        if (hand20.cards[j] > hand20.cards[j - 1]) return 1;
    }
    if (OJE_RDONLY != ojl_sort(&rohand)) return 1;
    if (OJE_RDONLY != ojl_reverse(&rohand)) return 1;
    return 0;
}

int t_equal(void) {
    int j, hbuf[20];
    oj_cardlist_t hand;

    for (j = 0; j < 5; ++j) ojl_append(&hand5, ojr_rand(52) + 1);
    ojl_clear(&hand20);
    ojl_extend(&hand20, &hand5, 0);
    if (! ojl_equal(&hand5, &hand20)) return 1;

    ojl_append(&hand20, ojr_rand(52) + 1);
    if (ojl_equal(&hand5, &hand20)) return 1;

    ojl_pop(&hand20);
    if (! ojl_equal(&hand5, &hand20)) return 1;

    ojl_pop(&hand20);
    ojl_append(&hand20, 53);
    if (ojl_equal(&hand5, &hand20)) return 1;

    ojl_new(&hand, hbuf, 20);
    ojl_copy(&hand, &uhand);
    ojl_set_pflag(&hand, OJF_UNIQUE);
    if (! ojl_equal(&hand, &uhand)) return 1;

    ojl_sort(&hand);
    if (! ojl_equal(&hand, &uhand)) return 1;

    ojl_reverse(&hand);
    if (! ojl_equal(&hand, &uhand)) return 1;
    return 0;
}

int t_fill(void) {
    int j, c;

    c = 5 + ojr_rand(10);
    ojl_fill(&hand20, c, OJD_STANDARD);

    for (j = 0; j < ojl_size(&hand20); ++j) {
        if (j + 1 != hand20.cards[j]) return 1;
    }
    ojl_fill(&shoe, 3 * 52, OJD_STANDARD);
    if (1 != shoe.cards[52] || 1 != shoe.cards[104]) return 1;
    return 0;
}

int fuzz(int count) {
    int i, c;
    char text[64];

    for (i = 0; i < count; ++i) {
        c = ojr_rand(14) + 1;
        switch (c) {
        case 1:
            ojl_clear(&hand5);
            break;
        case 2:
            ojl_clear(&hand20);
            ojl_clear(&uhand);
            break;
        case 3:
            ojl_append(&hand5, ojr_rand(52) + 1);
            break;
        case 4:
            ojl_append(&hand20, ojr_rand(52) + 1);
            ojl_append(&hand20, ojr_rand(52) + 1);
            ojl_append(&uhand, ojr_rand(52) + 1);
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
        if (0 == valid_mask(&uhand)) return 100 + c;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int r, failed = 0, count = 10000000;

    initialize();
    r = fuzz(count);
    if (r) {
        fprintf(stderr, "Test #%d failed.\n", r);
    }
    failed |= r;

    fprintf(stderr, "Card list fuzz test ");
    if (failed) {
        fprintf(stderr, "failed. Code = %d", failed);
    } else {
        fprintf(stderr, "passed %d random operations.\n", count);
    }
    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
