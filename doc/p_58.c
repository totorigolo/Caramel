#include <stdint.h>
#include <stdio.h>


int32_t ret4() {
    return 4;
}

int32_t main() {
    putchar('O' - 4 + ret4());
    putchar('O' - ret4());
    putchar('\n');
    return 0;
}