/*
 * Multi-line comment.
 */

#include <stdint.h>


int32_t main() {

    int32_t a = 1, b = 1;
    a = (a << 2) << 2; //16
    b = 2 << (b << 2); //32

    putchar('a' - 1 + a); //p
    putchar('\n');
    putchar('a' - 1 + b - 10); //v
    putchar('\n');

    return 0;
}