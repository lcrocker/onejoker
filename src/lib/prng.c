/*
 *  Created on: Feb 27, 2013
 *      Author: lee
 *
 * Pseudo-random number generator for OneJoker library.  Based on the public domain
 * JKISS by David Jones <http://www.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#include "onejoker.h"

/* Seed variables */
static uint32_t x, y, z, c;
static short _seeded = 0;

/* State vector and pointer */
#define STATE_SIZE 512
static uint16_t *sptr, state[2 * STATE_SIZE];

int oj_seed_prng(int seed) {
	int fn;
	uint32_t s;
	time_t t;

    sptr = state;

    /* Start with some reasonable defaults */
	x = 123456789;
	y = 987654321;
	z = 43219876;
	c = 6543217;

    if (0 != seed) {
    	x ^= (0x5A5A5A5A & seed);
    	z ^= (0xA5A5A5A5 & seed);
        _seeded = 1;
    	return 0;
    }
    fn = open("/dev/urandom", O_RDONLY);
    do {
    	if (-1 == fn) break;
    	if (4 != read(fn, &s, 4)) break;
    	x = s;
    	if (4 != read(fn, &s, 4)) break;
    	if (0 != s) y = s;
    	if (4 != read(fn, &s, 4)) break;
    	z = s;
    	if (4 != read(fn, &s, 4)) break;
    	c = s % 698769068 + 1;

    	close(fn);
        _seeded = 1;
    	return 0;
    } while (0);

    /* Fall back to using time() */
    time(&t);
	x ^= (0x5A5A5A5A & t);
	z ^= (0xA5A5A5A5 & t);
    _seeded = 1;
    return 0;
}

static void _oj_prng_reload(void) {
	int i;
	uint64_t t;

    assert(_seeded);
	for (i = 0; i < STATE_SIZE; ++i) {
		x = 314527869 * x + 1234567;
		y ^= y << 5;
		y ^= y >> 7;
		y ^= y << 22;
		t = 4294584393ull * z + c;
		c = t >> 32;
		z = t;
		((uint32_t *)state)[i] = x + y + z;
	}
	sptr = state + 2 * STATE_SIZE;
}

uint16_t oj_prng_next16(void) {
    assert(_seeded);
	if (sptr == state) _oj_prng_reload();
	return *--sptr;
}

uint32_t oj_prng_next32(void) {
    assert(_seeded);
	if (sptr < (state + 2)) _oj_prng_reload();
	sptr -= 2;
	return *((uint32_t *)sptr);
}

/*
 * Return a well-balanced random integer from 0 to limit-1.
 * Limited to 16 bits!
 */
int oj_rand(int limit) {
	int v, m = limit - 1;
    assert(_seeded);
	assert(limit > 1);
	assert(limit < 65536);

    m |= m >> 1;
    m |= m >> 2;
    m |= m >> 4;
    m |= m >> 8;

    do {
        v = oj_prng_next16() & m;
    } while (v >= limit);
    return v;
}
