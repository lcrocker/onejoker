/* OneJoker library <http://lcrocker.github.io/onejoker>
 *
 * Blackjack functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "onejoker.h"

__attribute__((hot))
int ojb_jtotal(const oj_cardlist_t * const sp) {
    int i, c, t = 0, ace = 0, soft = 0;
    assert(0x10ACE0FF == sp->_johnnymoss);

    for (i = 0; i < sp->length; ++i) {
        c = sp->cards[i];
        if (c >= OJ_CARD(OJR_ACE, OJS_CLUB)) {
            ace = 1;
            ++t;
        } else if (c >= OJ_CARD(OJR_TEN, OJS_CLUB)) {
            t += 10;
        } else {
            t += OJ_RANK(c) + 2;
        }
    }
    if (ace && t < 12) {
        t += 10;
        soft = 1;
    }
    return soft ? -t : t;
}
