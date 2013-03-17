/* Dealing with card sequences.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "onejoker.h"

/* Initialize an empty sequence object from with the given buffer.
 */
int ojs_new(oj_sequence_t *sp, int size, int *buf) {
    assert(0 != sp && 0 != size && 0 != buf);

    sp->_johnnymoss = 0x10ACE0FF;
    sp->allocation = size;
    sp->cards = buf;
    sp->length = 0;
    return size;
}

/* Remove all cards.
 */
void ojs_clear(oj_sequence_t *sp) {
    sp->length = 0;
}

/* Add a card to the end of the sequence.
 */
int ojs_append(oj_sequence_t *sp, int card) {
    assert(0 != sp && card > 0 && card <= 54);
    assert(0x10ACE0FF == sp->_johnnymoss);

    if (sp->length == sp->allocation) return 0;
    sp->cards[sp->length++] = card;
    return sp->length;
}

/* Copy all cards from source sequence to the end of dest sequence.
 */
int ojs_extend(oj_sequence_t *destp, oj_sequence_t *srcp) {
    assert(0 != srcp && 0 != destp);
    assert(0x10ACE0FF == srcp->_johnnymoss);
    assert(0x10ACE0FF == destp->_johnnymoss);

    count = srcp->length;
    if (count > (destp->allocation - destp->length))
        count = (destp->allocation - destp->length);

    memmove(destp->cards + destp->length, srcp->cards, count * sizeof(int));
    destp->length += count;
    return count;
}

int ojs_delete(oj_sequence_t *sp, int index) {
}

int ojs_insert(oj_sequence_t *sp, int index, int card) {
}

/* Remove and return a card from the end of the sequence.
 */
int ojs_deal_from(oj_sequence_t *sp) {
    assert(0 != sp);
    assert(0x10ACE0FF == sp->_johnnymoss);

    if (0 == sp->length) return 0;
    return sp->cards[--sp->length];
}


/* Remove and return a card from the start of the sequence.
 */
int ojs_deal_from_head(oj_sequence_t *sp) {
    int c;
    assert(0 != sp);
    assert(0x10ACE0FF == sp->_johnnymoss);

    if (0 == sp->length) return 0;
    c = *sp->cards;
    memmove(sp->cards, sp->cards + 1, (--sp->length) * sizeof(int));
    return c;
}

/* Insert card at the start of the sequence.
 */
int ojs_deal_to_head(oj_sequence_t *sp, int card) {
    assert(0 != sp);
    assert(card > 0 && card <= 54);
    assert(0x10ACE0FF == sp->_johnnymoss);

    if (sp->length == sp->allocation) return 0;
    memmove(sp->cards + 1, sp->cards, sp->length * sizeof(int));
    *sp->cards = card;
    return ++sp->length;
}

/* Find the given card in the sequence, remove and return it if
 * found, otherwise return 0.
 */
int ojs_pick(oj_sequence_t *sp, int card) {
    int i;
    assert(0 != sp);
    assert(0x10ACE0FF == sp->_johnnymoss);
    assert(card > 0 && card <= 54);

    for (i = 0; i < sp->length; ++i) {
        if (card == sp->cards[i]) {
            if (i != --sp->length) {
                memmove(sp->cards + i, sp->cards + i + 1,
                    (sp->length - i) * sizeof(int));
            }
            return card;
        }
    }
    return 0;
}

/* Copy <count> cards from end of source sequence to the end of dest
 * sequence without removing them from source. Return the number of cards
 * actually moved.
 */
int ojs_copy(oj_sequence_t *destp, oj_sequence_t *srcp, int count) {
    assert(0 != srcp);
    assert(0 != destp);
    assert(0x10ACE0FF == srcp->_johnnymoss);
    assert(0x10ACE0FF == destp->_johnnymoss);

    if (count > srcp->length) count = srcp->length;
    if (count > (destp->allocation - destp->length))
        count = (destp->allocation - destp->length);

    memmove( destp->cards + destp->length,
        srcp->cards + (srcp->length - count),
        count * sizeof(int) );
    destp->length += count;
    return count;
}

/* Move <count> cards from end of source sequence to the end of dest
 * sequence. Return the number of cards actually moved.
 */
int ojs_move(oj_sequence_t *destp, oj_sequence_t *srcp, int count) {
    int c = ojs_copy(destp, srcp, count);
    srcp->length -= c;
    return c;
}

