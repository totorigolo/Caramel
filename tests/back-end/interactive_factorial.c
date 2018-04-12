/*
 * MIT License
 *
 * Copyright (c) 2018 Kalate Hexanome, 4IF, INSA Lyon
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include <stdint.h>
#include <stdio.h>


void displayNumber(int32_t number) {
    int32_t tab[50] = {0};
    int32_t index = 0;
    int32_t i;

    // Negative numbers
    if (number < 0) {
        putchar('-');
        number *= -1;
    }

    while (number >= 1) {
        tab[index] = number % 10;
        number /= 10;
        index++;
    }
    if (index == 0) { // special case for displayNumber(0)
        index = 1;
    }

    for (i = index; i > 0; i--) {
        putchar('0' + tab[i - 1]);
    }
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

    putchar('E');
    putchar('n');
    putchar('t');
    putchar('e');
    putchar('r');
    putchar(' ');
    putchar('a');
    putchar(' ');
    putchar('d');
    putchar('i');
    putchar('g');
    putchar('i');
    putchar('t');
    putchar(':');
    putchar(' ');

    input = getchar() - '0';

    result = factorial(input);

    putchar('R');
    putchar('e');
    putchar('s');
    putchar('u');
    putchar('l');
    putchar('t');
    putchar(':');
    putchar(' ');
    displayNumber(result);
    putchar('\n');

    return 0;

}