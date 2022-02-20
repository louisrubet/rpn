# TRIGONOMETRY

`del default`

## pi

`6 fix pi`

-> stack should be 3.141593

`del default`

## real sin asin

`0 sin pi 2 / sin pi 6 / sin`
`0 asin 0 == 1 asin pi 2 / == 0.5 asin pi 6 / ==`

-> stack should be 0, 1, 0.5, 1, 1, 1

`del`

## real cos acos

`0 cos pi 3 / cos`
`1 acos 0 == 0.5 acos pi 3 / ==`

-> stack should be 1, 0.5, 1, 1

`del`

## real tan atan

`pi 4 / tan 1 == 1 atan pi 4 / ==`

-> stack should be 1, 1

`del`
