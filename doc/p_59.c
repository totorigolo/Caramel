#include <stdint.h>
#include <stdio.h>


int32_t main() {
    int32_t a;
    int32_t b[1];

    a = 'O';
    b[0] = 'K';
    putchar(a);
    putchar(b[0]);
    putchar('\n');

    return 0;
}