/* Copy the whole <srcp> sequence to <destp>, which is overwritten.
 * Return the number of cards moved (which will be less than the
 * length of <srcp> if <destp> doesn't have enough room).
 */
int ojs_copy_all(oj_sequence_t *destp, oj_sequence_t *srcp) {
    int count = srcp->length;
    assert(0 != srcp);
    assert(0 != destp);
    assert(0x10ACE0FF == srcp->_johnnymoss);
    assert(0x10ACE0FF == destp->_johnnymoss);

    if (count > destp->allocation) count = destp->allocation;
    memmove(destp->cards, srcp->cards, count * sizeof(int));
    return destp->length = count;
}

int ojs_truncate(oj_sequence_t *sp, int size) {
    if (size < sp->length) sp->length = size;
    return sp->length;
}

/* Fill a sequence with fresh cards based on deck type. Can be used to
 * fill multi-deck shoes as well.
 */
int ojs_fill(oj_sequence_t *sp, int count, oj_deck_type_t dt) {
    oj_deck_info_t *di = &oj_deck_info[dt];
    int c, remaining;
    assert(0 != sp);
    assert(0x10ACE0FF == sp->_johnnymoss);

    sp->length = 0;
    remaining = count;
    if (remaining > sp->allocation) remaining = sp->allocation;

    do {
        c = remaining;
        if (c > di->size) c = di->size;
        memmove(sp->cards + sp->length, di->cards, c * sizeof(int));

        sp->length += c;
        remaining -= c;
    } while (remaining);

    return sp->length;
}

#define SWAP(a,b) do{t=cp[a];cp[a]=cp[b];cp[b]=t;}while(0)
#define CMP(a,b) (cp[a]>cp[b])
#define CSWP(a,b) do{s=cp[a]+cp[b];d=abs(cp[a]-cp[b]);cp[a]=(s-d)>>1;cp[b]=(s+d)>>1;}while(0)

static void heapify(int *cp, int n, int start) {
    int t, lc, rc, head;
    int last = ((n + 1) >> 1);

    while (start < last) {
        lc = (start << 1) + 1;
        rc = lc + 1;

        if (rc <= n) {
            if (CMP(rc, lc)) head = rc;
            else head = lc;
        } else head = lc;

        if (CMP(head, start)) {
            SWAP(head, start);
            start = head;
        } else break;
    }
}

void ojs_sort(oj_sequence_t *sp) {
    int i, s, d, t;
    int n = sp->length, *cp = sp->cards;
    assert(0 != sp);
    assert(0x10ACE0FF == sp->_johnnymoss);

    switch (n) {
    case 0:
    case 1:
        return;
    case 2:
        CSWP(0, 1);
        return;
    case 3:
        CSWP(1, 2); CSWP(0, 2); CSWP(0, 1);
        return;
    case 4:
        CSWP(0, 1); CSWP(2, 3); CSWP(0, 2); CSWP(1, 3); CSWP(1, 2);
        return;
    case 5:
        CSWP(0, 1); CSWP(3, 4); CSWP(2, 4); CSWP(2, 3); CSWP(0, 3);
        CSWP(0, 2); CSWP(1, 4); CSWP(1, 3); CSWP(1, 2);
        return;
    default:
        break;
        /* Fall back to in-place heapsort */
    }
    for (i = (n - 1) >> 1; i >= 0; --i) heapify(cp, n - 1, i);
    for (i = n - 1; i > 0; --i) {
        SWAP(0, i);
        heapify(cp, i - 1, 0);
    }
}

/* Standard Fisher-Yates shuffle.
 */
void ojs_shuffle(oj_sequence_t *sp) {
    int i, j, t, *cp = sp->cards;
    assert(0 != sp);

    if (0 == sp->length) return;
    for (i = sp->length; i > 1; --i) {
        j = ojr_rand(i);
        SWAP(i - 1, j);
    }
}

void ojs_reverse(oj_sequence_t *sp) {
    int i, t, *cp = sp->cards;
    assert(0 != sp);

    for (i = 0; i < (sp->length >> 1); ++i) {
        SWAP(i, (sp->length - 1) - i);
    }
}

int ojs_equal(oj_sequence_t *sp1, oj_sequence_t *sp2) {
    int i;

    if (sp1->length != sp2->length) return 0;
    for (i = 0; i < sp1->length; ++i) {
        if (sp1->cards[i] != sp2->cards[i]) return 0;
    }
    return 1;
}
