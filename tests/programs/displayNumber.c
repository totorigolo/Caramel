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
        putchar('0' + tab[index]);
        number = number/10;
        index = index + 1;
    }
    putchar('a');
    putchar('\n');
    putchar('b');
    putchar('\n');

    i = 3;
//    putchar('0' + (i > 1-2));
//    putchar('\n');

    // TODO : manage signed integers in subtraction (... and others ? :x)
    for (i=(index - 1);i >= 0;i=i-1) {
        putchar('0' + tab[i]);
    }
    putchar('c');
    putchar('\n');
}

int32_t main() {

    displayNumber(20);

    return 0;
}