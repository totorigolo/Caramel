#include <stdint.h>

void main() {
   int32_t a[10];
   int32_t i =2;
   a[0] = 1;
   a[1] = 1;

   while (i<10) {
      a[i] = a[i-1] + a[i-2];
      i++;
    }

}
