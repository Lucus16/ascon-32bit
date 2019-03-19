# Optimizing Ascon on 32-bits architectures

I'm Lars Jellema, my supervisors are Peter Schwabe and Christoph Dobraunig. My
thesis is about optimizing Ascon for 32-bits architectures, which is in the
field of cryptographic engineering and optimization. Ascon is designed to run
fast on 64-bits architectures. The purpose of this thesis is to show that Ascon
is a good contender in the Caesar competition because it can even be made fast
on 32-bits architectures. I will look at the three separate phases of the Ascon
inner loop and try to optimize each one on a variety of 32-bits architectures,
starting with RISC-V. I will then compare the different techniques that are
effective on different architectures. I expect to be able to save only a few
percent on RISC-V, compared to a trivial implementation, but potentially more on
more complicated 32-bit architectures.

# Planning

- week  9 - performance and correctness test environment
- week 10 - basic riscv implementation and first optimization
- week 11 - try using gnu superoptimizer / Ko-/sboxoptimization
- week 12 - set up tools and test for second architecture
- week 13 - basic implementation
- week 14 - design optimizations
- week 15 - implement optimizations
- week 16 - set up tools and test for third architecture
- week 17 - basic implementation
- week 18 - break
- week 19 - design optimizations
- week 20 - implement optimizations
- week 21 - compare possible optimizations and tradeoffs
- week 22 - submit preliminary thesis document
- week 23 - prepare final presentation
- week 24 - handle feedback
- week 25 - submit final thesis document
- week 26 - final presentation
