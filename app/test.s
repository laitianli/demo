	.file	"test.c"
	.section	.rodata
.LC0:
	.string	"a = %d\n"
.LC1:
	.string	"(int)a = 0x%x\n"
.LC2:
	.string	"c"
	.align 8
.LC3:
	.string	"VSYSCALL_END(-2UL << 20):0x%x\n"
	.align 8
.LC4:
	.string	"VSYSCALL_START(-10UL << 20): 0x%x\n"
.LC5:
	.string	"VSYSCALL_FIRST_CALL: 0x%x\n"
	.text
.globl main
	.type	main, @function
main:
.LFB2:
	pushq	%rbp
.LCFI0:
	movq	%rsp, %rbp
.LCFI1:
	subq	$32, %rsp
.LCFI2:
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	movb	$-74, -7(%rbp)
	movw	$-18944, -6(%rbp)
	movl	$-1241513984, -4(%rbp)
	movsbl	-7(%rbp),%esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movsbl	-7(%rbp),%esi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	cmpl	$-1241513984, -4(%rbp)
	jne	.L2
	movl	$.LC2, %edi
	call	puts
.L2:
	movq	$-2097152, %rsi
	movl	$.LC3, %edi
	movl	$0, %eax
	call	printf
	movq	$-10485760, %rsi
	movl	$.LC4, %edi
	movl	$0, %eax
	call	printf
	movl	$2047, %esi
	movl	$.LC5, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	leave
	ret
.LFE2:
	.size	main, .-main
	.section	.eh_frame,"a",@progbits
.Lframe1:
	.long	.LECIE1-.LSCIE1
.LSCIE1:
	.long	0x0
	.byte	0x1
	.string	"zR"
	.uleb128 0x1
	.sleb128 -8
	.byte	0x10
	.uleb128 0x1
	.byte	0x3
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.byte	0x90
	.uleb128 0x1
	.align 8
.LECIE1:
.LSFDE1:
	.long	.LEFDE1-.LASFDE1
.LASFDE1:
	.long	.LASFDE1-.Lframe1
	.long	.LFB2
	.long	.LFE2-.LFB2
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB2
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xd
	.uleb128 0x6
	.align 8
.LEFDE1:
	.ident	"GCC: (GNU) 4.1.2 20080704 (Red Hat 4.1.2-55)"
	.section	.note.GNU-stack,"",@progbits
