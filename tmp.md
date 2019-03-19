i4^i3   i1  i4^i0
i2^i1   i0  i4^i3
i2^i1   i4  i3
i4^i3   i1  i2  i4^i0
i4^i0   i3  i1  i2^i1

t0 = 
t0 = ~i4; t0 |= i3; o2 = t0 ^ i2^i1;
t0 = i2 ^ i4^i0; t0 |= i1; o0 = t0 ^ i4^i3;
t0 = i3 ^ i1; t0 |= i2^i1; o1 = t0 ^ i4^i0;


o0  i2  i3  i0^i4   o4

order of need

i2^i1
i4^i3
i4^i0

i3  i4  i2^i1
i3  i1  i2^i1   i4^i0
i2  i4^i0   i1  i4^i3
i1  i4^i0   i4^i3
i0  i4^i3   i2^i1

t12 = r1 ^ r2;
t04 = r0 ^ r4;
t34 = r3 ^ r4;
r4 = r3 | ~r4;
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

r0, r1, r2, r3, r4 = r2, r3, r4, r0, r1
