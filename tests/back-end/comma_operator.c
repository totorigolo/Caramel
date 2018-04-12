/*
 * Multi-line comment.
 */

#include <stdio.h>
#include <stdint.h>


int32_t foo(int32_t a, int32_t b, int32_t c) {
    putchar('a');
    putchar('=');
    putchar('0' + a);
    putchar('\n');
    putchar('b');
    putchar('=');
    putchar('0' + b);
    putchar('\n');
    putchar('c');
    putchar('=');
    putchar('0' + c);
    putchar('\n');
    putchar('r');
    putchar('e');
    putchar('t');
    putchar('=');
    putchar('0' + (a, b, c));
    putchar('\n');
    return a, b, c;
}

int32_t main() {
    int32_t i;
    int32_t j;

    putchar('0' + foo(1 + 8, (2 && 4, 2 || 4, 5), 3 * 5 - 9)); // 6
    putchar('\n');

    putchar((putchar('i'), '\n'));
    for (i = 0; i < 10; i++) {
        putchar('0' + i);
    }
    putchar('\n');

    putchar((putchar('j'), '\n'));
    for (i = 0, j = 0; j %= 5, i < 10; i++, j += 2) {
        putchar('0' + j);
    }
    putchar('\n');

    return 0;
}
