/* This obsolete code builds an 8MB lookup table for a sort-and-hash hand
 * evaluator I was experimenting with. Turns out to be about half as fast as
 * the Suffecool-Senzee one. But it was a great way to test parts of the code
 * and learn some tricks, so it's left here for posterity.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "onejoker.h"

#define UNIQUEHANDS 2598960
#define TABLESIZE 0x400000
/* 0x20000000 */
#define MODTABLESIZE(x) ((x)&(TABLESIZE-1))
#define MAXDEPTH 20

typedef struct _bucket {
	uint32_t key;
	int next;
	short depth;
	short val;
} bucket_t;

bucket_t g_buckets[UNIQUEHANDS];
int32_t g_chains[TABLESIZE];

/* Given an array of exactly 5 ints with card numbers (1..52),
 * sort and pack them into a 32-bit integer key.
 */
#define SWAP(a,b) do{if(h[a]>h[b]){t=h[a];h[a]=h[b];h[b]=t;}}while(0)

uint32_t build_key(int *h) {
	register int t;
	SWAP(0,1);  SWAP(3,4);  SWAP(2,4);
	SWAP(2,3);  SWAP(1,4);  SWAP(0,3);
	SWAP(0,2);  SWAP(1,3);  SWAP(1,2);
	return (h[0]<<24)|(h[1]<<18)|(h[2]<<12)|(h[3]<<6)|(h[4]);
}

/* Hash functions.
 */

static uint32_t g_x = 0x76b24ce9, g_y = 0x5e2a572d, g_z = 0x99c52d8c;

uint32_t hf1(uint32_t key) {
	key *= g_x;
	key += g_y;
	return MODTABLESIZE(key>>8);
}

uint32_t hf2(uint32_t key, uint16_t sk) {
	key *= (g_y + sk);
	key += g_z;
	return MODTABLESIZE(key>>8);
}

/* Verify the bucket and chain structures.
 */
int buckets_are_valid(void) {
	int b, h, d, n, groups[10];
	long t;

	/* Bucket table should be full, with one entry for each hand.
	 */
	memset(groups, 0, sizeof(groups));
	for (b = 0; b < UNIQUEHANDS; ++b) {
		if (g_buckets[b].val < 1 || g_buckets[b].val > 7462) return 0;
		if (g_buckets[b].depth > MAXDEPTH) return 0;
		if (0 == g_buckets[b].key) return 0;
		++groups[oj_poker_handgroup(g_buckets[b].val)];
	}
	assert(1302540 == groups[1]);
	assert(1098240 == groups[2]);
	assert(123552 == groups[3]);
	assert(54912 == groups[4]);
	assert(10200 == groups[5]);
	assert(5108 == groups[6]);
	assert(3744 == groups[7]);
	assert(624 == groups[8]);
	assert(40 == groups[9]);

	t = 0;
	for (h = 0; h < TABLESIZE; ++h) {
		b = g_chains[h];
		if (b <= 0) continue;
		assert(b <= UNIQUEHANDS);
		--b;

		d = g_buckets[b].depth;
		assert(d > 0 && d <= MAXDEPTH);

		while (d > 1) {
			++t;

			n = g_buckets[b].next;
			assert(n >= 0 && n < UNIQUEHANDS);
			assert(d == g_buckets[n].depth + 1);
			b = n;
			--d;
		}
		assert(-1 == g_buckets[b].next);
		++t;
	}
	assert(UNIQUEHANDS == t);
	return 1;
}

/* First pass, enumerate all the hands, fill buckets and chains.
 */
int g_depth;

long enumerate_hands(void) {
	int h[8], d, b;
	long totalhands = 0;
	int collisions = 0;
	uint32_t key, hash1;

	memset(g_chains, 0, sizeof(g_chains));
	memset(g_buckets, 0, sizeof(g_buckets));
	g_depth = b = 0;

	for (h[0] = 1; h[0] <= 48; ++h[0]) {
		for (h[1] = h[0] + 1; h[1] <= 49; ++h[1]) {
			for (h[2] = h[1] + 1; h[2] <= 50; ++h[2]) {
				for (h[3] = h[2] + 1; h[3] <= 51; ++h[3]) {
					for (h[4] = h[3] + 1; h[4] <= 52; ++h[4]) {
						++totalhands;
						if (0 == (totalhands & 0x3FFFF)) {
							fprintf(stderr, "%ld\r", totalhands);
							fflush(stderr);
						}
						key = build_key(h);
						hash1 = hf1(key);

						g_buckets[b].next = -1;
						g_buckets[b].depth = 1;
						g_buckets[b].val = oj_poker_eval5(h[0], h[1], h[2], h[3], h[4]);
						g_buckets[b].key = key;

						if (g_chains[hash1]) {
							++collisions;
							d = g_buckets[g_chains[hash1] - 1].depth + 1;
							if (d > g_depth) g_depth = d;

							if (d > MAXDEPTH) {
								fprintf(stderr, "Max bucket size (%d) exceeded on hand %ld.\n",
									MAXDEPTH, totalhands);
								return 0;
							}
							g_buckets[b].next = g_chains[hash1] - 1;
							g_buckets[b].depth = d;
						}
						g_chains[hash1] = b + 1;
						++b;
					}
				}
			}
		}
	}
	fprintf(stderr, "Pass 1: %ld hands, %d collisions, depth %d.\n",
		totalhands, collisions, g_depth);
	return totalhands;
}

