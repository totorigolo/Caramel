#include <stdint.h>
#include <stdio.h>


int32_t main() {
    if (1 < 2) {
        putchar('O');
        putchar('K');
    } else {
        putchar('K');
        putchar('O');
    }
    putchar('\n');
}