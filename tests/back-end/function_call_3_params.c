/*
 * Multi-line comment.
 */
#include <stdio.h>
#include <stdint.h>

int32_t foo(int32_t a, int32_t b, int32_t c) {
    putchar(a);
    putchar(b);
    putchar(c);
    return b;
}

int32_t main() {
    foo('a', 'b', '\n');
    return 0;
}
