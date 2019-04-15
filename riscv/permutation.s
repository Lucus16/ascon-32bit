# xorror applies the rotates and xors of the linear diffusion layer to
# one 64-bit state word. It only changes the values in the temporary and
# destination registers. Arguments:
# Destination Low and High
# Source Low and High, for Rotations 0 and 1
# These may be swapped to support rotation amounts above 32
# Temporary registers 0, 1 and 2
.macro xorror dl, dh, sl, sh, sl0, sh0, r0, sl1, sh1, r1, t0, t1, t2
	slli \t0, \sl0, (32 - \r0)
	srli \t2, \sh0, \r0
	xor \t0, \t0, \t2
	slli \t2, \sl1, (32 - \r1)
	xor \t0, \t0, \t2
	srli \t2, \sh1, \r1
	xor \t0, \t0, \t2
	slli \t1, \sh0, (32 - \r0)
	srli \t2, \sl0, \r0
	xor \t1, \t1, \t2
	slli \t2, \sh1, (32 - \r1)
	xor \t1, \t1, \t2
	srli \t2, \sl1, \r1
	xor \t1, \t1, \t2
	xor \dl, \sl, \t1
	xor \dh, \sh, \t0
.endm

.text

.globl permutation
.align 2
permutation:
	# a0 state pointer
	# a1 round constant

	addi sp, sp, -20
	sw s0, (sp)
	sw s1, 4(sp)
	sw s2, 8(sp)
	sw s3, 12(sp)
	sw s4, 16(sp)

	mv t6, a0
	li a5, 0x3c
	li t5, 0xf
	mul t5, t5, a1
	add t5, t5, a5

	# t6 state pointer
	# t5 round constant
	# a5 final round constant

	lw a4, (t6)
	lw s4, 4(t6)
	lw a3, 8(t6)
	lw s3, 12(t6)
	lw a2, 16(t6)
	lw s2, 20(t6)
	lw a1, 24(t6)
	lw s1, 28(t6)
	lw a0, 32(t6)
	lw s0, 36(t6)

	# a0-a4 lower halves of state
	# s0-s4 upper halves of state

round:
	xor a2, a2, t5

	xor t0, a1, a2
	xor t1, a0, a4
	xor t2, a3, a4
	not a4, a4
	or a4, a4, a3
	xor a4, a4, t0
	xor a3, a3, a1
	or a3, a3, t0
	xor a3, a3, t1
	xor a2, a2, t1
	or a2, a2, a1
	xor a2, a2, t2
	not t1, t1
	and a1, a1, t1
	xor a1, a1, t2
	or a0, a0, t2
	xor t3, a0, t0

	xor t0, s1, s2
	xor t1, s0, s4
	xor t2, s3, s4
	not s4, s4
	or s4, s4, s3
	xor s4, s4, t0
	xor s3, s3, s1
	or s3, s3, t0
	xor s3, s3, t1
	xor s2, s2, t1
	or s2, s2, s1
	xor s2, s2, t2
	not t1, t1
	and s1, s1, t1
	xor s1, s1, t2
	or s0, s0, t2
	xor t4, s0, t0

	xorror a0, s0, a2, s2, a2, s2, 19, a2, s2, 28, t0, t1, t2
	xorror a2, s2, a4, s4, a4, s4, 1,  a4, s4, 6,  t0, t1, t2
	xorror a4, s4, a1, s1, a1, s1, 7,  s1, a1, 9,  t0, t1, t2
	xorror a1, s1, a3, s3, s3, a3, 29, s3, a3, 7,  t0, t1, t2
	xorror a3, s3, t3, t4, t3, t4, 10, t3, t4, 17, t0, t1, t2

	addi t5, t5, -15
	bne a5, t5, round

	sw a4, (t6)
	sw s4, 4(t6)
	sw a3, 8(t6)
	sw s3, 12(t6)
	sw a2, 16(t6)
	sw s2, 20(t6)
	sw a1, 24(t6)
	sw s1, 28(t6)
	sw a0, 32(t6)
	sw s0, 36(t6)

	lw s0, (sp)
	lw s1, 4(sp)
	lw s2, 8(sp)
	lw s3, 12(sp)
	lw s4, 16(sp)
	addi sp, sp, 20

	ret
