/*
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "onejoker.h"

#define TABLE4SIZE 270725
#define TABLE3SIZE 2210
#define TABLE2SIZE 1326

struct _table4 {
	uint32_t key;
	uint16_t vals[52];
} table4[TABLE4SIZE];

struct _table3 {
	uint32_t key;
	uint32_t t4index[52];
} table3[TABLE3SIZE];

struct _table2 {
	uint16_t key;
	uint16_t t3index[52];
} table2[TABLE2SIZE];

uint16_t table1[52];

int build_table4(void) {
	int c1, c2, c3, c4, c5;
	int count = 0;

	for (c1 = 1; c1 <= 49; ++c1) {
		for (c2 = c1 + 1; c2 <= 50; ++c2) {
			for (c3 = c2 + 1; c3 <= 51; ++c3) {
				for (c4 = c3 + 1; c4 <= 52; ++c4) {
					table4[count].key = (c1<<18)|(c2<<12)|(c3<<6)|c4;

					for (c5 = 1; c5 <= 52; ++c5) {
						if (c5 == c1 || c5 == c2 || c5 ==c3 || c5 == c4) {
							table4[count].vals[c5-1] = 0;
						} else {
							table4[count].vals[c5-1] = oj_poker_eval5(c1, c2, c3, c4, c5);
						}
					}
					++count;
					if (0 == (count & 0xFFFF)) {
						fprintf(stderr, "%d\r", count);
					}
				}
			}
		}
	}
	fprintf(stderr, "Table built, %d entries.\n", count);
	return count;
}

#define SWAP(a,b) do{s=h[a]+h[b];d=abs(h[b]-h[a]);h[a]=(s+d)/2;h[b]=(s-d)/2;}while(0)

uint32_t build_key(int *h) {
	register int s, d;
	SWAP(0,1);  SWAP(3,4);  SWAP(2,4);
	SWAP(2,3);  SWAP(1,4);  SWAP(0,3);
	SWAP(0,2);  SWAP(1,3);  SWAP(1,2);
	return (h[0]<<24)|(h[1]<<18)|(h[2]<<12)|(h[3]<<6)|(h[4]);
}

int build_table3(void) {
	uint32_t key, t4index;
	int h[8];
	int count = 0;

	for (h[0] = 1; h[0] <= 50; ++h[0]) {
		for (h[1] = h[0] + 1; h[1] <= 51; ++h[1]) {
			for (h[2] = h[1] + 1; h[2] <= 52; ++h[2]) {
				for (h[3] = 1; h[3] <= 52; ++h[3]) {
					if (h[3] == h[0] || h[3] == h[1] || h[3] == h[2]) continue;
					table3[count].key = (h[0]<<12)|(h[1]<<6)|h[2];

					key = build_key(h);
					for (t4index = 0; t4index < TABLE4SIZE; ++t4index) {
						if (key == table4[t4index].key) {
							table3[count].t4index[h[3]-1] = (uint32_t)count;
							break;
						}
					}
					assert(TABLE4SIZE != t4index);
					++count;
				}
			}
		}
	}
	return 0;
}

int build_table2(void) {
	return 0;
}

int build_table1(void) {
	return 0;
}

void dump_tables(void) {
	int i, j, k;

	printf("static uint16_t _oj_lc_table4[] = {\n");
	for (i = 0; i < TABLE4SIZE; ++i) {
		for (j = 0; j < 4; ++j) {
			printf("    ");
			for (k = 0; k < 13; ++k) {
				printf("%d, ", table4[i].vals[4*j + k]);
			}
			printf("\n");
		}
	}
	printf("};\n\nstatic uint32_t _oj_lc_table3[] = {\n");
	for (i = 0; i < TABLE3SIZE; ++i) {
		for (j = 0; j < 4; ++j) {
			printf("    ");
			for (k = 0; k < 13; ++k) {
				printf("%d, ", table3[i].t4index[4*j + k]);
			}
			printf("\n");
		}
	}
	printf("};\n\nstatic uint16_t _oj_lc_table2[] = {\n");
	for (i = 0; i < TABLE2SIZE; ++i) {
		for (j = 0; j < 4; ++j) {
			printf("    ");
			for (k = 0; k < 13; ++k) {
				printf("%d, ", table2[i].t3index[4*j + k]);
			}
			printf("\n");
		}
	}
	printf("};\n\nstatic uint16_t _oj_lc_table1[] = {\n");
	for (j = 0; j < 4; ++j) {
		printf("    ");
		for (k = 0; k < 13; ++k) {
			printf("%d, ", table1[4*j + k]);
		}
		printf("\n");
	}
	printf("};\n");
}

int main(int argc, char *argv[]) {
	int c, d;

	c = build_table4();
	c = build_table3();
	c = build_table2();
	c = build_table1();
	dump_tables();
}
