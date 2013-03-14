/* Pseudo-random number generator for OneJoker library.  Based on the public
 * domain JKISS by David Jones
 * <http://www.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf>.
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
static int _seeded = 0;

/* State vector and pointer. We calculate a bufferfull of random bits
 * at a time, and fetch them from the buffer, refilling when needed.
 */
#define STATE_SIZE 512
static uint16_t *sptr, state[2 * STATE_SIZE];

int ojr_seed(int seed) {
    int fn;
    uint32_t s;
    time_t t;

    sptr = state;

    /* Start with some reasonable defaults */
    x = 123456789;
    y = 987654321;
    z = 43219876;
    c = 6543217;

    /* If we were passed a nonzero seed, mix those bits in with the
     * defaults to get a repeatable sequence.
     */
    if (0 != seed) {
        x ^= (0x5A5A5A5A & seed);
        z ^= (0xA5A5A5A5 & seed);
        _seeded = 1;
        return 0;
    }
    /* Fetch all 128 bits of seed from system randomness.
     */
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

    /* Fall back to using time()
     */
    time(&t);
    x ^= (0x5A5A5A5A & t);
    z ^= (0xA5A5A5A5 & t);

    _seeded = 1;
    return 0;
}

/* Need more random bits.
 */
static void reload(void) {
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

/* Return next 16 random bits from buffer.
 */
uint16_t ojr_next16(void) {
    assert(_seeded);
    if (sptr == state) reload();
    return *--sptr;
}

/* Return next 32 random bits from buffer.
 */
uint32_t ojr_next32(void) {
    assert(_seeded);
    if (sptr < (state + 2)) reload();
    sptr -= 2;
    return *((uint32_t *)sptr);
}

/* Return a well-balanced random integer from 0 to limit-1.
 * Limited to 16 bits!
 */
int ojr_rand(int limit) {
    int v, m = limit - 1;
    assert(_seeded);
    assert(limit > 1);
    assert(limit < 65536);

    m |= m >> 1;
    m |= m >> 2;
    m |= m >> 4;
    m |= m >> 8;

    do {
        v = ojr_next16() & m;
    } while (v >= limit);
    return v;
}
