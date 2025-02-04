.section .data
.set NN, 30000
.set SIZE, 4 /* dd int */
/* length:
    .int NN
temp:
    .int 0
*/

.section .bss
array:
    .space NN * SIZE 

.section .text
    .globl main
main:
    movl $0, %ecx
    movl $array, %ebx
    movl $NN, %eax
    /* fill array */
form:
    movl %eax, (%ebx, %ecx, SIZE)
    inc %ecx
    dec %eax
    cmpl $NN, %ecx
    jne form
    /* filled from nn downto 1*/
    
    /* sort */
    movl $1, %esi /* outer counter. 1..NN eq 0..NN-1 */
    movl $NN, %edi /* max rpt of inner loop */
    dec %edi /* j = 1 to i - 1*/
    movl $array, %ebx
big_l:
    movl $0, %ecx /* inner counter */
small_l:
    movl (%ebx, %ecx, SIZE), %eax
    movl SIZE(%ebx, %ecx, SIZE), %edx
    cmp %edx, %eax
    jle nochange /* dont change if ok */
    movl %edx, (%ebx, %ecx, SIZE)
    movl %eax, SIZE(%ebx, %ecx, SIZE)
nochange:
    inc %ecx 
    cmp %ecx, %edi
    jne small_l
    inc %esi
    dec %edi
    cmpl $NN, %esi
    jne big_l
    
    movq  $1, %rax /*movq  $1, %rax*/
    xorq  %rbx, %rbx
    int   $0x80
