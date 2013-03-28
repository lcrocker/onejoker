/* Tests for pseudo-random number generator.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <math.h>

#include "onejoker.h"

/* Quickly verify that giving a non-zero seed gives us a repeatable sequence,
 * and that a zero seed doesn't.
 */
static uint32_t vals[6][100];

int allequal(x, y, s, n) {
    int i;
    for (i = s; i < n; ++i) {
        if (vals[x][i] != vals[y][i]) return 0;
    }
    return 1;
}

int seed_test(void) {
    int i, j;
    int seeds[] = { 0x12345678, 0x9ABCDEF0, 0, 0x12345678, 0x9ABCDEF0, 0 };

    for (i = 0; i < 6; ++i) {
        ojr_seed(seeds[i]);
        for (j = 0; j < 100; ++j) { vals[i][j] = ojr_next32(); }
    }
    if (! allequal(0, 3, 0, 100)) return 10;
    if (! allequal(1, 4, 0, 100)) return 20;
    for (i = 0; i < 10; ++i) {
        if (allequal(2, 5, 10 * i, 10 * i + 10)) return 30;
    }
    return 0;
}

/* Test that ojr_rand() gives a balanced random number within limits. Fill
 * buckets with a few million of them and measure the variance from the
 * expected value.
 */
double balance_test(long count) {
    int i, j, n, v, bcount[] = { 5, 7, 13, 31, 52, 53, 54 };
    long k, buckets[54];
    double exp, d, t, sd, z, maxz;
    assert(count > 1);

    maxz = 0.0;
    for (i = 0; i < 7; ++i) {
        n = bcount[i];

        for (j = 0; j < n; ++j) buckets[j] = 0;
        for (k = 0; k < count; ++k) {
            v = ojr_rand(n);
            assert(v >= 0 && v < n);
            ++buckets[v];
        }

        t = 0.0;
        exp = ((double)count / (double)n);
        for (j = 0; j < n; ++j) {
            d = (double)(buckets[j]) - exp;
            t += d * d;
        }
        sd = sqrt( t / (double)(n - 1) );

        for (j = 0; j < n; ++j) {
            z = fabs(((double)(buckets[j]) - exp) / sd);
            if (z > maxz) maxz = z;
        }
    }
    return maxz;
}

/* This function never returns! It continuously feeds random bits to stdout,
 * which can be piped into "dieharder -a -g 200" to verify the PRNG.
 */
void feed_dieharder(void) {
    int i;
    uint32_t buf[100];

    while (1) {
        for (i = 0; i < 100; ++i) {
            buf[i] = ojr_next32();
        }
        write(STDOUT_FILENO, buf, sizeof(buf));
    }
}

int main(int argc, char *argv[]) {
    int r, failed = 0;
    double fit;

    r = seed_test();
    failed |= r;
    fprintf(stderr, "Seed test returned %d (%s).\n", r, (r ? "fail" : "pass"));

    fit = balance_test(1000000);
    if (fit > 3.9) failed |= 1;
    fprintf(stderr, "Balance test maximum z-value = %.2f.\n", fit);

    if ((argc > 1) && (0 == strcmp("-d", argv[1]))) {
        fprintf(stderr, "Feeding dieharder tests...\n");
        feed_dieharder();
        /* Never exits */
    }
    fprintf(stderr, "Randomization tests ");
    if (failed) {
        fprintf(stderr, "failed. Code = %d\n", failed);
    } else {
        fprintf(stderr, "passed.\n");
    }
    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
