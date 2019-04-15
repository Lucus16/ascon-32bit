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

	xor a0, a0, a4
	xor a4, a4, a3
	xor a2, a2, a1
	not t0, a0
	not t1, a1
	not t2, a2
	not t3, a3
	not t4, a4
	and t0, t0, a1
	and t1, t1, a2
	and t2, t2, a3
	and t3, t3, a4
	and t4, t4, a0
	xor a0, a0, t1
	xor a1, a1, t2
	xor a2, a2, t3
	xor a3, a3, t4
	xor a4, a4, t0
	xor a1, a1, a0
	xor a0, a0, a4
	xor a3, a3, a2
	not a2, a2

	xor s0, s0, s4
	xor s4, s4, s3
	xor s2, s2, s1
	not t0, s0
	not t1, s1
	not t2, s2
	not t3, s3
	not t4, s4
	and t0, t0, s1
	and t1, t1, s2
	and t2, t2, s3
	and t3, t3, s4
	and t4, t4, s0
	xor s0, s0, t1
	xor s1, s1, t2
	xor s2, s2, t3
	xor s3, s3, t4
	xor s4, s4, t0
	xor s1, s1, s0
	xor s0, s0, s4
	xor s3, s3, s2
	not s2, s2

	slli t0, a0, 13
	srli t2, s0, 19
	xor t0, t0, t2
	slli t2, a0, 4
	xor t0, t0, t2
	srli t2, s0, 28
	xor t0, t0, t2
	slli t1, s0, 13
	srli t2, a0, 19
	xor t1, t1, t2
	slli t2, s0, 4
	xor t1, t1, t2
	srli t2, a0, 28
	xor t1, t1, t2
	xor a0, a0, t1
	xor s0, s0, t0

	slli t0, s1, 3
	srli t2, a1, 29
	xor t0, t0, t2
	slli t2, s1, 25
	xor t0, t0, t2
	srli t2, a1, 7
	xor t0, t0, t2
	slli t1, a1, 3
	srli t2, s1, 29
	xor t1, t1, t2
	slli t2, a1, 25
	xor t1, t1, t2
	srli t2, s1, 7
	xor t1, t1, t2
	xor a1, a1, t1
	xor s1, s1, t0

	slli t0, a2, 31
	srli t2, s2, 1
	xor t0, t0, t2
	slli t2, a2, 26
	xor t0, t0, t2
	srli t2, s2, 6
	xor t0, t0, t2
	slli t1, s2, 31
	srli t2, a2, 1
	xor t1, t1, t2
	slli t2, s2, 26
	xor t1, t1, t2
	srli t2, a2, 6
	xor t1, t1, t2
	xor a2, a2, t1
	xor s2, s2, t0

	slli t0, a3, 22
	srli t2, s3, 10
	xor t0, t0, t2
	slli t2, a3, 15
	xor t0, t0, t2
	srli t2, s3, 17
	xor t0, t0, t2
	slli t1, s3, 22
	srli t2, a3, 10
	xor t1, t1, t2
	slli t2, s3, 15
	xor t1, t1, t2
	srli t2, a3, 17
	xor t1, t1, t2
	xor a3, a3, t1
	xor s3, s3, t0

	slli t0, a4, 25
	srli t2, s4, 7
	xor t0, t0, t2
	slli t2, s4, 23
	xor t0, t0, t2
	srli t2, a4, 9
	xor t0, t0, t2
	slli t1, s4, 25
	srli t2, a4, 7
	xor t1, t1, t2
	slli t2, a4, 23
	xor t1, t1, t2
	srli t2, s4, 9
	xor t1, t1, t2
	xor a4, a4, t1
	xor s4, s4, t0

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
