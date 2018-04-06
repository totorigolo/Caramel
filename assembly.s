.file  "displayNumber.c"
.text

.Ltext0:
  .globl main
  .type main, @function


displayNumber:
  pushq   %rbp
  movq    %rsp, %rbp
  # TODO: Prolog
  subq $1024, %rsp

  movl    %edi, -4(%rbp)
  jmp    .L1_innerbeginof_displayNumber

.L1_innerbeginof_displayNumber:
  movl    $0, %r10d
  movl    %r10d, -8(%rbp)
  movl    $0, %r10d
  movl    %r10d, -12(%rbp)
  movl    $0, %r10d
  movl    %r10d, -16(%rbp)
  movl    $0, %r10d
  movl    %r10d, -20(%rbp)
  movl    $0, %r10d
  movl    %r10d, -24(%rbp)
  movl    $0, %r10d
  movl    %r10d, -28(%rbp)
  movl    $0, %r10d
  movl    %r10d, -32(%rbp)
  movl    $0, %r10d
  movl    %r10d, -36(%rbp)
  movl    $0, %r10d
  movl    %r10d, -40(%rbp)
  movl    $0, %r10d
  movl    %r10d, -44(%rbp)
  movl    $0, %r10d
  movl    %r10d, -48(%rbp)
  movl    $0, %r10d
  movl    %r10d, -52(%rbp)
  movl    $0, %r10d
  movl    %r10d, -56(%rbp)
  movl    $0, %r10d
  movl    %r10d, -60(%rbp)
  movl    $0, %r10d
  movl    %r10d, -64(%rbp)
  movl    $0, %r10d
  movl    %r10d, -68(%rbp)
  movl    $0, %r10d
  movl    %r10d, -72(%rbp)
  movl    $0, %r10d
  movl    %r10d, -76(%rbp)
  movl    $0, %r10d
  movl    %r10d, -80(%rbp)
  movl    $0, %r10d
  movl    %r10d, -84(%rbp)
  movl    $0, %r10d
  movl    %r10d, -88(%rbp)
  movl    $0, %r10d
  movl    %r10d, -92(%rbp)
  movl    $0, %r10d
  movl    %r10d, -96(%rbp)
  movl    $0, %r10d
  movl    %r10d, -100(%rbp)
  movl    $0, %r10d
  movl    %r10d, -104(%rbp)
  movl    $0, %r10d
  movl    %r10d, -108(%rbp)
  movl    $0, %r10d
  movl    %r10d, -112(%rbp)
  movl    $0, %r10d
  movl    %r10d, -116(%rbp)
  movl    $0, %r10d
  movl    %r10d, -120(%rbp)
  movl    $0, %r10d
  movl    %r10d, -124(%rbp)
  movl    $0, %r10d
  movl    %r10d, -128(%rbp)
  movl    $0, %r10d
  movl    %r10d, -132(%rbp)
  movl    $0, %r10d
  movl    %r10d, -136(%rbp)
  movl    $0, %r10d
  movl    %r10d, -140(%rbp)
  movl    $0, %r10d
  movl    %r10d, -144(%rbp)
  movl    $0, %r10d
  movl    %r10d, -148(%rbp)
  movl    $0, %r10d
  movl    %r10d, -152(%rbp)
  movl    $0, %r10d
  movl    %r10d, -156(%rbp)
  movl    $0, %r10d
  movl    %r10d, -160(%rbp)
  movl    $0, %r10d
  movl    %r10d, -164(%rbp)
  movl    $0, %r10d
  movl    %r10d, -168(%rbp)
  movl    $0, %r10d
  movl    %r10d, -172(%rbp)
  movl    $0, %r10d
  movl    %r10d, -176(%rbp)
  movl    $0, %r10d
  movl    %r10d, -180(%rbp)
  movl    $0, %r10d
  movl    %r10d, -184(%rbp)
  movl    $0, %r10d
  movl    %r10d, -188(%rbp)
  movl    $0, %r10d
  movl    %r10d, -192(%rbp)
  movl    $0, %r10d
  movl    %r10d, -196(%rbp)
  movl    $0, %r10d
  movl    %r10d, -200(%rbp)
  movl    $0, %r10d
  movl    %r10d, -204(%rbp)
  nop
  movl    $0, %r10d
  movl    %r10d, -208(%rbp)
  movl    $0, %r10d
  movl    %r10d, -212(%rbp)
  jmp    .L2_Wcond

.L2_Wcond:
  # empty with returnName=number
  movl    -4(%rbp), %r11d
  pushq   %r11
  movl    $1, %r10d
  movl    %r10d, %r10d
  popq    %r11
  cmp     %r10d, %r11d
  setge    %r11b
  cmpl    $0, %r11d
  je    .L4_Wend
  jmp    .L3_Wthen

