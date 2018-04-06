/*
 * Multi-line comment.
 */

#include <stdint.h>
#include <stdio.h>



int32_t main() {
    int32_t a = 0;
    if (1) {
        a = 1;
        if (0) {
            a = 1;
        } else if (1) {
            a = 1;
        } else {
            a = 1;
        }
    } else {
        if (0) {
            a = 1;
        } else if (0) {
            a = 1;
        } else {
            a = 1;
        }
    }
    return 0;
}