# ARM

Ascon S-box in 15 cycles, with 3 temporary registers on ARM. This leaves 1
register available for e.g. the loop counter.

```C
t12 = x1 ^ x2;
t04 = x0 ^ x4;
t34 = x3 ^ x4;
x4 = x3 | ~x4;
x4 ^= t12;
x3 ^= x1;
x3 |= t12;
x3 ^= t04;
x2 ^= t04;
x2 |= x1;
x2 ^= t34;
x1 = x1 & ~t04;
x1 ^= t34;
x0 |= t34;
x0 ^= t12;
```
