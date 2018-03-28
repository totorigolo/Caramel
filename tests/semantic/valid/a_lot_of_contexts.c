/*
 * Multi-line comment.
 */

#include <stdio.h>
#include <stdint.h>

void foo(int8_t a, int8_t b, int8_t c);

int32_t main(int32_t, char argv) {
    int32_t a, b = 0;
    int16_t c;

    void foo(int8_t a, int8_t b, int8_t c) {
        void bar(int16_t param) {
            int8_t x = 0;
            void baz() {
                int32_t y = 1;
            }
        }
    }

    return 0;
}
