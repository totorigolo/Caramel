#include <stdint.h>

int32_t function(int32_t a[], int32_t n) {
   int32_t i;
   int32_t result = 0;
   int32_t i = 0;
   while (i < n) {
      result = result+a[i];
      i += 1;
   }
   return result;
}
   
void main() {
   int32_t a[10];
   int32_t i;
   int32_t r;
   while (i < n) {
      a[i] = i;
      i += 1;
   }
   r = function(a,10);
}
