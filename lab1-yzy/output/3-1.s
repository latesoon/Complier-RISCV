	.attribute arch, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.section .data
	.globl pi
	.type pi,@object
	.size pi,4
pi:
	.zero 4
	.section .rodata
.LC0:
	.string "%d"
.LC1:
	.string "pi=%.5f\n"
.LC2:
	.word 1065353216
.LC3:
	.word 1073741824
.LC4:
	.word 1082130432
	.globl main
	.type main.@function
main:
	addi sp,sp,-32
	sd ra,24(sp)
	sd s0,16(sp)
	addi s0,sp,32
	lui a1,%hi(.LC2)
	flw fa1,%lo(.LC2)(a1)
	fsw fa1,-20(s0)
	lui a2,%hi(.LC0)
	addi a0,a2,%lo(.LC0)
	addi a1,s0,-28
	call __isoc99_scanf
	mv a2,zero
	sw a2,-24(s0)
.L1:
	lw a1,-28(s0)
	lw a2,-24(s0)
	blt a2,a1,.L2
	j .L5
.L2:	
	lw a2,-24(s0)
	andi a2,a2,1
	beq a2,zero,.L3
	lui a2,%hi(pi)
	flw fa2,%lo(pi)(a2)
	lui a3,%hi(.LC2)
	flw fa3,%lo(.LC2)(a3)
	flw fa1,-20(s0)
	fdiv.s fa3,fa3,fa1
	fsub.s fa2,fa2,fa3
	fsw fa2,%lo(pi)(a2)
	j .L4
.L3:
	lui a2,%hi(pi)
        flw fa2,%lo(pi)(a2)
        lui a3,%hi(.LC2)
        flw fa3,%lo(.LC2)(a3)
        flw fa1,-20(s0)
        fdiv.s fa3,fa3,fa1
        fadd.s fa2,fa2,fa3
        fsw fa2,%lo(pi)(a2)
.L4:
	lui a4,%hi(.LC3)
	flw fa4,%lo(.LC3)(a4)
	fadd.s fa1,fa1,fa4
	fsw fa1,-20(s0)
	lw a1,-24(s0)
	addiw a1,a1,1
	sw a1,-24(s0)
	j .L1
.L5:
	lui a2,%hi(pi)
        flw fa2,%lo(pi)(a2)
	lui a4,%hi(.LC4)
        flw fa4,%lo(.LC4)(a4)
	fmul.s fa2,fa2,fa4
	fcvt.d.s fa2,fa2
	fmv.x.d a1,fa2
	lui a4,%hi(.LC1)
	addi a0,a4,%lo(.LC1)
	call printf
	li a0,0
	ld ra,24(sp)
	ld s0,16(sp)
	addi sp,sp,32
	jr ra
	.size	main, .-main
