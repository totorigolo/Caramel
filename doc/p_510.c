#include <stdio.h>
#include <stdint.h>


void print_tab(int32_t tab[], int32_t size) {
    int32_t i;
    for (i = 0; i < size; i++) {
        putchar(tab[i]);
    }
}

int32_t main() {
    int32_t i = 0;
    int32_t tab[] = {'O', 'K', '\n'};

    print_tab(tab, 3);

    for (i = 0; i < 3; i++) {
        putchar(tab[i]);
    }

    return 0;
}
