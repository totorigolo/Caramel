/*
 * Multi-line comment.
 */

#include <stdint.h>

int32_t main() {
    int32_t a = 1;
    int32_t b = 2;
    int32_t c = 4;

    for (a = 5; a >= 0; a--) {
        putchar('0' + a);
        putchar('\n');
    }

    return 0;
}