.L3_Wthen:
  # empty with returnName=number
  movl    -4(%rbp), %r11d
  pushq   %r11
  movl    $10, %r10d
  movl    %r10d, %r10d
  popq    %r11
  movl    %r11d, %eax
  cltd
  idivl    %r10d

  pushq   %rdx
  # empty with returnName=index
  movl    -208(%rbp), %r10d
  popq    %rax
  cltq
  movl %eax, -204(%rbp,%r10,4)
  movl    %edx, %eax
  movl    $48, %r10d
  movl    %r10d, %r11d
  pushq   %r11
  # empty with returnName=index
  movl    -208(%rbp), %r10d
  cltq
  movl -204(%rbp,%r10,4), %r10d
  movl    %r10d, %r10d
  popq    %r11
  movl    %r11d, %r11d
  addl    %r10d, %r11d
  pushq    %rdi
  movl    %r11d, %edi

  call    putchar
  popq     %rdi
  # empty with returnName=number
  # empty with returnName=number
  movl    -4(%rbp), %r11d
  pushq   %r11
  movl    $10, %r10d
  movl    %r10d, %r10d
  popq    %r11
  movl    %r11d, %eax
  cltd
  idivl    %r10d

  movl    %eax, -4(%rbp)
  # empty with returnName=index
  # empty with returnName=index
  movl    -208(%rbp), %r11d
  pushq   %r11
  movl    $1, %r10d
  movl    %r10d, %r10d
  popq    %r11
  movl    %r11d, %r11d
  addl    %r10d, %r11d
  movl    %r11d, -208(%rbp)
  jmp    .L2_Wcond

.L4_Wend:
  jmp    .L5_funcinnerafter

.L5_funcinnerafter:
  movl    $97, %r10d
  pushq    %rdi
  movl    %r10d, %edi

  call    putchar
  popq     %rdi
  movl    $10, %r10d
  pushq    %rdi
  movl    %r10d, %edi

  call    putchar
  popq     %rdi
  movl    $98, %r10d
  pushq    %rdi
  movl    %r10d, %edi

  call    putchar
  popq     %rdi
  movl    $10, %r10d
  pushq    %rdi
  movl    %r10d, %edi

  call    putchar
  popq     %rdi
  # empty with returnName=i
  movl    $3, %r10d
  movl    %r10d, -212(%rbp)
  jmp    .L6_Finit

.L6_Finit:
  # empty with returnName=i
  # empty with returnName=index
  movl    -208(%rbp), %r11d
  pushq   %r11
  movl    $1, %r10d
  movl    %r10d, %r10d
  popq    %r11
  movl    %r11d, %r11d
  subl    %r10d, %r11d
  movl    %r11d, -212(%rbp)
  jmp    .L7_Fcond

.L7_Fcond:
  # empty with returnName=i
  movl    -212(%rbp), %r11d
  pushq   %r11
  movl    $0, %r10d
  movl    %r10d, %r10d
  popq    %r11
  cmp     %r10d, %r11d
  setge    %r11b
  cmpl    $0, %r11d
  je    .L10_Fend
  jmp    .L8_Fthen

.L8_Fthen:
  movl    $48, %r10d
  movl    %r10d, %r11d
  pushq   %r11
  # empty with returnName=i
  movl    -212(%rbp), %r10d
  cltq
  movl -204(%rbp,%r10,4), %r10d
  movl    %r10d, %r10d
  popq    %r11
  movl    %r11d, %r11d
  addl    %r10d, %r11d
  pushq    %rdi
  movl    %r11d, %edi

  call    putchar
  popq     %rdi
  jmp    .L9_Finc

.L9_Finc:
  # empty with returnName=i
  # empty with returnName=i
  movl    -212(%rbp), %r11d
  pushq   %r11
  movl    $1, %r10d
  movl    %r10d, %r10d
  popq    %r11
  movl    %r11d, %r11d
  subl    %r10d, %r11d
  movl    %r11d, -212(%rbp)
  jmp    .L7_Fcond

.L10_Fend:
  jmp    .L11_funcinnerafter

.L11_funcinnerafter:
  movl    $99, %r10d
  pushq    %rdi
  movl    %r10d, %edi

  call    putchar
  popq     %rdi
  movl    $10, %r10d
  pushq    %rdi
  movl    %r10d, %edi

  call    putchar
  popq     %rdi
  jmp    .L0_endof_displayNumber

.L0_endof_displayNumber:
  leave
  ret

main:
  pushq   %rbp
  movq    %rsp, %rbp
  # TODO: Prolog
  subq $1024, %rsp

  jmp    .L13_innerbeginof_main

.L13_innerbeginof_main:
  movl    $20, %r10d
  pushq    %rdi
  movl    %r10d, %edi

  call    displayNumber
  popq     %rdi
  movl    $0, %r10d
  movl    %r10d, %eax
  jmp    .L12_endof_main
  jmp    .L12_endof_main

.L12_endof_main:
  leave
  ret

