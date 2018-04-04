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

/*
 * Multi-line comment.
 */
#include <stdio.h>
#include <stdint.h>

int32_t main() {
    int32_t a = '0';
    int32_t b;
    putchar(a);
    putchar('\n');

    b = a + 1;
    putchar(b);
    putchar('\n');

    b = b + 2 - 1;
    putchar(b);
    putchar('\n');

    b = ((b + 3) - 3) + 1;
    putchar(b);
    putchar('\n');

    b = b + (1 + 2 + 3 - (5 + 1)) + 1;
    putchar(b);
    putchar('\n');

//    putchar(a);

//    b = a - 1;
//    putchar(b);
//    putchar('\n');

    return 0;
}
