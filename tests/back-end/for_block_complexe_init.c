/*
 * Multi-line comment.
 */

#include <stdint.h>

int32_t sum(int32_t a, int32_t b) {
    return a + b;
}

int32_t main() {
    int32_t a = 0;
    putchar('0' + sum(2, 3));
    for (a=sum(2, 3) + 5; a + 1 - 1; a = a - 1) { // 5 + 5 = 10
        putchar('a');
        putchar('\n');
    }
    return 0;
}
