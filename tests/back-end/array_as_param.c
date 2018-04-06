/*
* Multi-line comment.
*/
#include <stdio.h>
#include <stdint.h>


void print_tab(int32_t tab[], int32_t size) {
    int32_t i;
    for (i = 0; i < size; i++) {
        putchar('0' + tab[i]);
        putchar('\n');
    }
}

int32_t main() {
    int32_t tab[] = {1, 2, 3, 4, 5};
    print_tab(tab, 5);
    return 0;
}
