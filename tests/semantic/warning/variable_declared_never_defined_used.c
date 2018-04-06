/*
 * Send a warning for variables never used or defined
 */

#include <stdint.h>
#include <stdio.h>

extern int32_t a;

void main(){
    int32_t b;
}
