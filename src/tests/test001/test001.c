#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "onejoker.h"

#define BUCKETS 13
#define TOTAL_RANDS 10000000

static long buckets[BUCKETS];

/* This program never exits! It runs the balance test first, then continuously
 * feeds random bits to stdout, which can be piped into "dieharder -a -g 200"
 * to verify the PRNG.
 */

int main(int argc, char *argv[]) {
    int i, r;
    long min, max;
    uint16_t buf[100];

    oj_seed_prng(1234);
    fprintf(stderr, "Filling buckets...\n");

    for (i = 0; i < TOTAL_RANDS; ++i) {
        r = oj_rand(BUCKETS);
    	++buckets[r];
    }
    max = 0;
    min = TOTAL_RANDS;
    for (i = 0; i < BUCKETS; ++i) {
    	fprintf(stderr, "%3d %7ld\n", i, buckets[i]);
    	if (buckets[i] < min) min = buckets[i];
    	if (buckets[i] > max) max = buckets[i];
    }
    fprintf(stderr, "Spread: %g\n", (double)(max - min) / (double)TOTAL_RANDS);
    fprintf(stderr, "Feeding dieharder tests...\n");

	while (0) {
		for (i = 0; i < 100; ++i) {
			buf[i] = oj_prng_next16();
		}
		write(STDOUT_FILENO, buf, sizeof(buf));
	}
}
