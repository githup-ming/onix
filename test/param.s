	.file	"param.c"
	.text
	.globl	add
	.type	add, @function
add:
	pushl	%ebp
	movl	%esp, %ebp

	subl	$4, %esp # 保存4个字节，1个局部变量

	movl	8(%ebp), %edx # x
	movl	12(%ebp), %eax # y
	addl	%edx, %eax # edx += eax

	movl	%eax, -4(%ebp) # z = x + y
	movl	-4(%ebp), %eax #eax = Z

	leave
	ret
	.size	add, .-add
	.globl	main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp #保存栈帧
	subl	$12, %esp # 保存12个字节，3个变量

	movl	$3, -12(%ebp) # a=3
	movl	$4, -8(%ebp) # b=4
	pushl	-8(%ebp)  #将b压入栈中
	pushl	-12(%ebp) #将a压入栈中
	call	add
	addl	$8, %esp # 恢复栈

	movl	%eax, -4(%ebp) # c = add(a,b);
	movl	$0, %eax # 返回值存在eax中

	leave #恢复栈帧
	ret  # 函数返回

	.size	main, .-main
	.section	.note.GNU-stack,"",@progbits
