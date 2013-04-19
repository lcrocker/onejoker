/* OneJoker library <http://lcrocker.github.io/OneJoker>
 *
 * "Hello World" example, kept here to make sure that the documentation stays
 * in sync with the code.
 */

#include <stdio.h>
#include <stdlib.h>
#include "onejoker.h"

int main(int argc, char *argv[]) {
    int i, db[52], hb[5];
    char text[20];
    oj_cardlist_t deck, hand;

    ojl_new(&deck, db, 52);
    ojl_fill(&deck, 52, OJD_STANDARD);
    ojl_shuffle(&deck);

    ojl_new(&hand, hb, 5);
    for (i = 0; i < 5; ++i) ojl_append(&hand, ojl_pop(&deck));

    ojl_text(&hand, text, sizeof(text));
    printf("%s\n", text);

    return 0;
}