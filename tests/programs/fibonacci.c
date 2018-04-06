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


void displayNumber(int32_t number) {
    int32_t tab[50];
    int32_t index = 0;
    int32_t i;
    while (number >= 1){
        tab[index]=number%10;
        number = number/10;
        index = index + 1;
    }
    for (i=(index-1);i>=0;i--) {
        putchar('0' + tab[i]);
    }
}

int32_t fibonacci_rec(int32_t n) {
    if (n <= 1) {
        return n;
    } else {
        return fibonacci_rec(n - 1) + fibonacci_rec(n - 2);
    }
}

int32_t fibonacci_lin(int32_t n) {
    int32_t i = 0;
    int32_t j = 1;
    int32_t temp;
    int32_t k;

    for (k = 0; k < n; k = k+1) {
        temp = i + j;
        i = j + 0; // + 0 temp fix for double returns
        j = temp + 0;
    }
    return i;
}

int32_t main() {
    displayNumber(fibonacci_rec(20));
    putchar('\n');
    displayNumber(fibonacci_lin(20));
    putchar('\n');

    return 0;
}