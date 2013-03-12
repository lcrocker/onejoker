/* Miscellaneous combinatoric functions for OneJoker.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "onejoker.h"

/* For our purposes, we only need to consider combinations of a few cards out
 * of a deck, so rather than calculating arbitrary binomial coefficients for
 * large numbers which takes a lot of time, we just store a table for up to
 * (54 choose 12), for all values that fit into an unsigned long.
 */
static unsigned long bc_table[53][11] = {
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 6, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 10, 10, 5, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 15, 20, 15, 6, 1, 0, 0, 0, 0, 0, 0 },
    { 21, 35, 35, 21, 7, 1, 0, 0, 0, 0, 0 },
    { 28, 56, 70, 56, 28, 8, 1, 0, 0, 0, 0 },
    { 36, 84, 126, 126, 84, 36, 9, 1, 0, 0, 0 },
    { 45, 120, 210, 252, 210, 120, 45, 10, 1, 0, 0 },
    { 55, 165, 330, 462, 462, 330, 165, 55, 11, 1, 0 },
    { 66, 220, 495, 792, 924, 792, 495, 220, 66, 12, 1 },
    { 78, 286, 715, 1287, 1716, 1716, 1287, 715, 286, 78, 13 },
    { 91, 364, 1001, 2002, 3003, 3432, 3003, 2002, 1001, 364, 91 },
    { 105, 455, 1365, 3003, 5005, 6435, 6435, 5005, 3003, 1365, 455 },
    { 120, 560, 1820, 4368, 8008, 11440, 12870, 11440, 8008, 4368, 1820 },
    { 136, 680, 2380, 6188, 12376, 19448, 24310, 24310, 19448, 12376, 6188 },
    { 153, 816, 3060, 8568, 18564, 31824, 43758, 48620, 43758, 31824, 18564 },
    { 171, 969, 3876, 11628, 27132, 50388, 75582, 92378, 92378, 75582,
        50388 },
    { 190, 1140, 4845, 15504, 38760, 77520, 125970, 167960, 184756, 167960,
        125970 },
    { 210, 1330, 5985, 20349, 54264, 116280, 203490, 293930, 352716, 352716,
        293930 },
    { 231, 1540, 7315, 26334, 74613, 170544, 319770, 497420, 646646, 705432,
        646646 },
    { 253, 1771, 8855, 33649, 100947, 245157, 490314, 817190, 1144066,
        1352078, 1352078 },
    { 276, 2024, 10626, 42504, 134596, 346104, 735471, 1307504, 1961256,
        2496144, 2704156 },
    { 300, 2300, 12650, 53130, 177100, 480700, 1081575, 2042975, 3268760,
        4457400, 5200300 },
    { 325, 2600, 14950, 65780, 230230, 657800, 1562275, 3124550, 5311735,
        7726160, 9657700 },
    { 351, 2925, 17550, 80730, 296010, 888030, 2220075, 4686825, 8436285,
        13037895, 17383860 },
    { 378, 3276, 20475, 98280, 376740, 1184040, 3108105, 6906900, 13123110,
        21474180, 30421755 },
    { 406, 3654, 23751, 118755, 475020, 1560780, 4292145, 10015005, 20030010,
        34597290, 51895935 },
    { 435, 4060, 27405, 142506, 593775, 2035800, 5852925, 14307150, 30045015,
        54627300, 86493225 },
    { 465, 4495, 31465, 169911, 736281, 2629575, 7888725, 20160075, 44352165,
        84672315, 141120525 },
    { 496, 4960, 35960, 201376, 906192, 3365856, 10518300, 28048800, 64512240,
        129024480, 225792840 },
    { 528, 5456, 40920, 237336, 1107568, 4272048, 13884156, 38567100,
        92561040, 193536720, 354817320 },
    { 561, 5984, 46376, 278256, 1344904, 5379616, 18156204, 52451256,
        131128140, 286097760, 548354040 },
    { 595, 6545, 52360, 324632, 1623160, 6724520, 23535820, 70607460,
        183579396, 417225900, 834451800 },
    { 630, 7140, 58905, 376992, 1947792, 8347680, 30260340, 94143280,
        254186856, 600805296, 1251677700 },
    { 666, 7770, 66045, 435897, 2324784, 10295472, 38608020, 124403620,
        348330136, 854992152, 1852482996 },
    { 703, 8436, 73815, 501942, 2760681, 12620256, 48903492, 163011640,
        472733756, 1203322288, 2707475148UL },
    { 741, 9139, 82251, 575757, 3262623, 15380937, 61523748, 211915132,
        635745396, 1676056044, 3910797436UL },
    { 780, 9880, 91390, 658008, 3838380, 18643560, 76904685, 273438880,
        847660528, 2311801440UL, 0 },
    { 820, 10660, 101270, 749398, 4496388, 22481940, 95548245, 350343565,
        1121099408, 3159461968UL, 0 },
    { 861, 11480, 111930, 850668, 5245786, 26978328, 118030185, 445891810,
        1471442973, 4280561376UL, 0 },
    { 903, 12341, 123410, 962598, 6096454, 32224114, 145008513, 563921995,
        1917334783, 0, 0 },
    { 946, 13244, 135751, 1086008, 7059052, 38320568, 177232627, 708930508,
        2481256778UL, 0, 0 },
    { 990, 14190, 148995, 1221759, 8145060, 45379620, 215553195, 886163135,
        3190187286UL, 0, 0 },
    { 1035, 15180, 163185, 1370754, 9366819, 53524680, 260932815, 1101716330,
        4076350421UL, 0, 0 },
    { 1081, 16215, 178365, 1533939, 10737573, 62891499, 314457495, 1362649145,
        0, 0, 0 },
    { 1128, 17296, 194580, 1712304, 12271512, 73629072, 377348994, 1677106640,
        0, 0, 0 },
    { 1176, 18424, 211876, 1906884, 13983816, 85900584, 450978066, 2054455634,
        0, 0, 0 },
    { 1225, 19600, 230300, 2118760, 15890700, 99884400, 536878650,
        2505433700UL, 0, 0, 0 },
    { 1275, 20825, 249900, 2349060, 18009460, 115775100, 636763050,
        3042312350UL, 0, 0, 0 },
    { 1326, 22100, 270725, 2598960, 20358520, 133784560, 752538150,
        3679075400UL, 0, 0, 0 },
    { 1378, 23426, 292825, 2869685, 22957480, 154143080, 886322710,
        0, 0, 0, 0 },
    { 1431, 24804, 316251, 3162510, 25827165, 177100560, 1040465790,
        0, 0, 0, 0 }
};

