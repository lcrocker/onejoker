/* OneJoker library <https://github.com/lcrocker/OneJoker/>
 * Information on types of card decks.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "onejoker.h"

/* standard */
int _oj_dt_0_cards[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,
        14,15,16,17,18,19,20,21,22,23,24,25,26,
        27,28,29,30,31,32,33,34,35,36,37,38,39,
        40,41,42,43,44,45,46,47,48,49,50,51,52 };

/* 1joker */
int _oj_dt_1_cards[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,
        14,15,16,17,18,19,20,21,22,23,24,25,26,
        27,28,29,30,31,32,33,34,35,36,37,38,39,
        40,41,42,43,44,45,46,47,48,49,50,51,52,53 };

/* 2jokers */
int _oj_dt_2_cards[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,
        14,15,16,17,18,19,20,21,22,23,24,25,26,
        27,28,29,30,31,32,33,34,35,36,37,38,39,
        40,41,42,43,44,45,46,47,48,49,50,51,52,53,54 };

/* stripped32 */
int _oj_dt_3_cards[] = { 21,22,23,24,25,26,27,28,
        29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,
        45,46,47,48,49,50,51,52 };

/* stripped40 */
int _oj_dt_4_cards[] = { 1,2,3,4,5,6,7,8,9,10,11,12,
        13,14,15,16,17,18,19,20,21,22,23,24,
        37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52 };

/* stripped40j */
int _oj_dt_5_cards[] = { 1,2,3,4,5,6,7,8,9,10,11,12,
        13,14,15,16,17,18,19,20,21,22,23,24,
        37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53 };

#define OJD_NTYPES 6

char *_oj_dt_names[] = {
    "STANDARD", "ONEJOKER", "TWOJOKERS",
    "STRIPPED32", "STRIPPED40", "STRIPPED40J",
};

oj_sequence_t _oj_common_decks[] = {
    { 0x10ACE0FF, 52, 52, _oj_dt_0_cards },
    { 0x10ACE0FF, 53, 53, _oj_dt_1_cards },
    { 0x10ACE0FF, 54, 54, _oj_dt_2_cards },
    { 0x10ACE0FF, 32, 32, _oj_dt_3_cards },
    { 0x10ACE0FF, 40, 40, _oj_dt_4_cards },
    { 0x10ACE0FF, 41, 41, _oj_dt_5_cards },
};

#ifdef _WIN32
#define STRCASECMP(a,b) stricmp((a),(b))
#else
#define STRCASECMP(a,b) strcasecmp((a),(b))
#endif

int ojd_ntypes(void) { return OJD_NTYPES; }
int ojd_ncards(int type) { return _oj_common_decks[type].length; }
oj_sequence_t *ojd_deck(int type) { return &_oj_common_decks[type]; }
oj_sequence_t *ojd_deck_by_name(char *name) {
    int i;
    for (i = 0; i < OJD_NTYPES; ++i) {
        if (0 == STRCASECMP(_oj_dt_names[i], name))
            return &_oj_common_decks[i];
    }
    return NULL;
}
