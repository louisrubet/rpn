# rpn [![License: LGPLv3](https://www.gnu.org/graphics/lgplv3-88x31.png)](https://www.gnu.org/licenses/lgpl-3.0.en.html)

Reverse Polish Notation CLI calculator with arbitrary precision

## Install from source

```bash
git clone https://github.com/louisrubet/rpn
cd rpn
cargo build --release
./target/release/rpn
```

## Quick Start

Interactive:

```bash
$ rpn
rpn> 2 3 + dup *
1> 25

rpn> sqrt
1> 5

rpn> pi 2 / sin
1> 1

rpn> 255 hex
1> 0xff

rpn> quit
```

Or from command line:

```bash
rpn "2 10 pow"          # 1024
rpn "0xFF 1 +"          # 0x100
rpn "(1,2) (3,4) +"     # (4,6)
```

## Features

- **Arbitrary precision** via MPFR
- **Number bases** 2-62 (bin, hex, etc.)
- **Complex numbers** with full arithmetic
- **Programs** as first-class objects
- **Control flow**: if/then/else, for/next, while/repeat, do/until
- **Variables** with three-level scoping

Complete manual [here in MANUAL.md](MANUAL.md)

## Operations

| Category   | Commands                                                                                                       |
| ---------- | -------------------------------------------------------------------------------------------------------------- |
| Arithmetic | `+` `-` `*` `/` `^` `neg` `inv` `sqrt` `sq` `abs` `mod` `%` `fact` `floor` `ceil` `ip` `fp` `min` `max` `sign` |
| Bitwise    | `&` `\|` `^` `~`                                                                                               |
| Stack      | `swap` `dup` `drop` `rot` `roll` `pick` `depth` `over` `dup2` `dupn` `drop2` `dropn` `erase`                   |
| Compare    | `>` `>=` `<` `<=` `==` `!=` `and` `or` `xor` `not`                                                             |
| Trig       | `sin` `cos` `tan` `asin` `acos` `atan` `atan2` `d->r` `r->d` `pi`                                              |
| Hyperbolic | `sinh` `cosh` `tanh` `asinh` `acosh` `atanh`                                                                   |
| Logs       | `ln` `exp` `log10` `alog10` `log2` `alog2` `logn` `alogn` `e`                                                  |
| Complex    | `re` `im` `arg` `conj` `c->r` `r->c` `p->r` `r->p`                                                             |
| String     | `->str` `str->` `chr` `num` `size` `pos` `sub`                                                                 |
| Variables  | `sto` `rcl` `purge` `sto+` `sto-` `sto*` `sto/` `sneg` `sinv` `vars` `clusr`                                   |
| Control    | `if` `then` `else` `end` `ift` `ifte` `for` `next` `step` `while` `repeat` `do` `until` `start`                |
| Display    | `std` `fix` `sci` `prec` `hex` `dec` `bin` `base`                                                              |
| Misc       | `eval` `type` `default` `help` `quit` `version` `uname`                                                        |

## Examples

### Variables & Programs

```
rpn> 42 'answer' sto
rpn> answer 1 +
1> 43

rpn> << dup * >> 'sq' sto
rpn> 7 sq
1> 49
```

### Number Bases

```
rpn> 0b1010 0b0110 +
1> 0b10000

rpn> 16bff dec
1> 255

rpn> 1000 36 base
1> 36brs
```

### Complex Numbers

```
rpn> (3,4) abs
1> 5

rpn> (1,0) (0,1) *
1> (0,1)
```

## License

LGPL-3.0 — See LICENSE file.
