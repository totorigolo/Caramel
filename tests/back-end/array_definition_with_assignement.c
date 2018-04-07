/*
 * Multi-line comment.
 */

#include <stdint.h>
#include <stdio.h>


/*
 * Displays:
 *   - bao
 *   - 12300
 */
int32_t main() {
    int32_t tab1[] = {2 * 10 + 2, 2 * 7, 20 - 3, 3 * 3 + 1, 3 * 6};
    int32_t tab2[5] = {1, 2, 3};
    int32_t i;

    for (i = 0; i < 5; i++) {
        putchar('a' + tab1[i]);
    }
    putchar('\n');

    for (i = 0; i < 5; i++) {
        putchar('0' + tab2[i]);
    }
    putchar('\n');

    return 0;
}
