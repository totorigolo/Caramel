/*
 * MIT License
 *
 * Copyright (c) 2018 insa.4if.hexanome_kalate
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


int32_t main() {
    int32_t i = 5;

    putchar('0' + i); // 5
    putchar('0' + (i += 3)); // 8
    putchar('0' + i); // 8
    putchar('\n');

    putchar('0' + i); // 8
    putchar('0' + (i -= 6)); // 2
    putchar('0' + i); // 2
    putchar('\n');

    putchar('0' + i); // 2
    putchar('0' + (i *= 4)); // 8
    putchar('0' + i); // 8
    putchar('\n');

    putchar('0' + i); // 8
    putchar('0' + (i /= 2)); // 4
    putchar('0' + i); // 4
    putchar('\n');

    putchar('0' + i); // 4
    putchar('0' + (i %= 3)); // 1
    putchar('0' + i); // 1
    putchar('\n');

    putchar('0' + i); // 1
    putchar('0' + (i += 4)); // 5
    putchar('0' + i); // 5
    putchar('\n');

    putchar('0' + i); // 5
    putchar('0' + (i &= 4)); // 4
    putchar('0' + i); // 4
    putchar('\n');

    putchar('0' + i); // 4
    putchar('0' + (i |= 3)); // 7
    putchar('0' + i); // 7
    putchar('\n');

    putchar('0' + i); // 7
    putchar('0' + (i ^= 5)); // 2
    putchar('0' + i); // 2
    putchar('\n');

    putchar('0' + i); // 2
    putchar('0' + (i <<= 2)); // 8
    putchar('0' + i); // 8
    putchar('\n');

    putchar('0' + i); // 8
    putchar('0' + (i >>= 1)); // 4
    putchar('0' + i); // 4
    putchar('\n');

    return 0;
}
