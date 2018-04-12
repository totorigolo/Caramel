#include <stdio.h>
#include <stdint.h>

int32_t foo(int32_t a, int32_t b, int32_t c,
            int32_t d, int32_t e, int32_t f,
            int32_t g, int32_t h, int32_t i) {
    putchar('0' + a);
    putchar('0' + b);
    putchar('0' + c);
    putchar('0' + d);
    putchar('0' + e);
    putchar('0' + f);
    putchar('0' + g);
    putchar('0' + h);
    putchar('0' + i);
    return d;
}

int32_t main() {
    int32_t k = 'O' - foo(1, 2, 3, 4, 5, 6, 7, 8, 9);
    putchar('\n');
    putchar('O');
    putchar(k);
    putchar('\n');
    return 0;
}
