/* Test card sequence and name functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "onejoker.h"

int sequence_test(void) {
    int c;
    int hbuf1[4], hbuf2[16], dbuf[56];
    oj_sequence_t hand1, hand2, deck;

    c = oj_seq_new(&hand1, 4, hbuf1);
    assert(4 == c);
    c = oj_seq_deal_to_end(&hand1, 1);
}

int main(int argc, char *argv[]) {
	return EXIT_SUCCESS;
}
