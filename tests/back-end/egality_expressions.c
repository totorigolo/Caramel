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

int32_t main() {

    // 1
    int32_t a = 1 == 1;
    int32_t b = 2 > 1;
    int32_t c = 1 >= 1;
    int32_t d = 1 < 2;
    int32_t e = 1 <= 1;
    int32_t f = 1 != 0;
    // 0
    int32_t g = 1 == 0;
    int32_t h = 2 > 3;
    int32_t i = 1 >= 2;
    int32_t j = 3 < 2;
    int32_t k = 1 <= 0;
    int32_t l = 1 != 1;

    // complex 1
    int32_t m = (1 && (4-1) > 2 && 2 != 1) == (0 || 4-2*2 || (4-3));
    // complex 0
    int32_t n = (1 && (4-1) > 2 && 2 != 1) == (0 || 4-2*2 == 1 || (4-3) > 2);

    // not ?
//    int32_t o = !0;

    putchar('0' + a);
    putchar('0' + a + b);
    putchar('0' + a + b + c);
    putchar('0' + a + b + c + d);
    putchar('0' + a + b + c + d + e);
    putchar('0' + a + b + c + d + e + f);
    putchar('\n');
    putchar('0' + g);
    putchar('0' + g + h);
    putchar('0' + g + h + i);
    putchar('0' + g + h + i + j);
    putchar('0' + g + h + i + j + k);
    putchar('0' + g + h + i + j + k + l);
    putchar('\n');
    putchar('0' + m);
    putchar('0' + n);
    putchar('\n');

    if (m <= 1 && n == 0) {
        putchar('A');
        putchar('\n');
    } else {
        putchar('B');
        putchar('\n');
    }

//    putchar('\n');
//    putchar('0' + o);

    return 0;
}