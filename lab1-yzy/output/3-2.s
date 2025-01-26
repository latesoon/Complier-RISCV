	.attribute arch, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.section .bss
	.globl	nums
	.type	nums, @object
	.size	nums, 40
nums:
	.zero	40
	.section .rodata
.LC0:
	.string "%d"
.LC1:
	.string "%d "
	.text
	.globl sort
	.type sort,@function
sort:
	addi sp,sp,-32
	sd s0,24(sp)
	addi s0,sp,32
	sw a0,-20(s0)
	sw zero,-16(s0)
.L1:
	lw a1,-20(s0)
	lw a2,-16(s0)
	blt a2,a1,.L2
	j .L7
.L2:
	addiw a2,a2,1
	sw a2,-12(s0)
.L3:
	lw a1,-20(s0)
	lw a2,-12(s0)
	blt a2,a1,.L4
	j .L6
.L4:
	lui a3,%hi(nums)
	addi a3,a3,%lo(nums)
	lw a1,-16(s0)
	slli a1,a1,2
	add a1,a1,a3
	lw a4,0(a1)
	lw a2,-12(s0)
	slli a2,a2,2
	add a2,a2,a3
	lw a5,0(a2)
	ble a4,a5,.L5
	sw a5,0(a1)
	sw a4,0(a2)
.L5:
	lw a2,-12(s0)
	addiw a2,a2,1
	sw a2,-12(s0)
	j .L3
.L6:
	lw a2,-16(s0)
	addiw a2,a2,1
	sw a2,-16(s0)
	j .L1	
.L7:
	ld s0,24(sp)
	addi sp,sp,32
	jr ra
	.globl main
	.type main,@function
main:
	addi sp,sp,-32
	sd ra,24(sp)
	sd s0,16(sp)
	addi s0,sp,32
	addi a1,s0,-28
	lui a0,%hi(.LC0)
	addi a0,a0,%lo(.LC0)
	call __isoc99_scanf
	sw zero,-20(s0)
.L8:
	lw a1,-28(s0)
	lw a2,-20(s0)
	ble a1,a2,.L9
	slli a2,a2,2
	lui a3,%hi(nums)
	addi a3,a3,%lo(nums)
	add a1,a3,a2
	lui a0,%hi(.LC0)
        addi a0,a0,%lo(.LC0)
        call __isoc99_scanf
	lw a2,-20(s0)
	addiw a2,a2,1
	sw a2,-20(s0)
	j .L8
.L9:
	lw a0,-28(s0)
	call sort
	sw zero,-20(s0)
.L10:
	lw a1,-28(s0)
	lw a2,-20(s0)
	ble a1,a2,.L11
	slli a2,a2,2
        lui a3,%hi(nums)
        addi a3,a3,%lo(nums)
        add a1,a3,a2
        lw a1,0(a1)
	lui a0,%hi(.LC1)
        addi a0,a0,%lo(.LC1)
	call printf
	lw a2,-20(s0)
	addiw a2,a2,1
	sw a2,-20(s0)
	j .L10
.L11:
	li a0,10
	call putchar
	li a0,0
	ld ra,24(sp)
	ld s0,16(sp)
	addi sp,sp,32
	jr ra
	.size	main, .-main

