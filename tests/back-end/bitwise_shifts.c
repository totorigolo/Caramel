/*
 * Multi-line comment.
 */

#include <stdint.h>


int32_t main() {

    int32_t a = 1, b = 1;
    int32_t c = 32, d = 32;



    a = (a << 2) << 2; //16
    b = 2 << (b << 2); //32

    c = (c >> 2) >> 1; //4
    d = 32 >> (d >> 4); //8

    putchar('a' - 1 + a); //p
    putchar('\n');
    putchar('a' - 1 + b - 10); //v
    putchar('\n');

    putchar('0' + c);
    putchar('\n');
    putchar('0' + d);
    putchar('\n');


    return 0;
}