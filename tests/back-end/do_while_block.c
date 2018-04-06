/*
 * Multi-line comment.
 */

#include <stdint.h>
#include <stdio.h>



int32_t main() {
    int32_t a = 5;
    do {
        putchar('0'+a);
        a--;
    } while(a);
    putchar('\n');
    putchar('e');
    putchar('x');
    putchar('i');
    putchar('t');
    putchar('\n');
    return 0;
}