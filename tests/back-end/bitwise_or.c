/*
 * Multi-line comment.
 */

#include <stdint.h>


int32_t main() {

    int32_t a = 1;
    int32_t b = 2;
    int32_t c = 3;
    int32_t d;

    d = a|b; //3

    putchar('0' + d);
    putchar('\n');

    d= b|c; //3

    putchar('0' + d);
    putchar('\n');


    return 0;
}