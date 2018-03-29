/*
 * Multi-line comment.
 */

#include <stdio.h>
#include <stdint.h>


void foo(
        int16_t a,
        int32_t b,
) {
    return 0;
}

int32_t main(int32_t, char argv[]) {
    int32_t tab[] = {
            1,
            2,
            3,
            //3,
    };
    return 0;
}
