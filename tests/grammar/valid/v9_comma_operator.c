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
    return a, b, c;
}

int32_t main() {
    putchar('0' + foo(1 + 8, (2 && 4, 2 || 4, 5), 3 * 5 - 9)); // 6
    return 0;
}
