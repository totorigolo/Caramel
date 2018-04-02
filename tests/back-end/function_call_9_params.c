/*
 * Multi-line comment.
 */
#include <stdio.h>
#include <stdint.h>

int32_t foo(int32_t a, int32_t b, int32_t c,
            int32_t d, int32_t e, int32_t f,
            int32_t g, int32_t h, int32_t i) {
    return b;
}

int32_t main() {
    foo(1, 2, 3, 4, 5, 6, 7, 8, 9);
    return 0;
}
