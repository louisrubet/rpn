# PARSE COMPLEX

`default del`

## type

`(1,2) type`

-> stack should be "complex"

`del`

## cplx

`(1,2) (1, 2) ( 1 , 2 )`

-> stack should be (1,2), (1,2), (1,2)

`del`

## cplx Inf

```
(Inf,3) (Inf,-Inf)
```

-> stack should be (Inf,3), (Inf,-Inf)

`del`

## unterminated

`( 3.14e2 , -2`

-> stack should be (314,-2)

`del`

## unterminated err

```
(
(a
(123
(,
(,)
(12,
(,13)
(,3.14
```

-> stack should be '(', '(a', '(123', '(,', '(,)', '(12,', '(,13)', '(,3.14'

`del`
