/* OneJoker library <http://lcrocker.github.io/OneJoker/>
 * Information on types of card decks.
 */

#include <stdlib.h>
#include "onejoker.h"

/* Standard 52-card deck */
static int _standard_cards[] = {
    1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,
    25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,
    46,47,48,49,50,51,52
};

/* 53-card deck with one joker */
static int _onejoker_cards[] = {
    1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,
    25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,
    46,47,48,49,50,51,52,53
};

/* 54 cards, one "black" joker and one "red" joker */
static int _twojokers_cards[] = {
    1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,
    25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,
    46,47,48,49,50,51,52,53,54
};

/* 2s through 6s removes (as in Manila poker, Skat) */
static int _stripped32_cards[] = {
    21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,
    37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52
};

/* 8s, 9s, 10s removed (as in Panguingue) */
static int _stripped40_cards[] = {
    1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,
    37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52
};

/* 8s, 9s, 10s removed, joker added (as in Mexican poker) */
static int _stripped40j_cards[] = {
    1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,
    37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53
};

/* 9s and above only (Pinochle) */
static int _pinochle_cards[] = {
    29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52
};

#define OJD_NTYPES 7

oj_cardlist_t _oj_common_decks[] = {
    { 0x10ACE0FF, 52, 52, OJF_RDONLY, 0, 0LL, NULL, _standard_cards },
    { 0x10ACE0FF, 53, 53, OJF_RDONLY, 0, 0LL, NULL, _onejoker_cards },
    { 0x10ACE0FF, 54, 54, OJF_RDONLY, 0, 0LL, NULL, _twojokers_cards },
    { 0x10ACE0FF, 32, 32, OJF_RDONLY, 0, 0LL, NULL, _stripped32_cards },
    { 0x10ACE0FF, 40, 40, OJF_RDONLY, 0, 0LL, NULL, _stripped40_cards },
    { 0x10ACE0FF, 41, 41, OJF_RDONLY, 0, 0LL, NULL, _stripped40j_cards },
    { 0x10ACE0FF, 24, 24, OJF_RDONLY, 0, 0LL, NULL, _pinochle_cards },
};

int ojd_ntypes(void) { return OJD_NTYPES; }
int ojd_size(oj_deck_type_t t) { return _oj_common_decks[t].length; }
oj_cardlist_t *ojd_deck(oj_deck_type_t t) { return &_oj_common_decks[t]; }
