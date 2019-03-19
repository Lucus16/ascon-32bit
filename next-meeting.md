- why recommend block size 64 with 6 rounds over block size 128 with 8 rounds
  when the latter is faster? Is the former more secure?
- Should I implement both?

- Optimizing S-Box Implementations for Several Criteria Using SAT Solvers - Ko
  Stoffelen
    - 10,11 UNSAT (~4cpuh, ~200cpuh)
- GNU superoptimizer will be even slower
- What should I write about the instruction sequence I invented with
  unstructured thinking?

- Caesar selection argumentation?
- Save my CE grade for next year.

- optimized riscv sbox from 44 to 34
- optimized arm32 sbox from 34 to 30 ops, from 15 to 13 registers

- on riscv, pC is 2 cycles, pL is 80 cycles, pS is 34 cycles,
  looping is 2 cycles
- on aarch32, pC is ? cycles, pL is 20 cycles, pS is 30 cycles
- on aarch64, pC is 2 cycles, pL is 10 cycles, pS is 15 cycles
