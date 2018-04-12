#include <stdint.h>
#include <stdio.h>


void main(void) {
    int32_t a;
    int32_t b;
    int32_t c;
    a = 'M';
    b = 1 + a + 1;
    c = b - 4;
    putchar(b);
    putchar(c);
    putchar('\n');
}
