#include <stdint.h>
#include <stdio.h>


int32_t main() {
    int32_t a, b, c;
    a = 'O';
    b = a;
    c = 'K';
    a = '\n';
    putchar(b);
    putchar(c);
    putchar(a);
}