
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "onejoker.h"

long enumerate(long *groups) {
	int c1, c2, c3, c4, c5;
	int i, val, cat;
	long totalhands = 0;

	memset(groups, 0, 10 * sizeof(long));

	for (c1 = 1; c1 <= 52; ++c1) {
		for (c2 = 1; c2 <= 52; ++c2) {
			if (c2 == c1) continue;
			for (c3 = 1; c3 <= 52; ++c3) {
				if (c3 == c1 || c3 == c2) continue;
				for (c4 = 1; c4 <= 52; ++c4) {
					if (c4 == c1 || c4 == c2 || c4 == c3) continue;
					for (c5 = 1; c5 <= 52; ++c5) {
						if (c5 == c1 || c5 == c2 || c5 == c3 || c5 == c4) continue;
						++totalhands;
						if (0 == (totalhands & 0x3FFFFF)) {
							fprintf(stderr, "%ld\r", totalhands);
						}

						val = oj_poker_eval5(c1, c2, c3, c4, c5);
						cat = oj_poker_handgroup(val);
						++groups[cat];
					}
				}
			}
		}
	}
	return totalhands;
}

int main(int argc, char *argv[]) {
	int i;
	long t, groups[10];

	t = enumerate(groups);
	assert(311875200 == t);
	assert(156304800 == groups[1]);
	assert(131788800 == groups[2]);
	assert(14826240 == groups[3]);
	assert(6589440 == groups[4]);
	assert(1224000 == groups[5]);
	assert(612960 == groups[6]);
	assert(449280 == groups[7]);
	assert(74880 == groups[8]);
	assert(4800 == groups[9]);

	fprintf(stderr, "Total hands: %ld\n", t);
	for (i = 1; i < 10; ++i) {
		fprintf(stderr, "%ld %s\n", groups[i], oj_poker_handname(i));
	}

	return EXIT_SUCCESS;
}
