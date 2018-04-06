/*
 * Error from ASTVisitor
 * You can only declare function in global
 */

#include <stdio.h>
#include <stdint.h>


void foo(int8_t p1, int8_t p2, int8_t p3);

int32_t main() {
    int32_t a, b = 0;
    int16_t c;

    void bar(int8_t pp1, int8_t pp2, int8_t pp3) {
        int8_t x = 0;
    }

    bar(16, 15, 14);

    return 0;
}
