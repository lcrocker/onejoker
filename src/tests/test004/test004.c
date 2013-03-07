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
#define TABLE3SIZE 22100
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
	uint32_t t3index[52];
} table2[TABLE2SIZE];

uint32_t table1[53 * 52 + 1];

#define SWAP(a,b) do{s=h[a]+h[b];d=abs(h[b]-h[a]);h[b]=(s+d)/2;h[a]=(s-d)/2;}while(0)

uint32_t key4(int *h) {
	register int s, d;

	SWAP(0,1);  SWAP(2,3);  SWAP(0,2);
	SWAP(1,3);  SWAP(1,2);
	return (h[0]<<18)|(h[1]<<12)|(h[2]<<6)|h[3];
}

int build_table4(void) {
	int c1, c2, c3, c4, c5;
	int h[8], count = 0;

	for (c1 = 1; c1 <= 49; ++c1) {
		for (c2 = c1 + 1; c2 <= 50; ++c2) {
			for (c3 = c2 + 1; c3 <= 51; ++c3) {
				for (c4 = c3 + 1; c4 <= 52; ++c4) {
					h[0] = c1; h[1] = c2; h[2] = c3; h[3] = c4;
					table4[count].key = key4(h);

					for (c5 = 1; c5 <= 52; ++c5) {
						if (c5 == c1 || c5 == c2 || c5 == c3 || c5 == c4) {
							table4[count].vals[c5 - 1] = 0;
						} else {
							table4[count].vals[c5 - 1] = oj_poker_eval5(c1, c2, c3, c4, c5);
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
	fprintf(stderr, "Table 4: %d entries.\n", count);
	return count;
}

uint32_t key3(int *h) {
	register int s, d;

	SWAP(1,2);  SWAP(0,2);  SWAP(0,1);
	return (h[0]<<12)|(h[1]<<6)|h[2];
}

int findkey4(uint32_t key) {
	int mid, low = 0, high = TABLE4SIZE - 1;

	while (high >= low) {
		mid = (high + low) / 2;
		if (table4[mid].key < key) low = mid + 1;
		else if (table4[mid].key > key) high = mid - 1;
		else return mid;
	}
	assert(0);
}

int build_table3(void) {
	int c1, c2, c3, c4;
	uint32_t key, t4index;
	int h[8], count = 0;

	for (c1 = 1; c1 <= 50; ++c1) {
		for (c2 = c1 + 1; c2 <= 51; ++c2) {
			for (c3 = c2 + 1; c3 <= 52; ++c3) {
				for (c4 = 1; c4 <= 52; ++c4) {
					if (c4 == c1 || c4 == c2 || c4 == c3) continue;
					h[0] = c1; h[1] = c2; h[2] = c3;
					table3[count].key = key3(h);
					h[3] = c4;
					key = key4(h);

					t4index = findkey4(key);
					table3[count].t4index[c4 - 1] = 52 * t4index + c4 - 2;
				}
				++count;
				if (0 == (count & 0x3FF)) {
					fprintf(stderr, "%d\r", count);
				}
			}
		}
	}
	fprintf(stderr, "Table 3: %d entries.\n", count);
	return count;
}

uint16_t key2(int *h) {
	register int s, d;

	SWAP(0,1);
	return (h[0]<<6)|h[1];
}

int findkey3(uint32_t key) {
	int mid, low = 0, high = TABLE3SIZE - 1;

	while (high >= low) {
		mid = (high + low) / 2;
		if (table3[mid].key < key) low = mid + 1;
		else if (table3[mid].key > key) high = mid - 1;
		else return mid;
	}
	assert(0);
}

int build_table2(void) {
	int c1, c2, c3;
	uint32_t key, t3index;
	int h[8], count = 0;

	for (c1 = 1; c1 <= 51; ++c1) {
		for (c2 = c1 + 1; c2 <= 52; ++c2) {
			for (c3 = 1; c3 <= 52; ++c3) {
				if (c3 == c1 || c3 == c2) continue;
				h[0] = c1; h[1] = c2;
				table2[count].key = key2(h);
				h[2] = c3;
				key = key3(h);

				t3index = findkey3(key);
				table2[count].t3index[c3 - 1] = 52 * t3index +  c3 - 2;
			}
			++count;
		}
	}
	fprintf(stderr, "Table 2: %d entries.\n", count);
	return count;
}

int findkey2(uint16_t key) {
	int mid, low = 0, high = TABLE2SIZE - 1;

	while (high >= low) {
		mid = (high + low) / 2;
		if (table2[mid].key < key) low = mid + 1;
		else if (table2[mid].key > key) high = mid - 1;
		else return mid;
	}
	assert(0);
}

int build_table1(void) {
	int c1, c2;
	uint16_t key, t2index;
	int h[8], count = 0;

	for (c1 = 1; c1 <= 52; ++c1) {
		for (c2 = 1; c2 <= 52; ++c2) {
			if (c2 == c1) continue;
			h[0] = c1; h[1] = c2;
			key = key2(h);

			t2index = findkey2(key);
			table1[53*c1 + c2] = 52 * t2index + c2 - 2;
		}
		++count;
	}
	return count;
}

void dump_tables(void) {
	int i, j, k;

	fprintf(stderr, "Writing tables.\n");

	printf("static uint16_t _oj_lc_table4[%d] = {", 52 * TABLE4SIZE);
	for (i = 0; i < TABLE4SIZE; ++i) {
		for (j = 0; j < 4; ++j) {
			printf("\n    ");
			for (k = 0; k < 13; ++k) printf("%d, ", table4[i].vals[4*j + k]);
		}
	}

	printf("\n};\n\nstatic uint32_t _oj_lc_table3[%d] = {", 52 * TABLE3SIZE);
	for (i = 0; i < TABLE3SIZE; ++i) {
		for (j = 0; j < 4; ++j) {
			printf("\n    ");
			for (k = 0; k < 13; ++k) printf("%d, ", table3[i].t4index[4*j + k]);
		}
	}

	printf("\n};\n\nstatic uint32_t _oj_lc_table2[%d] = {", 52 * TABLE2SIZE);
	for (i = 0; i < TABLE2SIZE; ++i) {
		for (j = 0; j < 4; ++j) {
			printf("\n    ");
			for (k = 0; k < 13; ++k) printf("%d, ", table2[i].t3index[4*j + k]);
		}
	}

	printf("\n};\n\nstatic uint32_t _oj_lc_table1[%d] = {", 53 * 52 + 1);
	for (i = 0; i < 53 * 53; ++i) {
		if (0 == (i & 0x7)) printf("\n    ");
		printf("%d, ", table1[i]);
	}
	printf("\n};\n");
}

int main(int argc, char *argv[]) {
	int c, d;

	c = build_table4();
	assert(TABLE4SIZE == c);

	c = build_table3();
	assert(TABLE3SIZE == c);

	c = build_table2();
	assert(TABLE2SIZE == c);

	c = build_table1();
	assert(52 == c);

	dump_tables();
	fprintf(stderr, "Done.\n");
}
