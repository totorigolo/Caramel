/*
 * Multi-line comment.
 */

#include <stdint.h>
#include <stdio.h>




int32_t main() {

    int32_t a = 1;
    int32_t b = 3;
    int32_t d = 7;
    int32_t c;

    c = a&b; //1
    putchar('0' + c);
    putchar('\n');


    c = d&b; //3
    putchar('0' + c);
    putchar('\n');

    return 0;
}