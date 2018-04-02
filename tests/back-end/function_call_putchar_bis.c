/*
 * Multi-line comment.
 */
#include <stdio.h>
#include <stdint.h>

void remote_write(int32_t c) {
    putchar(c);
}

void remote_write_2(int32_t _, int32_t c) {
    putchar(c);
}

int32_t main() {
    remote_write('h');
    remote_write('i');
    remote_write('!');
    remote_write_2('?', '\n');
    return 0;
}
