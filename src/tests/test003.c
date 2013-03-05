/* Build the lookup table for direct-hash poker hand evaluator. The version
 * here is the final one that actually builds the table from the given hash
 * functions and seeds. A hacked-up version of this program was also used to
 * test various hash functions and find good seeds.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "onejoker.h"

/* On some machines the table lookup will be faster, but on most modern
 * 64-bit machines just doing the shifts should be better.  Uncomment
 * whichever is faster on your machine.
 */

#define MB(c) (1ull<<(c))
/*
_static uint64_t _masks[] = { 0,
	1ull <<  1, 1ull <<  2, 1ull <<  3, 1ull <<  4,
	1ull <<  5, 1ull <<  6, 1ull <<  7, 1ull <<  8,
	1ull <<  9, 1ull << 10, 1ull << 11, 1ull << 12,
	1ull << 13, 1ull << 14, 1ull << 15, 1ull << 16,
	1ull << 17, 1ull << 18, 1ull << 19, 1ull << 20,
	1ull << 21, 1ull << 22, 1ull << 23, 1ull << 24,
	1ull << 25, 1ull << 26, 1ull << 27, 1ull << 28,
	1ull << 29, 1ull << 30, 1ull << 31, 1ull << 32,
	1ull << 33, 1ull << 34, 1ull << 35, 1ull << 36,
	1ull << 37, 1ull << 38, 1ull << 39, 1ull << 40,
	1ull << 41, 1ull << 42, 1ull << 43, 1ull << 44,
	1ull << 45, 1ull << 46, 1ull << 47, 1ull << 48,
	1ull << 49, 1ull << 50, 1ull << 51, 1ull << 52,
	1ull << 53, 1ull << 54
};
#define MB(c) (_masks[c])
*/
#define MAKEMASK(a,b,c,d,e) (MB(a)|MB(b)|MB(c)|MB(d)|MB(e))

#define UNIQUEHANDS 2598960
#define TABLESIZE 0x400000
#define MODTABLESIZE(x) ((x>>10)&0x3FFFFF)
#define MAXDEPTH 20

typedef struct _bucket {
	int next;
	short depth;
	short val;
	uint64_t mask;
	/*
	uint32_t hash1;
	uint32_t hash2;
	*/
} bucket_t;

bucket_t g_buckets[UNIQUEHANDS];
int g_chains[TABLESIZE];

/* Seed variables.
 */
uint32_t g_h = 0, g_a = 0, g_b = 0, g_c = 0;

/* Primary hash function. Hash 64-bit mask down to 22-bit table size.
 */
uint32_t hf1(uint64_t *mask) {
	uint32_t u = g_h;

	u ^= *((uint32_t *)mask);
	u *= (g_a ^ ((uint32_t *)mask)[1]);
	u += g_b;
	return MODTABLESIZE(u);
}

/* Secondary hash.
 */
uint32_t hf2(uint64_t *mask, uint32_t h1, uint16_t sk) {
	uint32_t u = h1;

	u ^= *((uint32_t *)mask);
	u *= (g_c ^ ((uint32_t *)mask)[1]);
	u += (sk << 16);
	return MODTABLESIZE(u);
}

/* Verify the bucket and chain structures.
 */
