/*
 * Multi-line comment.
 */
#include <stdint.h>
#include <stdio.h>



int32_t foo() {
    int32_t a = 0;
    if (a) {
        return 5;
        putchar('a');
        putchar('f');
        putchar('t');
        putchar('e');
        putchar('r');
    } else {
        return 6;
        putchar('a');
        putchar('f');
        putchar('t');
        putchar('e');
        putchar('r');
    }
    return 0;
}

int32_t main() {
    int32_t a = 0;
    putchar('0' + foo());
    putchar('\n');
    return 0;
}