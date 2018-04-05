/*
 * Multi-line comment.
 */

#include <stdint.h>

int32_t main() {
    int32_t tab[5];
    int32_t i = 3;

    tab[2] = 5;
    tab[i] = 7;

    for (i = 0; 5 - i; i = i + 1) {
        putchar('0' + tab[i]);
        putchar('\n');
    }

    return 0;
}