int buckets_are_valid(void) {
	int b, h, d, n, groups[10];
	long t;
	uint64_t mask1, mask2;

	/* Bucket table should be full, with one entry for each hand.
	 */
	mask1 = mask2 = 0ull;
	memset(groups, 0, sizeof(groups));

	for (b = 0; b < UNIQUEHANDS; ++b) {
		if (g_buckets[b].val < 1 || g_buckets[b].val > 7462) return 0;
		if (g_buckets[b].depth > MAXDEPTH) return 0;
		if (0ull == g_buckets[b].mask) return 0;
		mask1 |= g_buckets[b].mask;
		mask2 ^= g_buckets[b].mask;
		++groups[oj_poker_handgroup(g_buckets[b].val)];
	}
	assert(0x1FFFFFFFFFFFFEull == mask1);
	assert(0ull == mask2);
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
	mask1 = mask2 = 0ull;
	for (h = 0; h < TABLESIZE; ++h) {
		b = g_chains[h];
		if (b <= 0) continue;
		assert(b <= UNIQUEHANDS);
		--b;

		d = g_buckets[b].depth;
		assert(d > 0 && d <= MAXDEPTH);

		while (d > 1) {
			mask1 |= g_buckets[b].mask;
			mask2 ^= g_buckets[b].mask;
			++t;

			n = g_buckets[b].next;
			assert(n >= 0 && n < UNIQUEHANDS);
			assert(d == g_buckets[n].depth + 1);
			b = n;
			--d;
		}
		assert(-1 == g_buckets[b].next);
		mask1 |= g_buckets[b].mask;
		mask2 ^= g_buckets[b].mask;
		++t;
	}
	assert(UNIQUEHANDS == t);
	assert(0x1FFFFFFFFFFFFEull == mask1);
	assert(0ull == mask2);

	return 1;
}

/* First pass, enumerate all the hands, fill buckets and chains.
 */
int g_depth;

long enumerate_hands(void) {
	int c1, c2, c3, c4, c5, d, b;
	long totalhands = 0;
	int collisions = 0;
	uint64_t cardmask;
	uint32_t hash1;

	memset(g_chains, 0, sizeof(g_chains));
	memset(g_buckets, 0, sizeof(g_buckets));
	g_depth = b = 0;

	for (c1 = 1; c1 <= 48; ++c1) {
		for (c2 = c1 + 1; c2 <= 49; ++c2) {
			for (c3 = c2 + 1; c3 <= 50; ++c3) {
				for (c4 = c3 + 1; c4 <= 51; ++c4) {
					for (c5 = c4 + 1; c5 <= 52; ++c5) {
						++totalhands;
						if (0 == (totalhands & 0x3FFFF)) {
							fprintf(stderr, "%ld\r", totalhands);
							fflush(stderr);
						}
						cardmask = MAKEMASK(c1, c2, c3, c4, c5);
						hash1 = hf1(&cardmask);

						g_buckets[b].next = -1;
						g_buckets[b].depth = 1;
						g_buckets[b].val = oj_poker_eval5(c1, c2, c3, c4, c5);
						g_buckets[b].mask = cardmask;

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

/* Search for a secondary key that will rehash the given chain to empty
 * (and different! -- that was a fun bug to find) buckets. I arbitrarily
 * throw away the first 128 sks to make the hashes better (and therefore
 * presumably likely to succeed earlier), but there's really no reason not
 * to try them all.
 */
void _debug(uint32_t h1) {
	int b, d, i, sk, newhashes[MAXDEPTH];
	uint32_t h2;

	for (sk = 129; sk < 135; ++sk) {
		b = g_chains[h1] - 1;
		d = 0;

		do {
			h2 = hf2(&g_buckets[b].mask, h1, sk);
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

uint16_t find_sk(uint32_t h1) {
	int h2, sk, b, d, i, j, dup, newhashes[MAXDEPTH];

	for (sk = 129; sk < 0x8000; ++sk) {
		b = g_chains[h1] - 1;
		d = 0;

		do {
			assert(d < MAXDEPTH);
			h2 = hf2(&g_buckets[b].mask, h1, sk);
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

		h2 = hf2(&g_buckets[b].mask, h1, sk);
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
}

/* Starting with the deepest buckets, make multiple passes over the
 * chains attempting to rehash.
 */
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
			v = c;
			++totalhands;
		}
		printf("0x%04x, ", v);
	}
	printf("\n};\n");

	fprintf(stderr, "Pass 3: %ld values, %ld pointers, %ld empty cells.\n",
		totalhands, pointers, empties);
	return totalhands;
}

int main(int argc, char *argv[]) {
	int i, r;
	long t;
	uint32_t u;

	g_h = 0x76b24ce9;
	g_a = 0x5e2a572f;
	g_b = 0x99c52d8c;
	g_c = 0xbea51fe5;

	fprintf(stderr, "Keys: h=%08x a=%08x b=%08x c=%08x\n", g_h, g_a, g_b, g_c);

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
