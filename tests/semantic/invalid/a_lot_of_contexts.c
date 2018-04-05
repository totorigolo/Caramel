/*
 * Multi-line comment.
 */

#include <stdio.h>
#include <stdint.h>

void foo(int8_t p1, int8_t p2, int8_t p3);

int32_t main() {
    int32_t a, b = 0;
    int16_t c;

    void foo(int8_t p1, int8_t p2, int8_t p3) {
        void bar(int16_t param) {
            int8_t x = 0;
            void baz() {
                int32_t y = 1 + p1;
            }
            baz();
        }
        bar(5);
    }

    foo(16, 15, 14);

    return 0;
}
