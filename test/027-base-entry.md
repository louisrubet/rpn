# fix entry

`default del`

## table for fixed entry, 4 fix output >= 0

```
4 fix
1 0.01 0.0001 0.00006 0.00004 0 0.012 0.001256 100 100.001 100.00006 100.00004 12345678910111213.12355
```

-> stack should be 1.0000, 0.0100, 0.0001, 0.0001, 0.0000, 0.0000, 0.0120, 0.0013, 100.0000, 100.0010, 100.0001, 100.0000, 12345678910111213.1236

`del default`

## table for fixed entry, 4 fix output <= 0

```
4 fix
-1 -0.01 -0.0001 -0.00006 -0.00004 -0 -0.012 -0.001256 -100 -100.001 -100.00006 -100.00004 -12345678910111213.12355
```

-> stack should be -1.0000, -0.0100, -0.0001, -0.0001, -0.0000, -0.0000, -0.0120, -0.0013, -100.0000, -100.0010, -100.0001, -100.0000, -12345678910111213.1236

`del default`

## table for sci entry, 4 fix output >= 0

```
4 fix
0e100 1e0 1.e0 1.001e0 1.e-3 1.e-4 6.e-5 4.e-5 1.00001e2 1.0000006e2 1.0000004e2 1234.5678917e2
```

-> stack should be 0.0000, 1.0000, 1.0000, 1.0010, 0.0010, 0.0001, 0.0001, 0.0000, 100.0010, 100.0001, 100.0000, 123456.7892

`del default`

## table for sci entry, 4 fix output <= 0

```
4 fix
-0e100 -1e0 -1.e0 -1.001e0 -1.e-3 -1.e-4 -6.e-5 -4.e-5 -1.00001e2 -1.0000006e2 -1.0000004e2 -1234.5678917e2
```

-> stack should be -0.0000, -1.0000, -1.0000, -1.0010, -0.0010, -0.0001, -0.0001, -0.0000, -100.0010, -100.0001, -100.0000, -123456.7892

`del default`

## table for singularity

```
4 fix
nan @nan@ -nan inf -inf @inf@ -@inf@
```

-> stack should be nan, nan, nan, inf, -inf, inf, -inf

`del default`

## some strange behaviour (1)

```
0 fix
1 2 / dup +
```

-> stack should be 1

`del default`

## some strange behaviour (2)

```
1 fix
0.6
```

-> stack should be 0.6

`del default`

## some strange behaviour (3)

```
0 fix
110.6 0.6
```

-> stack should be 111, 1

`del default`

## hex (1)

`0x4000000000000`

-> stack should be 0x4000000000000

`del default`

## hex (2)

`2 50 ^ hex`

-> stack should be 0x4000000000000

`del default`

## hex (3)

`12.34 hex`

-> stack should be 0xc

`del default`

## dec (1)

`2 50 ^`

-> stack should be 1125899906842624

`del default`

## dec (2)

`0x4000000000000 dec`

-> stack should be 1125899906842624

`del default`

## dec (3)

`12.34 dec`

-> stack should be 12.34

`del default`

## bin (1)

`0b100000000000000000000000000000000000000000000000000 bin`

-> stack should be 0b100000000000000000000000000000000000000000000000000

`del default`

## bin (2)

`2 50 ^ bin`

-> stack should be 0b100000000000000000000000000000000000000000000000000

`del default`

## bin (3)

`0x4000000000000 bin`

-> stack should be 0b100000000000000000000000000000000000000000000000000

`del default`

## bin (4)

`12.34 bin`

-> stack should be 0b1100

`del default`

## base entry (1)

`3b111 dup dec`

-> stack should be 3b111, 13

`del default`

## base entry (2)

`3b114`

-> stack should be 3b11, 4

`del default`

## base entry (3)

`1b0`

-> stack should be 1, 'b0'

`del default`

## base entry (4)

`62b20 dup dec`

-> stack should be 62b20, 124

`del default`

## base entry (5)

`63b20`

-> stack should be 63, 'b20'

`del default`

## base entry (6)

`2b11001100 0b11001100 ==`

-> stack should be 2b1

`del default`

## base entry (7)

`10b1234 1234 ==`

-> stack should be 10b1

`del default`

## base entry (8)

`16b1234 0x1234 ==`

-> stack should be 16b1

`del default`

## base display (1)

`2 62 for i 62 i base next`

-> stack should be 2b111110, 3b2022, 4b332, 5b222, 6b142, 7b116, 8b76, 9b68, 10b62, 11b57, 12b52, 13b4a, 14b46, 15b42, 16b3e, 17b3b, 18b38, 19b35, 20b32, 21b2k, 22b2i, 23b2g, 24b2e, 25b2c, 26b2a, 27b28, 28b26, 29b24, 30b22, 31b20, 32b1u, 33b1t, 34b1s, 35b1r, 36b1q, 37b1P, 38b1O, 39b1N, 40b1M, 41b1L, 42b1K, 43b1J, 44b1I, 45b1H, 46b1G, 47b1F, 48b1E, 49b1D, 50b1C, 51b1B, 52b1A, 53b19, 54b18, 55b17, 56b16, 57b15, 58b14, 59b13, 60b12, 61b11, 62b10

`del default`

## base display (2)

`2 62 for i i 62 base next`

-> stack should be 62b2, 62b3, 62b4, 62b5, 62b6, 62b7, 62b8, 62b9, 62bA, 62bB, 62bC, 62bD, 62bE, 62bF, 62bG, 62bH, 62bI, 62bJ, 62bK, 62bL, 62bM, 62bN, 62bO, 62bP, 62bQ, 62bR, 62bS, 62bT, 62bU, 62bV, 62bW, 62bX, 62bY, 62bZ, 62ba, 62bb, 62bc, 62bd, 62be, 62bf, 62bg, 62bh, 62bi, 62bj, 62bk, 62bl, 62bm, 62bn, 62bo, 62bp, 62bq, 62br, 62bs, 62bt, 62bu, 62bv, 62bw, 62bx, 62by, 62bz, 62b10

`del default`

## base display (3)

```
100 dup 3 base ==
13455600 dup 5 base ==
55756 dup 17 base ==
2345321 dup 62 base ==
```

-> stack should be 1, 1, 1, 1

`del default`

## base display (4)

```
100 18 base dup 3 base == dec
13455600 55 base dup 5 base == dec
55756 9 base dup 17 base == dec
2345321 57 base dup 62 base == dec
```

-> stack should be 1, 1, 1, 1

`del default`

## negative base numbers (1)

`1000 hex neg`

-> stack should be -0x3e8

`del default`

## negative base numbers (2)

`1000 7 base neg`

-> stack should be -7b2626

`del default`

## negative base numbers (3)

`1000 bin neg`

-> stack should be -0b1111101000

`del default`

## negative base numbers (4)

`-0b1111101000 3 base`

-> stack should be -3b1101001

`del default`

## inf should not be based-represented

`-1 bin 0 bin / 1 3 base 0 3 base /`

-> stack should be -inf, inf

`del default`

## nan should not be based-represented

`-0 bin 0 bin / 0 3 base 0 3 base /`

-> stack should be nan, nan

`del default`
