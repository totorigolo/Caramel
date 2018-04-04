/*
 * Multi-line comment.
 */

#include <stdio.h>
#include <stdint.h>

int32_t foo(int32_t a, int32_t b, int32_t c);

int32_t main() {
    int32_t a, b = 0;
    int32_t c = '!';
    foo('\n', 'a', 'd');
    putchar('d');
    return 0;
}

int32_t foo(int32_t a, int32_t b, int32_t c)
{
    int32_t a2 = 'h', b2, c2;
    putchar(c);
    putchar(a2);
    return 0;
}
