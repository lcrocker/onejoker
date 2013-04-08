/* OneJoker library <https://github.com/lcrocker/OneJoker>
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>

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

int validate(void) {
    int i, f[54];

    for (i = 0; i < iter1.deck->length; ++i) {
        f[i] = 0;
        if (i != iter1.deck_invert[iter1.deck->cards[i]]) return 2;
    }
    for (i = 0; i < iter1.k; ++i) {
        if (iter1.a[i] < 0 || iter1.a[i] >= iter1.deck->length) return 1;
        if (++f[iter1.a[i]] > 1) return 3;
    }
    return 0;
}

int nvals[] = { 52, 50, 40, 31, 22, 16, 12, 7 };
int kvals[] = {  2,  3,  4,  5,  6,  7,  7, 7 };
long long tvals[] = { 1326, 19600, 91390, 169991, 74613, 11440, 792, 1 };

int test_combinations(int n, int k) {
    int i, r;
    long long t, c;
    assert(0x10ACE0FF == deck._johnnymoss);

    ojs_fill(&deck, 52, oj_dt_standard);
    ojs_shuffle(&deck);
    ojs_truncate(&deck, n);

    t = ojc_iter_new(&iter1, &deck, &hand1, k, cbuf1, 0LL);
    if (t != iter1.total || t != iter1.remaining) return 50;

    c = 0;
    while (ojc_iter_next(&iter1)) {
        if (c != (iter1.total - iter1.remaining) - 1) return 10;

        t = ojc_rank(&hand1, &iter1);
        ojc_hand_at(t, &hand2, &iter1);
        if (! ojs_equal(&hand2, &hand1)) return 30;
        ++c;

        if (0 == ojr_rand(10000)) {
            r = validate();
            if (r) return r + 20;
        }
    }
    for (i = 0; i < k; ++i) {
        hand2.cards[i] = deck.cards[deck.length - k + i];
        hand2.length = k;
    }
    if (! ojs_equal(&hand1, &hand2)) return 40;
    return 0;
}

int loop_combinations(void) {
    int i, k, n, r;

    for (i = 0; i < 8; ++i) {
        n = nvals[i];
        k = kvals[i];

        r = test_combinations(n, k);
        if (0 != r) return r;

        n = 20 + ojr_rand(10);
        k = 1 + ojr_rand(4);

        r = test_combinations(n, k);
        if (0 != r) return r;
    }
    return 0;
}

int test_montecarlo(int n, int k, long long count) {
    int r;
    long long t;
    assert(0x10ACE0FF == deck._johnnymoss);

    ojs_fill(&deck, 52, oj_dt_standard);
    ojs_shuffle(&deck);
    ojs_truncate(&deck, n);

    t = ojc_iter_new(&iter1, &deck, &hand1, k, cbuf1, count);
    while (ojc_iter_next_random(&iter1)) {
        t = ojc_rank(&hand1, &iter1);
        ojc_hand_at(t, &hand2, &iter1);
        if (! ojs_equal(&hand2, &hand1)) return 70;

        if (0 == ojr_rand(10000)) {
            r = validate();
            if (r) return r + 60;
        }
    }
    return 0;
}

int loop_montecarlo(void) {
    int i, k, n, r;

    for (i = 0; i < 8; ++i) {
        n = 20 + ojr_rand(10);
        k = 2 + ojr_rand(4);

        r = test_montecarlo(n, k, 100000LL);
        if (0 != r) return r;
    }
    return 0;
}

long long buckets[52];

int random_balance(void) {
    int i;
    long long count = 1000000;
    double exp, d, t, sd, z, maxz;

    ojs_fill(&deck, 52, oj_dt_standard);
    ojc_iter_new(&iter1, &deck, &hand1, 5, cbuf1, count);

    for (i = 0; i < 52; ++i) buckets[i] = 0;
    while (ojc_iter_next_random(&iter1)) {
        for (i = 0; i < 5; ++i) {
            ++buckets[hand1.cards[i] - 1];
        }
    }
    t = 0.0;
    exp = ((5.0 * count) / 52.0);
    for (i = 0; i < 52; ++i) {
        d = (double)(buckets[i]) - exp;
        t += d * d;
    }
    sd = sqrt(t / 51.0);

    maxz = 0.0;
    for (i = 0; i < 52; ++i) {
        z = fabs(((double)(buckets[i]) - exp) / sd);
        if (z > maxz) maxz = z;
    }
    if (maxz > 3.9) return 90;
    return 0;
}

int main(int argc, char *argv[]) {
    int r, failed = 0;

    initialize();
    r = loop_combinations();
    failed |= r;
    fprintf(stderr, "Combinations test %sed.\n", (r ? "fail" : "pass"));

    r = loop_montecarlo();
    failed |= r;
    fprintf(stderr, "Monte carlo test %sed.\n", (r ? "fail" : "pass"));

    r = random_balance();
    failed |= r;
    fprintf(stderr, "Balance test %sed.\n", (r ? "fail" : "pass"));

    fprintf(stderr, "Combinatorics tests ");
    if (failed) {
        fprintf(stderr, "failed. Code: %d\n", failed);
    } else {
        fprintf(stderr, "passed.\n");
    }
    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
