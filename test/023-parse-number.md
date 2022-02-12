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

-> stack should be 16, 16

`del`

## hex err

`0x 0X`

-> stack should be '0x', '0X'

`del`

## bin

`0b1101 0b0`

-> stack should be 13, 0

`del`

## bin err

`0b`

-> stack should be '0b'

`del`
