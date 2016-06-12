#hello.s
.data
	msg:.string "Hello, world!\n"
	len=. - msg
.text
.global _start
_start:
	movl $len, %edx #message len
	movl $msg, %ecx #message 
	movl $1, %ebx	#fd stdout
	movl $4, %eax	#call sys_write
	int  $0x80
	movl $0, %ebx
	movl $1, %eax   #call sys_exit
	int  $0x80
