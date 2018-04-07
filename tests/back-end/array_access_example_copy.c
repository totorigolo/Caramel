/*
 * Multi-line comment.
 */

#include <stdint.h>
#include <stdio.h>



int32_t vice() {
    int32_t tab1[4] = {1, 2, 3, 4};
    int32_t tab2[4] = {6, 7, 8, 9};
    int32_t i = 0;

    for (i = 0; 4 - i; i++) {
        putchar('0' + tab1[i]);
        putchar('=');
        putchar('>');
        tab1[i] = tab2[i];
        putchar('0' + tab2[i]);
        putchar('\n');
    }
    for (i = 0; 4 - i; i++) {
        putchar('0' + tab1[i]);
    }
    putchar('\n');
    for (i = 0; 4 - i; i++) {
        putchar('0' + tab2[i]);
    }
    putchar('\n');

    return 0;
}

int32_t versa() {
    int32_t tab1[4] = {1, 2, 3, 4};
    int32_t tab2[4] = {6, 7, 8, 9};
    int32_t i = 0;

    for (i = 0; 4 - i; i++) {
        putchar('0' + tab1[i]);
        putchar('=');
        putchar('>');
        tab1[i] = tab2[4 - 1 - i];
        putchar('0' + tab2[4 - 1 - i]);
        putchar('\n');
    }
    for (i = 0; 4 - i; i++) {
        putchar('0' + tab1[i]);
    }
    putchar('\n');
    for (i = 0; 4 - i; i++) {
        putchar('0' + tab2[i]);
    }
    putchar('\n');

    return 0;
}

int32_t main() {
    vice();
    versa();

    return 0;
}
