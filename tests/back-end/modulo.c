/*
 * Multi-line comment.
 */

#include <stdint.h>
#include <stdio.h>



int32_t foo() {
    return 3;
}

int32_t foo2() {
    return 10;
}



int32_t main() {

    int32_t d = 10;
    int32_t e = 3;

    int32_t a = 10%3;
    int32_t b = 10%(2+1);
    int32_t c = (5+5)%(2+1);
    int32_t f = d%e;
    int32_t g = d%foo();
    int32_t h = foo2()%e;
    putchar('0' + a);
    putchar('\n');
    putchar('0' + b);
    putchar('\n');
    putchar('0' + c);
    putchar('\n');
    putchar('0' + f);
    putchar('\n');
    putchar('0' + g);
    putchar('\n');
    putchar('0' + h);
    putchar('\n');
    return 0;
}