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

int32_t ackermann(int32_t x, int32_t y)
{
    if(x<0 || y<0) {
//        putchar('A');
        return 0-1;
    }
    if(x==0) {
//        putchar('B');
        return y+1;
    }
    if(y==0) {
//        putchar('C');
        return ackermann(x-1,1);
    }
//    putchar('D');
    return ackermann(x-1,ackermann(x,y-1));
}

int32_t main() {
    displayNumber(ackermann(3, 3));
    putchar('\n');

    return 0;
}