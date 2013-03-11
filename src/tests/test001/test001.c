/* Tests for pseudo-random number generator.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "onejoker.h"

/* Just quickly verify that giving a non-zero seed gives us a
 * repeatable sequence, and a zero seed doesn't.
 */
int seed_test(void) {
    int i, seeds[] = { 1234, 9876, 0, 1234, 9876, 0 };
    uint32_t vals[6];

    for (i = 0; i < (sizeof(seeds)/sizeof(int)); ++i) {
        oj_seed_prng(seeds[i]);
        vals[i] = oj_prng_next32();
    }
    assert(vals[0] == vals[3]);
    assert(vals[1] == vals[4]);

    /* Yes, this will fail every 4 billion tests or so. That's OK.
     * If it fails more than that, something's wrong.
     */
    assert(vals[2] != vals[5]);
    fprintf(stderr, "Seed test passed.\n");
    return 1;
}

/* oj_rand() should give a balanced random number within limits.
 * Fill buckets with a few million of them and measure the fit
 * with the expected value.
 */
double balance_test(long count) {
    int i, n, v, bsize[] = { 5, 7, 13, 31, 52, 53, 54 };
    long j, buckets[54];
    double exp, fit, maxfit = 0.0;

    fprintf(stderr, "Balance test, %ld trials.\n", count);
    for (i = 0; i < 7; ++i) {
        n = bsize[i];

        for (j = 0; j < n; ++j) buckets[j] = 0;
        for (j = 0; j < count; ++j) {
            v = oj_rand(n);
            assert(v >= 0 && v < n);
            ++buckets[v];
        }
        fit = 0.0;
        exp = ((double)count / (double)n);
        for (j = 0; j < n; ++j) {
            fit += ((double)(buckets[j]) - exp) * ((double)(buckets[j]) - exp);
        }
        fit /= count;
        if (fit > maxfit) maxfit = fit;
        fprintf(stderr, "%2d buckets, fit: %.3f\n", n, fit);
    }
    return maxfit;
}

/* This function never returns! It runs the balance test first, then
 * continuously feeds random bits to stdout, which can be piped into
 * "dieharder -a -g 200" to verify the PRNG.
 */
void feed_dieharder(void) {
    int i;
    uint32_t buf[100];

    fprintf(stderr, "Feeding dieharder tests...\n");
    while (1) {
        for (i = 0; i < 100; ++i) {
            buf[i] = oj_prng_next32();
        }
        write(STDOUT_FILENO, buf, sizeof(buf));
    }
}

int main(int argc, char *argv[]) {
    int r;
    double fit;

    r = seed_test();
    assert(0 != r);

    fit = balance_test(10000000);
    assert(fit < 2.0);

    feed_dieharder();
    /* Never exits */
}