void _debug(uint32_t h1) {
	int b, d, i, sk, newhashes[MAXDEPTH];
	uint32_t h2;

	for (sk = 129; sk < 135; ++sk) {
		b = g_chains[h1] - 1;
		d = 0;

		do {
			h2 = hf2(g_buckets[b].key, sk);
			newhashes[d++] = h2;
			b = g_buckets[b].next;
		} while (-1 != b);

		fprintf(stderr, "\n!%3d ", sk);
		for (i = 0; i < d; ++i) {
			fprintf(stderr, "%9u ", newhashes[i]);
		}
		fprintf(stderr, "\n");
		fflush(stderr);
	}
}

/* Search for a secondary key that will rehash the given chain to empty
 * (and different! -- that was a fun bug to find) buckets. I arbitrarily
 * throw away the first 128 sks to make the hashes better (and therefore
 * presumably likely to succeed earlier), but there's really no reason not
 * to try them all.
 */
uint16_t find_sk(uint32_t h1) {
	int h2, sk, b, d, i, j, dup, newhashes[MAXDEPTH];

	for (sk = 129; sk < 0x8000; ++sk) {
		b = g_chains[h1] - 1;
		d = 0;

		do {
			assert(d < MAXDEPTH);
			h2 = hf2(g_buckets[b].key, sk);
			if (0 != g_chains[h2]) break;

			dup = 0;
			for (i = 0; i < d; ++i) {
				if (h2 == newhashes[i]) dup = 1;
			}
			if (dup) break;

			newhashes[d++] = h2;
			b = g_buckets[b].next;
		} while (-1 != b);
		if (-1 == b) return sk; /* Success */
	}
	return 0;
}

/* Rehash the chain using the given secondary key.
 */
void rehash(int h1, uint16_t sk) {
	int h2, b, n, d;

	b = g_chains[h1] - 1;
	d = g_buckets[b].depth;

	do {
		--d;
		assert(d >= 0);

		h2 = hf2(g_buckets[b].key, sk);
		assert(0 == g_chains[h2]);
		g_chains[h2] = b + 1;

		n = g_buckets[b].next;
		g_buckets[b].depth = 1;
		g_buckets[b].next = -1;
		b = n;
	} while (-1 != b);

	g_chains[h1] = -sk;
}

/* Starting with the deepest buckets, make multiple passes over the
 * chains attempting to rehash.
 */
int rehash_all(void) {
	int b, d, h1, h2, sk, chains = 0, collisions = 0;

	fprintf(stderr, "Depth: ");
	for (d = g_depth; d > 1; --d) {
		fprintf(stderr, "%d ", d);
		fflush(stderr);

		for (h1 = 0; h1 < TABLESIZE; ++h1) {
			if (0 == (b = g_chains[h1])) continue;
			if (d != g_buckets[--b].depth) continue;

			++chains;
			sk = find_sk(h1);
			if (0 == sk) {
				fprintf(stderr, "\nPass 2: failed rehash at %d:%u.\n", d, h1);
				return 0;
			}
			rehash(h1, sk);
			collisions += d - 1;
		}
	}
	fprintf(stderr, "\nPass 2: %d chains, %d collisions resolved.\n",
		chains, collisions);
	return 1;
}

int build_table(void) {
	int b, c;
	long totalhands = 0, pointers = 0, empties = 0;
	uint32_t h1;
	uint16_t v;

	fprintf(stderr, "Pass 3: Writing table...\n");
	printf("static uint16_t _oj_lc_table1[] = {");
	for (h1 = 0; h1 < TABLESIZE; ++h1) {
		if (0 == (h1 % 12)) {
			printf("\n    ");
		}
		c = g_chains[h1];
		if (0 == c) {
			v = 0;
			++empties;
		} else if (c < 0) {
			v = 0x8000 | (-c);
			++pointers;
		} else {
			v = g_buckets[c - 1].val;
			++totalhands;
		}
		printf("%5u, ", v);
	}
	printf("\n};\n");

	fprintf(stderr, "Pass 3: %ld values, %ld pointers, %ld empty cells.\n",
		totalhands, pointers, empties);
	return totalhands;
}

int main(int argc, char *argv[]) {
	int i, r;
	long t;

	fprintf(stderr, "Keys: x=%08x y=%08x z=%08x\n", g_x, g_y, g_z);

	t = enumerate_hands();
	assert(UNIQUEHANDS == t);
	assert(buckets_are_valid());

	r = rehash_all();
	assert(0 != r);
	assert(buckets_are_valid());

	t = build_table();
	assert(UNIQUEHANDS == t);
	fprintf(stderr, "Done.\n");
}
