# REAL

`default del`

## real decimal

`1`

-> stack size should be 1

-> stack should be 1

`del`

## real decimal (2)

`2.345`

-> stack should be 2.345

`del`

## real decimal (3)

`1 2.345 3 4.9`

-> stack size should be 4

-> stack should be 1, 2.345, 3, 4.9

`del`

## real hex

`0x1234 0x10.10`

-> stack should be 0x1234, 0x10

`del`

## real hex (2)

```
0x1.234p+12 0x1.01p+4
dec swap dec swap
```

-> stack should be 4660, 16.0625

`del`

## real binary

`0b11001100`

-> stack should be 0b11001100

`del`

## real inf, nan

```
inf
@inf@
+inf
+@inf@
-inf
-@inf@
nan
@nan@
```

-> stack should be inf, inf, inf, inf, -inf, -inf, nan, nan

`del`

## prec (1)

```
default
56 prec
pi
```

-> stack should be 3.141592653589793

`del`

## prec (2)

```
default
100 prec
pi
```

-> stack should be 3.14159265358979323846264338328

`del`

## prec error (1)

`0 prec`

-> error should be 4

`del`

## prec error (2)

`0x8000000000000000 prec`

-> error should be 4

`del default`

## round (1)

`"nearest" round`

-> error should be 0

`del`

## round (2)

`"toward zero" round`

-> error should be 0

`del`

## round (3)

`"toward +inf" round`

-> error should be 0

`del`

## round (4)

`"toward -inf" round`

-> error should be 0

`del`

## round (5)

`"away from zero" round`

-> error should be 0

`del`

## round (6)

`round`

-> error should be 2

`del default`

## add (1)

`1.2 2.3 +`

-> stack should be 3.5

`del`

## add (2)

`1.2 2.3+`

-> stack should be 3.5

`del`

## add (3)

`2.3 +`

-> error should be 2

-> stack size should be 1

`del`

## add (4)

`+`

-> error should be 2

`del`

## sub (1)

`1.2 2.3 -`

-> stack should be -1.1

`del`

## sub (2)

`1.2 2.3-`

-> stack should be -1.1

`del`

## sub (3)

`2.3 -`

-> error should be 2

-> stack size should be 1

`del`

## sub (4)

`-`

-> error should be 2

`del`

## mul (1)

`1.2 2.3 *`

-> stack should be 2.76

`del`

## mul (2)

`1.2 2.3*`

-> stack should be 2.76

`del`

## mul (3)

`2.3 *`

-> error should be 2

-> stack size should be 1

`del`

## mul (4)

`*`

-> error should be 2

`del`

## div (1)

`1.2 2.3 /`

-> stack should be 0.52173913043478260869565217391304347826

`del`

## div (2)

`1.2 2.3/`

-> stack should be 0.52173913043478260869565217391304347826

`del`

## div (3)

`2.3 /`

-> error should be 2

-> stack size should be 1

`del`

## div (4)

`/`

-> error should be 2

`del`

## chs (1)

`3.14 chs`

-> stack should be -3.14

`del`

## chs (2)

`chs`

-> error should be 2

## neg (1)

`3.14 neg`

-> stack should be -3.14

`del`

## neg (2)

`neg`

-> error should be 2

## inv (1)

`2 inv`

-> stack should be 0.5

`del`

## inv (2)

`inv`

-> error should be 2

## % (1)

`2 30 %`

-> stack should be 0.6

`del`

## % (2)

`2 30%`

-> stack should be 0.6

del

## % (3)

`2 %`

-> error should be 2

-> stack size should be 1

`del`

## % (4)

`%`

-> error should be 2

`del`

## %CH (1)

`2 0.6 %CH`

-> stack should be 30

`del`

## %CH (2)

`2 0.6%CH`

-> stack should be 30

`del`

## %CH (3)

`2 %CH`

-> error should be 2

-> stack size should be 1

`del`

## %CH (4)

`%CH`

-> error should be 2

`del`

## ^ (1)

`2 10 ^`

-> stack should be 1024

`del`

## ^ (2)

`2 10^`

-> stack should be 1024

`del`

## ^ (3)

`2 ^`

-> error should be 2

-> stack size should be 1

`del`

## ^ (4)

`^`

-> error should be 2

`del`

## sqrt (1)

`9 sqrt`

-> stack should be 3

`del`

## sqrt (2)

`sqrt`

-> error should be 3

## sq (1)

`12 sq`

-> stack should be 144

`del`

## sq (2)

`sq`

-> error should be 2

## sqr (1)

`12 sqr`

-> stack should be 144

`del`

## sqr (2)

`sqr`

-> error should be 2

## mod (1)

`9 4 mod`

-> stack should be 1

`del`

## mod (2)

`9 mod`

-> error should be 2

-> stack size should be 1

`del`

## mod (3)

`mod`

-> error should be 2

`del`

## abs (1)

`-9 abs`

-> stack should be 9

`del`

## abs (2)

`9 abs`

-> stack should be 9

`del`

## abs (3)

`abs`

-> error should be 2

`del`

## fact (1)

`6 fact`

-> stack should be 720

`del`

## fact (2)

`'r' fact`

-> error should be 3

-> stack size should be 1

`del`

## fact (3)

`fact`

-> error should be 2

`del`

## sign (1)

`23 sign -34 sign 0 sign`

-> stack should be 1, -1, 0

`del`

## sign (2)

`sign`

-> error should be 2

`del`

## mant (1)

`123.456 mant -123.456 mant 0 mant`

-> stack should be 0.123456, 0.123456, 0

`del`

## mant (2)

`inf mant`

-> error should be 4

`-inf mant`

-> error should be 4

`nan mant`

-> error should be 4

`del`

## xpon (1)

`123.456 xpon -123.456 xpon 0 mant`

-> stack should be 3, 3, 0

`del`

## xpon (2)

`inf xpon`

-> error should be 4

`-inf xpon`

-> error should be 4

`nan xpon`

-> error should be 4

`del`

## min (1)

`1 2 min 4 3 min`

-> stack should be 1, 3

`del`

## min (2)

`'a' 'g' min`

-> error should be 3

`del`

## min (3)

`1 min`

-> error should be 2

`del`

## min (4)

`min`

-> error should be 2

`del`

## max (1)

`1 2 max 4 3 max`

-> stack should be 2, 4

`del`

## max (2)

`'a' 'g' max`

-> error should be 3

`del`

## max (3)

`1 max`

-> error should be 2

`del`

## max (4)

`max`

-> error should be 2

`del`

## ip (1)

`1.22 ip`

-> stack should be 1

`del`

## ip (2)

`-1.22 ip`

-> stack should be -1

`del`

## fp (1)

`1.22 fp`

-> stack should be 0.22

`del`

## fp (2)

`-1.22 fp`

-> stack should be -0.22

`del`

## floor (1)

`1.22 floor`

-> stack should be 1

`del`

## floor (2)

`-1.22 floor`

-> stack should be -2

`del`

## ceil (1)

`1.22 ceil`

-> stack should be 2

`del`

## ceil (2)

`-1.22 ceil`

-> stack should be -1

`del default`
