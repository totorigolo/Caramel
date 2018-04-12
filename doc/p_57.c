#include <stdint.h>
#include <stdio.h>


int32_t main() {
    int32_t i = 'O';
    while (i >= 'K') {
        putchar(i);
        i -= 4;
    }
    putchar('\n');
}
