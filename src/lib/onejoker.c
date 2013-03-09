#include <stdlib.h>
#include <stdio.h>

#include "onejoker.h"

int _oj_johnnymoss = 0;

/* Initialization and finalization for library.  This is the Linux way.
 * On other OSs you'll have to arrange for some way to make sure that
 * _oj_init_library() gets called.
 */
void _init(void) {
	int r;

	r = oj_init_library(0);
	if (r) {
		exit(EXIT_FAILURE);
	}
	return;
}

void _fini(void) {
	return;
}

int oj_init_library(int seed) {
    int r;

    r = oj_seed_prng(seed);
    if (r) return r;

    _oj_johnnymoss = 0x10ACE0FF;
    return 0;
}

/* Dealing with card sequences.
 */

int oj_seq_new(oj_seq_t *sp, int size, int *buf) {
}

int oj_seq_fill(oj_decktype_t dt) {
}

int oj_seq_shuffle(oj_sequence_t *sp) {
}

int oj_seq_deal_from(oj_sequence_t *sp) {
}

int oj_seq_deal_to(oj_sequence_t *sp, int card) {
}

int oj_seq_copy(oj_sequence_t *destp, oj_sequence_t *srcp, int count) {
}

/* Dealing with card names. Should probably do some i18n eventually.
 */

int oj_card_from_name(char *name, char *next) {
}

char *oj_name_of_card(int card, oj_cardstyle_t style) {
}

int oj_seq_name(oj_seq_t *sp, char *name, int size) {
}

int oj_add_by_name(oj_seq_t *sp, char *names) {
}

/* Various combinatorics.
 */

long oj_binomial(int n, int m) {
}

long oj_iter_new(oj_iterator_t *iter, int length, oj_sequence_t *deck) {
}

oj_iterator_t *oj_iter_next(oj_iterator_t *iter) {
}

long oj_colex_rank(oj_sequence_t *hand, oj_sequence_t *deck) {
}

long oj_hand_at_colex_rank(long r, int length, oj_sequence_t *deck) {
}
