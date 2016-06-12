	.file	"puts.c"
	.text
.globl fun_add
	.type	fun_add, @function
fun_add:
	pushl	%ebp
	movl	%esp, %ebp
	movl	12(%ebp), %eax
	addl	8(%ebp), %eax
	popl	%ebp
	ret
	.size	fun_add, .-fun_add
.globl main
	.type	main, @function
main:
	leal	4(%esp), %ecx
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ecx
	subl	$24, %esp
	movl	$0, -8(%ebp)
	movl	$5, 4(%esp)
	movl	$3, (%esp)
	call	fun_add
	movl	%eax, -8(%ebp)
	movl	$0, %eax
	addl	$24, %esp
	popl	%ecx
	popl	%ebp
	leal	-4(%ecx), %esp
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.1.2 20080704 (Red Hat 4.1.2-55)"
	.section	.note.GNU-stack,"",@progbits
