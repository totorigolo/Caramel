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

    for (i = (index - 1); i >= 0; i--) {
        putchar('0' + tab[i]);
    }
    putchar('\n');
}


int32_t factorial(int32_t n) {
    if (n == 0) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

int32_t main() {

    int32_t input;
    int32_t result;

    putchar('S');
    putchar('a');
    putchar('i');
    putchar('s');
    putchar('i');
    putchar('r');
    putchar(' ');
    putchar('l');
    putchar('e');
    putchar(' ');
    putchar('c');
    putchar('h');
    putchar('i');
    putchar('f');
    putchar('f');
    putchar('r');
    putchar('e');
    putchar(' ');
    putchar('\n');

    input = getchar() - '0';

    result = factorial(input);

    putchar('R');
    putchar('e');
    putchar('s');
    putchar('u');
    putchar('l');
    putchar('t');
    putchar('a');
    putchar('t');
    putchar(' ');
    putchar('\n');
    displayNumber(result);

    return 0;

}