unsigned long oj_binomial(int n, int k) {
    if (k > n) return 0;
    if ((n - k) < k) k = n - k;

    if (n < 1 || k < 0 || n > 54 || k > 12) return 0;
    else if (0 == k) return 1;
    else if (1 == k) return n;
    else return bc_table[n-2][k-2];
}

/* Given a <k>-length 0-based array <a> of integers in the range
 * 0..<n>-1, assumed to be in ascending order, modify the array to
 * next in colexicographical order. Return 1 if we did in fact
 * modify it, or 0 if it was already at the end. This is so we can
 * do "do { ... } while (colex_next( ... ));"
 */
int colex_next(int *a, int n, int k) {
    int i, j;

    for (i = 0; i < k; ++i) {
        if ( ((i < k - 1) && (a[i] < (a[i + 1] - 1))) ||
             ((i == k - 1) && (a[i] < n - 1)) ) {
            ++a[i];
            for (j = 0; j < i; ++j) a[j] = j;
            return 1;
        }
    }
    return 0;
}

unsigned long colex_rank(int *a, int k) {
    int i;
    unsigned long r = 0;

    for (i = 0; i < k; ++i) r += oj_binomial(a[i], i + 1);
    return r;
}

unsigned long oj_iter_new(oj_iterator_t *iter, int length, oj_sequence_t *deck) {
}

unsigned long oj_iter_next(oj_iterator_t *iter) {
}

unsigned long oj_iter_count(oj_iterator_t *iter) {
}

unsigned long oj_colex_rank(oj_sequence_t *hand, oj_sequence_t *deck) {
}

unsigned long oj_hand_at_colex_rank(unsigned long r, int length, oj_sequence_t *deck) {
}
