/*
 * Multi-line comment.
 */

#include <stdint.h>
#include <stdio.h>



int32_t sum(int32_t a, int32_t b) {
    return a + b;
}

int32_t main() {
    int32_t a = 0;
    putchar('0' + sum(2, 3));
    if (sum(2, 3) - 5) { // 5 - 5 = 0
        putchar('a');
        putchar('\n');
    }
    return 0;
}
