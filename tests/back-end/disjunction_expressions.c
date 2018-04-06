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



int32_t foo(int32_t a, int32_t b) {
    int32_t useless = 1 + 2 + 8 * 4 - 4 || 1 || 5;
    putchar('0' + a);
    return b;
}

int32_t main() {

    int32_t f = 0 || 1; // 1
    int32_t g = 0 || (1-1) || (1-3+4); // 1
    int32_t h = foo(1, 1) || 0 || 0; // 1
    int32_t i = 0 || ((1-1) || (3*4-4-4-4)); // 0
    int32_t k = ((0 || (1-1)) || (3*4-4-4-4)) + 1 || 4; // 1
    int32_t l = (14+7*2-4*7 || (1+1+1-3)*(4 || 2)) || 0 || (0 || 0*( 1 || 2 ) || 0-4+5); // 1
    putchar('\n');
    putchar('0' + f);
    putchar('0' + f + g);
    putchar('0' + f + g + h);
    putchar('\n');
    putchar('0' + i);
    putchar('0' + i + k);
    putchar('0' + i + k + l);
    putchar('\n');
    foo(1, 0) || foo(2, 1) || foo(3, 0); // 12
    putchar('\n');

    return 0;
}