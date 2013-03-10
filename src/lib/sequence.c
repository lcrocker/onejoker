/* Dealing with card sequences.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "onejoker.h"

/* Initialize an empty sequence object from with the given buffer.
 */
int oj_seq_new(oj_sequence_t *sp, int size, int *buf) {
    assert(0 != sp);
    assert(0 != size);
    assert(0 != buf);

    sp->_johnnymoss = 0x10ACE0FF;
    sp->allocation = size;
    sp->cards = buf;
    sp->length = 0;
    return size;
}

/* Remove and return a card from the end of the sequence.
 */
int oj_seq_deal_from_end(oj_sequence_t *sp) {
    assert(0 != sp);
    assert(0x10ACE0FF == sp->_johnnymoss);

    if (0 == sp->length) return 0;
    return sp->cards[--sp->length];
}

/* Add a card to the end of the sequence.
 */
int oj_seq_deal_to_end(oj_sequence_t *sp, int card) {
    assert(0 != sp);
    assert(card > 0 && card <= 54);
    assert(0x10ACE0FF == sp->_johnnymoss);

    if (sp->length == sp->allocation) return 0;
    sp->cards[sp->length++] = card;
    return sp->length;
}

/* Remove and return a card from the start of the sequence.
 */
int oj_seq_deal_from_head(oj_sequence_t *sp) {
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
int oj_seq_deal_to_head(oj_sequence_t *sp, int card) {
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
int oj_seq_pick(oj_sequence_t *sp, int card) {
    int i;
    assert(0 != sp);
    assert(0x10ACE0FF == sp->_johnnymoss);
    assert(card > 0 && card <= 54);

    for (i = 0; i < sp->length; ++i) {
        if (card == sp->cards[i]) {
            --sp->length;
            memmove(sp->cards + i, sp->cards + i + 1, sp->length - i);
            return card;
        }
    }
    return 0;
}

/* Move <count> cards from source sequence to the end of dest sequence.
 * Return the number of cards actually moved.
 */
int oj_seq_move(oj_sequence_t *destp, oj_sequence_t *srcp, int count) {
    int i, c, moved = 0;
    assert(0 != srcp);
    assert(0 != destp);
    assert(0x10ACE0FF == srcp->_johnnymoss);
    assert(0x10ACE0FF == destp->_johnnymoss);

    if (count > srcp->length) count = srcp->length;
    for (i = 0; i < count; ++i) {
        if (destp->length == destp->allocation) return moved;
        c = oj_seq_deal_from_end(srcp);
        oj_seq_deal_to_end(destp, c);
        ++moved;
    }
    return moved;
}

/* Copy the whole <srcp> sequence to <destp>, which is overwritten.
 * Return the number of cards moved (which will be less than the
 * length of <srcp> if <destp> doesn't have enough room).
 */
int oj_seq_copy(oj_sequence_t *destp, oj_sequence_t *srcp) {
    int count = srcp->length;
    assert(0 != srcp);
    assert(0 != destp);
    assert(0x10ACE0FF == srcp->_johnnymoss);
    assert(0x10ACE0FF == destp->_johnnymoss);

    if (count > destp->allocation) count = destp->allocation;
    memmove(destp->cards, srcp->cards, count);
    return destp->length = count;
}

/* Fill a sequence with fresh cards based on deck type. Can be used to
 * fill multi-deck shoes as well.
 */
int oj_seq_fill(oj_sequence_t *sp, int count, oj_deck_type_t dt) {
    oj_deck_info_t *di = oj_deck_info(dt);
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
}

int oj_seq_shuffle(oj_sequence_t *sp) {
}
