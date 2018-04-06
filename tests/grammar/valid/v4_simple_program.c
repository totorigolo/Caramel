#include <stdint.h>
#include <stdio.h>


int main(int, char) {
    int i;
    puts('c');
    while(i < 10) {
        if (i % 2 == 0) {
            put('a' + i);
        }
        i++;
    }
    return 0;
}