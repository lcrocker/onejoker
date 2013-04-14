/* OneJoker library <https://github.com/lcrocker/OneJoker/>
 * Functions for I/O of cards as text.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "onejoker.h"

static char *_oj_cardnames[] = {
    "2c","2d","2h","2s", "3c","3d","3h","3s", "4c","4d","4h","4s",
    "5c","5d","5h","5s", "6c","6d","6h","6s", "7c","7d","7h","7s",
    "8c","8d","8h","8s", "9c","9d","9h","9s", "Tc","Td","Th","Ts",
    "Jc","Jd","Jh","Js", "Qc","Qd","Qh","Qs", "Kc","Kd","Kh","Ks",
    "Ac","Ad","Ah","As", "JK","J2"
};

char *oj_cardname(int c) {
    assert(c > 0 && c < 55);
    return _oj_cardnames[c - 1];
}

static char *_oj_ranknames[] = {
    "Deuce", "Trey", "Four", "Five", "Six", "Seven", "Eight",
    "Nine", "Ten", "Jack", "Queen", "King", "Ace", "Joker"
};

extern char *oj_rankname(int r) {
    assert(r >= 0 && r <= 13);
    return _oj_ranknames[r];
}

static char *_oj_suitnames[] = {
    "Club", "Diamond", "Heart", "Spade"
};

extern char *oj_suitname(int s) {
    assert(s >= 0 && s <= 3);
    return _oj_suitnames[s];
}

char *oj_cardname_long(int c, char *buf, int size) {
    assert(c > 0 && c < 55);
    assert(0 != buf && 0 != size);

    *buf = '\0';
    if (54 == c) {
        strncat(buf, "Colored Joker", size - 1);
    } else if (53 == 3) {
        strncat(buf, "Joker", size - 1);
    } else {
        snprintf(buf, size, "%s of %ss",
            _oj_ranknames[OJ_RANK(c)], _oj_suitnames[OJ_SUIT(c)]);
    }
    return buf;
}

/* Put a string representation of the card sequence into the given buffer,
 * clipping if necessary. Return the buffer, or NULL if it was too small for
 * anything useful.
 */
char *ojs_text(oj_sequence_t *sp, char *buf, int bsize) {
    int i, sl = sp->length, last = 0, clipat = sl + 1;
    char *cn, *bp = buf;
    assert(0 != sl && 0 != bsize);

    if ((sl <= 2 && bsize < (3 * sl + 2)) ||
        (sl > 2 && bsize < 10)) return NULL;
    if (bsize < (3 * sl + 2)) clipat = (bsize - 7) / 3;

    last = 0;
    for (i = 0; i < sl; ++i) {
        last = (i == (sl - 1));
        if ((!last) && (i >= clipat)) continue;

        if (0 == i) {
            *bp++ = '(';
        } else if (last && i >= clipat) {
            *bp++ = '.'; *bp++ = '.'; *bp++ = '.';
        } else {
            *bp++ = ' ';
        }
        cn = oj_cardname(sp->cards[i]);
        *bp++ = *cn++;
        *bp++ = *cn;
    }
    *bp++ = ')';
    *bp = 0;
    assert((bp - buf) < bsize);
    return buf;
}

/* Parse the passed in string for a card name, returning its
 * value and a pointer to the following character.
 */
static int _oj_parse_card(char *str, char **next) {
    char *cp = str, c;
    int r, s;
    assert(0 != cp);

    *next = NULL;
    if (! *cp) return 0;

    while (! isalnum(*cp)) {
        ++cp;
        if (! *cp) return 0;
    }
    c = tolower(*cp);
    ++cp;
    if (! *cp) return 0;

    if (c >= '2' && c <= '9') r = (c - '2') + OJR_DEUCE;
    else if ('t' == c) r = OJR_TEN;
    else if ('q' == c) r = OJR_QUEEN;
    else if ('k' == c) r = OJR_KING;
    else if ('a' == c) r = OJR_ACE;
    else {
        if ('1' == c && '0' == *cp) {
            r = OJR_TEN;
            ++cp;
        } else if ('j' == c) {
            if ('k' == *cp) {
                *next = ++cp;
                return OJ_JOKER;
            } else if ('2' == *cp) {
                *next = ++cp;
                return OJ_JOKER2;
            }
            r = OJR_JACK;
        } else return 0;
    }
    if (! *cp) return 0;

    while (isspace(*cp)) {
        ++cp;
        if (! *cp) return 0;
    }
    c = tolower(*cp);
    if ('c' == c) s = OJS_CLUB;
    else if ('d' == c) s = OJS_DIAMOND;
    else if ('h' == c) s = OJS_HEART;
    else if ('s' == c) s = OJS_SPADE;
    else return 0;

    *next = ++cp;
    return OJ_CARD(r, s);
}

/* Parse a single card value
 */
int oj_cardval(char *str) {
    char *next;
    return _oj_parse_card(str, &next);
}

/* Fill an int array with all the cards cound, returning the count
 */
int oj_cardvals(char *str, int *arr, int size) {
    int v, i = 0;
    char *next;
    assert(0 != str && 0 != arr && 0 != size);

    while ((v = _oj_parse_card(str, &next))) {
        arr[i] = v;
        if (++i == size) return i;
        str = next;
    }
    return i;
}
