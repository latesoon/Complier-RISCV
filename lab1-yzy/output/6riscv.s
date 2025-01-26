	.file	"6.c"
	.option nopic
	.attribute arch, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.globl	c
	.section	.sdata,"aw"
	.align	2
	.type	c, @object
	.size	c, 4
c:
	.word	1081627116
	.section	.rodata
	.align	3
.LC1:
	.string	"a[2]=%d c=%f"
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-48
	sd	ra,40(sp)
	sd	s0,32(sp)
	addi	s0,sp,48
	li	a5,2
	sw	a5,-24(s0)
	li	a5,1
	sw	a5,-32(s0)
	sw	zero,-20(s0)
	j	.L2
.L7:
	lui	a5,%hi(c)
	flw	fa4,%lo(c)(a5)
	lui	a5,%hi(.LC0)
	flw	fa5,%lo(.LC0)(a5)
	feq.s	a5,fa4,fa5
	bne	a5,zero,.L9
	lw	a5,-32(s0)
	mv	a4,a5
	li	a5,10
	beq	a4,a5,.L10
	lw	a5,-32(s0)
	addiw	a5,a5,1
	sext.w	a5,a5
	sw	a5,-32(s0)
	lui	a5,%hi(c)
	flw	fa4,%lo(c)(a5)
	lw	a5,-24(s0)
	fcvt.s.w	fa5,a5
	fdiv.s	fa4,fa4,fa5
	lui	a5,%hi(c)
	flw	fa5,%lo(c)(a5)
	fadd.s	fa5,fa4,fa5
	lui	a5,%hi(c)
	fsw	fa5,%lo(c)(a5)
	j	.L6
.L10:
	nop
.L6:
	lw	a5,-20(s0)
	addiw	a5,a5,1
	sw	a5,-20(s0)
.L2:
	lw	a5,-20(s0)
	sext.w	a4,a5
	li	a5,14
	ble	a4,a5,.L7
	j	.L4
.L9:
	nop
.L4:
	lw	a4,-32(s0)
	lui	a5,%hi(c)
	flw	fa5,%lo(c)(a5)
	fcvt.d.s	fa5,fa5
	fmv.x.d	a2,fa5
	mv	a1,a4
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	printf
	li	a5,0
	mv	a0,a5
	ld	ra,40(sp)
	ld	s0,32(sp)
	addi	sp,sp,48
	jr	ra
	.size	main, .-main
	.section	.rodata
	.align	2
.LC0:
	.word	1065353216
	.ident	"GCC: () 12.2.0"
	.section	.note.GNU-stack,"",@progbits
