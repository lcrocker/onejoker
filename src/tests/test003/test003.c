/*
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "onejoker.h"

int main(int argc, char *argv[]) {
    int n, k;
    long long b;

    for (n = 0; n <= 54; ++n) {
        for (k = 0; k <= 54; ++k) {
            b = oj_binomial(n, k);
            printf("(%d, %d) = %lld\n", n, k, b);
        }
    }

    return EXIT_SUCCESS;
}
