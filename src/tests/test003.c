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

    for (n = 54; n <= 200; ++n) {
        b = ojc_binomial(n, 26);
        printf("(%d, %d) = %lld\n", n, 26, b);
    }

    return EXIT_SUCCESS;
}
