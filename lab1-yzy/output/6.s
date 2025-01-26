	.text
	.file	"6.c"
	.section	.rodata.cst4,"aM",@progbits,4
	.p2align	2                               # -- Begin function main
.LCPI0_0:
	.long	0x3f800000                      # float 1
	.text
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	$0, -24(%rbp)
	movl	$2, -8(%rbp)
	movl	$1, -12(%rbp)
	movl	$0, -4(%rbp)
.LBB0_1:                                # =>This Inner Loop Header: Depth=1
	cmpl	$15, -4(%rbp)
	jge	.LBB0_8
# %bb.2:                                #   in Loop: Header=BB0_1 Depth=1
	movss	c, %xmm0                        # xmm0 = mem[0],zero,zero,zero
	movss	.LCPI0_0(%rip), %xmm1           # xmm1 = mem[0],zero,zero,zero
	ucomiss	%xmm1, %xmm0
	jne	.LBB0_4
	jp	.LBB0_4
# %bb.3:
	jmp	.LBB0_9
.LBB0_4:                                #   in Loop: Header=BB0_1 Depth=1
	cmpl	$10, -12(%rbp)
	jne	.LBB0_6
# %bb.5:                                #   in Loop: Header=BB0_1 Depth=1
	jmp	.LBB0_7
.LBB0_6:                                #   in Loop: Header=BB0_1 Depth=1
	movl	-12(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -12(%rbp)
	movss	c, %xmm0                        # xmm0 = mem[0],zero,zero,zero
	cvtsi2ssl	-8(%rbp), %xmm1
	divss	%xmm1, %xmm0
	addss	c, %xmm0
	movss	%xmm0, c
.LBB0_7:                                #   in Loop: Header=BB0_1 Depth=1
	movl	-4(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -4(%rbp)
	jmp	.LBB0_1
.LBB0_8:                                # %.loopexit
	jmp	.LBB0_9
.LBB0_9:
	movl	-12(%rbp), %esi
	movss	c, %xmm0                        # xmm0 = mem[0],zero,zero,zero
	cvtss2sd	%xmm0, %xmm0
	movabsq	$.L.str, %rdi
	movb	$1, %al
	callq	printf@PLT
	xorl	%eax, %eax
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	c,@object                       # @c
	.data
	.globl	c
	.p2align	2
c:
	.long	0x407851ec                      # float 3.88000011
	.size	c, 4

	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"a[2]=%d c=%f"
	.size	.L.str, 13

	.ident	"Ubuntu clang version 14.0.0-1ubuntu1.1"
	.section	".note.GNU-stack","",@progbits
