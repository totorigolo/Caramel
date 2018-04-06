/*
* Multi-line comment.
*/
#include <stdio.h>
#include <stdint.h>


int32_t foo(int32_t a[], int32_t b) {
    putchar('0' + a[b]); // 3
    putchar('\n');
}

int32_t main() {
    int32_t tab[] = {1, 2, 3, 4, 5};
    foo(tab, 2);
    return 0;
}

/*
foo:
  movl -12(%rbp), %eax
  cltq
  leaq 0(,%rax,4), %rdx
  movq -8(%rbp), %rax
  addq %rdx, %rax
  movl (%rax), %eax
  movl %eax, %edi
  call putchar

main:
  leaq -32(%rbp), %rax
  movl $2, %esi
  movq %rax, %rdi
  call foo
 */
