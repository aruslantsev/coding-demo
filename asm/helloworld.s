.data
msg:
    .ascii "Hello, world!\n"
    len = . - msg

/*
msg:
    db "Hello, world!",0x0A,0
    len equ $-msg
*/

.text
    .globl main        # entrypoint
    # .type   main, @function
main:
    movl  $4, %eax     # syscall 4 — sys_write
    movl  $1, %ebx     # stdout
    movl  $msg, %ecx   # pointer to string
    movl  $len, %edx   # length of the string
    int   $0x80        # syscall

    movl  $1, %eax     # syscall 1 — sys_exit
    xorl  %ebx, %ebx   # exitcode 0
    # movl $0, %ebx
    int   $0x80        # syscall
