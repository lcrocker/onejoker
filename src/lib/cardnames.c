/* Functions for dealing with textual representations of cards.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "onejoker.h"

static char *card_names_ascii2[] = { "XX",
    "2c", "2d", "2h", "2s", "3c", "3d", "3h", "3s",
    "4c", "4d", "4h", "4s", "5c", "5d", "5h", "5s",
    "6c", "6d", "6h", "6s", "7c", "7d", "7h", "7s",
    "8c", "8d", "8h", "8s", "9c", "9d", "9h", "9s",
    "Tc", "Td", "Th", "Ts", "Jc", "Jd", "Jh", "Js",
    "Qc", "Qd", "Qh", "Qs", "Kc", "Kd", "Kh", "Ks",
    "Ac", "Ad", "Ah", "As", "JK", "J2",
};

/* Return pointer to internal string name of card.
 */
char *oj_text_from_card(int card) {
    assert(card > 0 && card <= 54);
    return card_names_ascii2[card];
}

/* Given name of card, parse and return integer (or 0 to fail).
 * If <next> is non-null, fill it with pointer to character after
 * the final one we parsed. If the function returns 0, <next> is
 * not modified, and if it does not, <next> is only written to
 * immediately before returning, so <next> can be the address of
 * <name> without strange things happening.
 */
static int _oj_card_from_text(char *text, char **next) {
    int rank = 0, suit = 0;

    assert(0 != text);
    if (!*text) return 0;
    while (isspace(*text)) ++text;
    if (!*text) return 0;

    if (isdigit(*text)) {
        if ('1' == *text) {
            if ('0' == text[1]) {
                rank = 8;
                ++text;
            }
        } else {
            rank = *text - '2';
        }
    } else if ('J' == toupper(*text)) {
        if ('K' == toupper(text[1])) {
            if (next) *next = ++text;
            return 53;
        } else if ('2' == text[1]) {
            if (next) *next = ++text;
            return 54;
        }
        rank = 9;
    } else if ('T' == toupper(*text)) {
        rank = 8;
    } else if ('Q' == toupper(*text)) {
        rank = 10;
    } else if ('K' == toupper(*text)) {
        rank = 11;
    } else if ('A' == toupper(*text)) {
        rank = 12;
    }
    ++text;

    if (!*text) return 0;
    while (isspace(*text)) ++text;
    if (!*text) return 0;

    if ('c' == tolower(*text)) {
        suit = 0;
    } else if ('d' == tolower(*text)) {
        suit = 1;
    } else if ('h' == tolower(*text)) {
        suit = 2;
    } else if ('s' == tolower(*text)) {
        suit = 3;
    } else {
        return 0;
    }

    if (next) *next = ++text;
    return ((rank << 2) + suit) + 1;
}

int oj_card_from_text(char *text) {
    return _oj_card_from_text(text, 0);
}

/* Fill the character array <text> with the textual representation
 * of the cards from the given integer array.
 */
int oj_text_from_cards(
    int n,      /* Number of cards */
    int *ip,    /* Card array */
    int tsize,  /* Size of output string */
    char *text, /* Output string */
    char *sep   /* Separator */)
{
    int i, cl, sl, out;
    char *cn;

    assert(0 != n && 0 != ip && 0 != tsize && 0 != text);
    if (sep) sl = strlen(sep);
    out = 0;

    for (i = 0; i < n; ++i) {
        if (sep && 0 != i && tsize > sl) {
            strncpy(text, sep, sl);
            text += sl;
            tsize -= sl;
        }
        cn = card_names_ascii2[ip[i]];
        cl = strlen(cn);
        if (tsize > cl) {
            strncpy(text, cn, cl);
            text += cl;
            tsize -= cl;
            ++out;
        }
    }
    *text = '\0';
    return out;
}

/* Add to int array <dp> all the cards in <text>. Return the number
 * of cards added. Assumes ascii2 card text type.
 */
int oj_cards_from_text(int max, int *dp, char *names) {
    int i, c, added;

    added = 0;
    for (i = 0; i < max; ++i) {
        c = _oj_card_from_text(names, &names);
        if (!c) break;
        dp[i] = c;
        ++added;
    }
    return added;
}
