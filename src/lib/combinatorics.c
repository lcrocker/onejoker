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
        return bc_table[(((n - 3) * (n - 3)) >> 2) + (k - 2)];
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

/* Start a new iteration sequence. We need to provide a <deck>
 * sequence that is the set to make combinations of, a mutable <hand>
 * to receive the combinations, <k> for the size of the combinations,
 * and a <k>-sized integer buffer for internal use.
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

    for (i = 0; i < deck->length; ++i) {
        iter->deck_invert[deck->cards[i]] = i;
    }

    if (count) {
        iter->remaining = count + 1;
        ojc_iter_next_random(iter);
    } else {
        iter->remaining = iter->total;
        for (i = 0; i < k; ++i) {
            hbuf[i] = i;
            hand->cards[i] = deck->cards[hbuf[i]];
        }
    }
    return iter->total;
}

/* Assuming everything is set up properly with iter_new, advance
 * hand to the next in colex order. Return 1 if successful. In C, we
 * do the loop with <new>, do { <stuff> } while (<next>); In Python we
 * call it from a generator.
 */
int ojc_iter_next(oj_iterator_t *iter) {
    int i, j, *a = iter->a, k = iter->k, n = iter->deck->length,
        *cp = iter->hand->cards;
    assert(0 != iter && 0x10ACE0FF == iter->_johnnymoss);

    for (i = 0; i < k; ++i) {
        if ( ((i < k - 1) && (a[i] < (a[i + 1] - 1))) ||
             ((i == k - 1) && (a[i] < n - 1)) ) {
            ++a[i];
            for (j = 0; j < i; ++j) a[j] = j;

            for (i = 0; i < k; ++i) {
                cp[i] = iter->deck->cards[a[i]];
            }
            --iter->remaining;
            return 1;
        }
    }
    return 0;
}

#define SWAP(a,b) do{t=cp[a];cp[a]=cp[b];cp[b]=t;}while(0)

int ojc_iter_next_random(oj_iterator_t *iter) {
    int i, j, t, k = iter->k, n = iter->deck->length,
        *cp = iter->deck->cards;
    assert(0 != iter && 0x10ACE0FF == iter->_johnnymoss);

    for (i = n; i > (n - k); --i) {
        j = ojr_rand(i);
        SWAP(i - 1, j);
    }
    memmove(iter->hand->cards, &cp[n - k], k * sizeof(int));
    if (0 == --iter->remaining) return 0;
    return 1;
}

long long ojc_colex_rank(oj_sequence_t *hand, oj_iterator_t *iter) {
}

int ojc_colex_hand_at(oj_sequence_t *hand, oj_iterator_t *iter, long long rank) {
}
