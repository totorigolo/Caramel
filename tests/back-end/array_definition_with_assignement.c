/*
 * Multi-line comment.
 */

#include <stdint.h>
#include <stdio.h>



int32_t main() {
    int32_t tab1[] = {1 + 1, 2 - 1, 3 * 5};
    int32_t tab2[5] = {1, 2, 3};
    int32_t i = 2;

    for (i = 0; 3 - i; i = i + 1) {
        putchar('a' + tab1[i]);
    }
    putchar('\n');

    for (i = 0; 5 - i; i = i + 1) {
        putchar('0' + tab2[i]);
    }
    putchar('\n');

    return 0;
}
