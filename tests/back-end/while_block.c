/*
 * Multi-line comment.
 */

#include <stdint.h>

int32_t main() {
    int32_t a = 5;
    while (a) {
        putchar('0' + a);
        a = a - 1;
    }
    putchar('\n');
    return 0;
}