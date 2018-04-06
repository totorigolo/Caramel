//
// Created by alexandre on 05/04/18.
//

#include <stdint.h>
#include <stdio.h>


void displayNumber(int32_t number) {
    int32_t tab[50];
    int32_t index = 0;
    int32_t i;
    while (number >= 1) {
        tab[index] = number % 10;
        number = number / 10;
        index = index + 1;
    }

    // TODO : manage signed integers in subtraction (... and others ? :x)
    // b *displayNumber+913
    for (i = index; i > 0; i--) { // cond=, inc=901
        putchar('0' + tab[i - 1]);
        // b *displayNumber+996
    }
}

int32_t hailstone(int32_t n) {
    int32_t tab[100];
    int32_t index = 0;
    int32_t i;


    while (n != 1) {
        if (index < 100) {
            tab[index] = n;
            if (n % 2 == 0) {
                n = n / 2;
            } else {
                n = 3 * n + 1;
            }
        } else {
            n = 1;
        }
        index = index + 1;
    }

    if (index < 100) {
        tab[index] = 1;
        index = index + 1;
    }


    for (i = 0; i < index; i++) {
        displayNumber(tab[i]);
        putchar('\n');
    }
    return 0;
}


int32_t main() {

    hailstone(20);


}
