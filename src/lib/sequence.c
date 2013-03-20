/* OneJoker library <http://github.com/lcrocker/OneJoker/>
 * Dealing with card sequences. Generally modelled after the Python list
 * methods of the same name, but with concessions to speed.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "onejoker.h"

/* Initialize an empty sequence object with the given buffer.
 */
int ojs_new(oj_sequence_t *sp, int size, int *buf) {
    assert(0 != sp && 0 != size && 0 != buf);

    sp->_johnnymoss = 0x10ACE0FF;
    sp->allocation = size;
    sp->cards = buf;
    sp->length = 0;
    return size;
}

inline void ojs_clear(oj_sequence_t *sp) {
    sp->length = 0;
}

inline int ojs_length(oj_sequence_t *sp) {
    return sp->length;
}

/* Python doesn't have this one, but it's handy for our uses */
inline void ojs_truncate(oj_sequence_t *sp, int size) {
    if (size < sp->length) sp->length = size;
}

int ojs_append(oj_sequence_t *sp, int card) {
    assert(0 != sp && 0x10ACE0FF == sp->_johnnymoss);
    assert(card > 0 && card <= 54);

    if (sp->length == sp->allocation) return 0;
    sp->cards[sp->length++] = card;
    return sp->length;
}

int ojs_extend(oj_sequence_t *destp, oj_sequence_t *srcp, int count) {
    assert(0 != srcp && 0x10ACE0FF == srcp->_johnnymoss);
    assert(0 != destp && 0x10ACE0FF == destp->_johnnymoss);

    if (0 == count) count = srcp->length;
    if (count > (destp->allocation - destp->length))
        count = (destp->allocation - destp->length);

    memmove(destp->cards + destp->length, srcp->cards, count * sizeof(int));
    destp->length += count;
    return count;
}

int ojs_insert(oj_sequence_t *sp, int index, int card) {
    assert(0 != sp && 0x10ACE0FF == sp->_johnnymoss && index >= 0);
    assert(card > 0 && card <= 54);

    if (sp->length == sp->allocation) return 0;
    memmove(sp->cards + index + 1, sp->cards + index,
        (sp->length - index) * sizeof(int));
    sp->cards[index] = card;
    return ++sp->length;
}

/* Here's a bit of a deviation from the Python model: pop() does not
 * take an index, but that functionality is done with delete(), which
 * returns the value deleted.
 */
int ojs_pop(oj_sequence_t *sp) {
    assert(0 != sp && 0x10ACE0FF == sp->_johnnymoss);

    if (0 == sp->length) return 0;
    return sp->cards[--sp->length];
}

int ojs_delete(oj_sequence_t *sp, int index) {
    int v;
    assert(0 != sp && 0x10ACE0FF == sp->_johnnymoss && index >= 0);

    if (index >= sp->length) return 0;
    v = sp->cards[index];
    --sp->length;
    memmove(sp->cards + index, sp->cards + index + 1,
        (sp->length - index) * sizeof(int));
    return v;
}

int ojs_remove(oj_sequence_t *sp, int card) {
    int i;
    assert(0 != sp && 0x10ACE0FF == sp->_johnnymoss);
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

int ojs_index(oj_sequence_t *sp, int card) {
    int i;
    assert(0 != sp && 0x10ACE0FF == sp->_johnnymoss);
    assert(card > 0 && card <= 54);

    for (i = 0; i < sp->length; ++i) {
        if (card == sp->cards[i]) return i;
    }
    return -1;
}

/* Many applications involve sorting small hands inside a loop, so we go to
 * some effort here to optimize the hell out of those special cases.
 */

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
    assert(0 != sp && 0x10ACE0FF == sp->_johnnymoss);

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
        /* Fall back to normal in-place heapsort */
    }
    for (i = (n - 1) >> 1; i >= 0; --i) heapify(cp, n - 1, i);
    for (i = n - 1; i > 0; --i) {
        SWAP(0, i);
        heapify(cp, i - 1, 0);
    }
}

void ojs_reverse(oj_sequence_t *sp) {
    int i, t, *cp = sp->cards;
    assert(0 != sp && 0x10ACE0FF == sp->_johnnymoss);

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

/* Fill a sequence with fresh cards based on deck type. Can be used to fill
 * multi-deck shoes as well.
 */
int ojs_fill(oj_sequence_t *sp, int count, oj_deck_type_t dt) {
    oj_deck_info_t *di = &oj_deck_info[dt];
    int c, remaining;
    assert(0 != sp && 0x10ACE0FF == sp->_johnnymoss);

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

/* Standard Fisher-Yates shuffle.
 */
void ojs_shuffle(oj_sequence_t *sp) {
    int i, j, t, *cp = sp->cards;
    assert(0 != sp && 0x10ACE0FF == sp->_johnnymoss);

    for (i = sp->length; i > 1; --i) {
        j = ojr_rand(i);
        SWAP(i - 1, j);
    }
}
