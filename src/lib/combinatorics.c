/* OneJoker library <https://github.com/lcrocker/OneJoker/>
 * Iterator object functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "onejoker.h"
#include "bctable.h"

long long ojc_binomial(int n, int k) {
    int i, j;
    long long b;
    assert(n >= 0 && k >= 0);

    if (0 == k || n == k) return 1LL;
    if (k > n) return 0LL;

    if (k > (n - k)) k = n - k;
    if (1 == k) return (long long)n;

    if (n <= 54 && k <= 54) {
        return bctable[(((n - 3) * (n - 3)) >> 2) + k];
    }
    /* Last resort: actually calculate */
    b = 1LL;
    for (i = 1; i <= k; ++i) {
        b *= (n - (k - i));
        if (b < 0) return -1LL; /* Overflow */
        b /= i;
    }
    return b;
}

/* Start a new iteration sequence. We need to provide a <deck> sequence that is
 * the set to make combinations of, a mutable <hand> sequence to receive the
 * combinations, <k> for the size of the combinations, and a <k>-sized integer
 * buffer for internal use. If <count> is nonzero, the monte carlo interator
 * will stop after that many iterations.
 */
long long ojc_iter_new(
    oj_iterator_t *iter,    /* Iterator to initialize */
    oj_sequence_t *deck,    /* Universe set of cards */
    oj_sequence_t *hand,    /* Hand to put results into */
    int k,                  /* Cards per hand */
    int *hbuf,              /* Scratch array of <k> integers */
    long long count)        /* Monte carlo limit */
{
    int i;

    assert(0 != iter && 0 != deck && 0 != hand && 0 != hbuf && 0 != k);
    assert(hand->allocation >= k);
    assert(deck->length >= k && deck->length <= 54);

    iter->_johnnymoss = 0x10ACE0FF;
    iter->k = hand->length = k;
    iter->a = hbuf;
    iter->deck = deck;
    iter->hand = hand;
    iter->total = ojc_binomial(deck->length, k);

    if (0 == count) {
        iter->count = iter->total;
    } else {
        iter->count = count;
    }
    iter->remaining = iter->count;

    for (i = 0; i < k; ++i) hbuf[i] = i;
    for (i = 0; i < deck->length; ++i) {
        iter->deck_invert[deck->cards[i]] = i;
    }
    return iter->count;
}

/* Assuming everything is set up properly with iter_new, advance <hand> to the
 * next in colex order. Return 1 if there are more to process. In C, we do the
 * loop like this:
 *   <new>;
 *   while <next> { <stuff> };
 * In Python we call it from a generator.
 */
int ojc_iter_next(oj_iterator_t *iter) {
    int i, j, *a = iter->a, k = iter->k, n = iter->deck->length;
    assert(0 != iter && 0x10ACE0FF == iter->_johnnymoss);

    if (0 == iter->remaining) {
        return 0;
    } else if (iter->remaining != iter->count) {
        for (i = 0; i < k; ++i) {
            if ( ((i < k - 1) && (a[i] < (a[i + 1] - 1))) ||
                  ((i == k - 1) && (a[i] < n - 1)) ) {
                ++a[i];
                for (j = 0; j < i; ++j) a[j] = j;
            }
        }
    }
    for (i = 0; i < k; ++i) {
        iter->hand->cards[i] = iter->deck->cards[a[i]];
    }
    --iter->remaining;
    return 1;
}

#define SWAP(a,b) do{t=dp[a];dp[a]=dp[b];dp[b]=t;ip[dp[a]]=(a);ip[dp[b]]=(b);}while(0)

int ojc_iter_next_random(oj_iterator_t *iter) {
    int i, j, t, k = iter->k, n = iter->deck->length,
        *dp = iter->deck->cards, *ip = iter->deck_invert;
    assert(0 != iter && 0x10ACE0FF == iter->_johnnymoss);

    if (0 == iter->remaining) return 0;

    for (i = 0; i < k; ++i) {
        j = ojr_rand(n - i);
        SWAP(i, i + j);
        iter->hand->cards[i] = dp[i];
    }
    --iter->remaining;
    return 1;
}

/* Once we have set up an interation object with the appropriate values, we can
 * calculate some things without actually running the iterations. "Rank" is the
 * order in which a given hand would appear in the sequence if we ran it. The
 * "hand_at" function does the reverse: which hand would appear at that rank.
 * All of these functions use colexicographical order, because that's the
 * simplest to calculate in most cases.
 */
long long ojc_rank(oj_sequence_t *hand, oj_iterator_t *iter) {
    int i, buf[64];
    long long b, r = 0;
    assert(0 != hand && 0 != iter);
    assert(hand->length < 64);

    if (hand->length != iter->k) return -1;
    for (i = 0; i < iter->k; ++i) {
        buf[i] = iter->deck_invert[hand->cards[i]];
    }
    _ojs_sort_int_array(iter->k, buf);

    for (i = 0; i < iter->k; ++i) {
        r += ojc_binomial(buf[i], i + 1);
    }
    return r;
}

void ojc_hand_at(long long rank, oj_sequence_t *hand, oj_iterator_t *iter) {
    int i, buf[64], v = iter->deck->length;
    long long b;
    assert(0 != hand && 0 != iter);
    assert(hand->allocation >= iter->k);

    for (i = iter->k; i >= 1; --i) {
        while ((b = ojc_binomial(v, i)) > rank) {
            --v;
        }
        buf[i - 1] = v;
        rank -= b;
    }
    hand->length = iter->k;
    for (i = 0; i < iter->k; ++i) {
        hand->cards[i] = iter->deck->cards[buf[i]];
    }
}
