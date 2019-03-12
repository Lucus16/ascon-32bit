# ARM

Ascon S-box in 15 cycles, with 3 temporary registers on ARM. This leaves 1
register available for e.g. the loop counter.

```C
t12 = r1 ^ r2;
t04 = r0 ^ r4;
t34 = r3 ^ r4;
r4 = r3 & ~r4;
r4 ^= t12;
r3 ^= r1;
r3 |= t12;
r3 ^= t04;
r2 ^= t04;
r2 |= r1;
r2 ^= t34;
r1 = r1 & ~t04;
r1 ^= t34;
r0 |= t34;
r0 ^= t12;
```
