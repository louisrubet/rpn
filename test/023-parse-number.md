# PARSE NUMBER

`default del`

## type

`1.0 type`

-> stack should be "number"

`del`

## numb 1

`3.14 +3.14 -3.14`

-> stack should be 3.14, 3.14, -3.14

`del`

## spaces

` -3.14 -3 .14`

-> stack should be -3.14, -3, 0.14

`del`

## exp entry

`+3.14e2`

-> stack should be 314

`del`

## inf nan

`+inf inf -inf nan`

-> stack should be inf, inf, -inf, nan

`del`

## hex

`0x10 0X10`

-> stack should be 0x10, 0x10

`del`

## hex err

`0x 0X`

-> stack should be '0x', '0X'

`del`

## bin

`0b1101 0b0`

-> stack should be 0b1101, 0b0

`del`

## bin err

`0b`

-> stack should be '0b'

`del`

## base

`del 3b12`

-> stack should be 3b12

## base (2)
`del 0x1e2`

-> stack should be 0x1e2

## base (3)

`del 0x-1e2 5b-1234 0b-1`

-> stack should be -0x1e2, -5b1234, -0b1

## base err

`del 0b12`

-> stack should be '0b12'

## base err (2)

`del 1b0`
-> stack should be '1b0'

`del -1b33`

-> stack should be '-1b33'

`del -63b1`

-> stack should be '-63b1'

## wrong base errors

`del 0b0.1100`

-> stack should be 0b1

## particular writings

`del 10b12345 2b1100`

-> stack should be 12345, 0b1100

`del default`
