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
char *oj_text_of_card(int card, oj_card_style_t style) {
    assert(card > 0 && card <= 54);
    assert(oj_cs_ascii2 == style);
    return card_names_ascii2[card];
}

/* Given name of card, parse and return integer (or 0 to fail).
 * If <next> is non-null, fill it with pointer to character after
 * the final one we parsed. If the function returns 0, <next> is
 * not modified, and if it does not, <next> is only written to
 * immediately before returning, so <next> can be the address of
 * <name> without strange things happening.
 */
int oj_card_from_text(char *text, char **next) {
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

/* Fill the character array <text> with the textual representation of
 * the given sequence. Don't overflow <size> characters. If <sep> is
 * given, write one after each card but the last. Zero terminate, and
 * return the number of cards actually written (which may be less than
 * the number in the sequence if we run out of room).
 */
int ojs_text(oj_sequence_t *sp, char *text, int size, char sep) {
    int i, len, needed;
    char *np;

    assert(0 != sp);
    assert(size >= 1);
    assert(0 != text);

    for (i = 0; i < sp->length; ++i) {
        np = card_names_ascii2[sp->cards[i]];
        len = strlen(np);
        needed = len + 1;
        if (size < needed) break;

        strncpy(text, np, len);
        text += len;
        size -= len;

        if (sep && (i < (sp->length - 1)) && size > 1) {
            *text++ = sep;
            --size;
        }
    }
    *text = '\0';
    return i;
}

/* Add to int array <dp> all the cards in <text>. Return the number
 * of cards added. Assumes ascii2 card text type.
 */
int oj_cards_from_text(int max, int *dp, char *names) {
    int i, c, added;

    added = 0;
    for (i = 0; i < max; ++i) {
        c = oj_card_from_text(names, &names);
        if (!c) break;
        dp[i] = c;
        ++added;
    }
    return added;
}
