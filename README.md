# **rpn** - **R**everse **P**olish **N**otation CLI calculator  [![License: LGPLv3](https://www.gnu.org/graphics/lgplv3-88x31.png)](https://www.gnu.org/licenses/lgpl-3.0.en.html)

### A math functional language using reverse (postfix) polish notation

```rpn
rpn> 1 2 +
3
rpn> 2 sqrt
2> 3
1> 1.4142135623730950488016887242096980786
```

### Manipulating reals, complexes, strings, symbols on a stack

```rpn
rpn> 1 2 + 2
2> 3
1> 2
rpn> r->c sq conj (1,1) /
(-3.5,-8.5)
```

```rpn
rpn> 0x1234 dec
4660
rpn> bin
0b1001000110100
```

```rpn
rpn> 4 fix "sqrt of 2 is about " 2 sqrt ->str +
"sqrt of 2 is about 1.4142"
```

```rpn
rpn> << -> x << x 3 ^ 2 x * - 3 + >> >> 'f' sto
rpn> 2 f
7
rpn> (1,2) f
(-10,-6)
```

### Arbitrary precision

```rpn
rpn> 256 prec
rpn> pi
3.1415926535897932384626433832795028841971693993751058209749445923078164062862
rpn>
```

### Variables, structured programming

```rpn
rpn> 0 1 10000 for i i sq + next
333383335000
```

```rpn
rpn> a 1 > if then a sq 'calc' eval else 'stop' eval end
```

```rpn
rpn> << dup
>  1 > if then
>    dup 1 - fibo swap 2 - fibo +
>  else
>    1 == 1 0 ifte
>  end >>
>'fibo' sto
rpn> 12 fibo
144
```

### Available functions

```rpn
rpn> 
Display all 145 possibilities? (y or n)
help     sqrt     arg      <=       depth    ift      ->       log2
h        sq       c->r     !=       roll     ifte     pi       alog2
?        abs      r->c     ==       rolld    do       sin      exp2
quit     sign     p->r     and      over     until    asin     sinh
q        %        r->p     or       ->str    while    cos      asinh
exit     %CH      std      xor      str->    repeat   acos     cosh
test     mod      fix      not      chr      sto      tan      acosh
version  fact     sci      same     num      rcl      atan     tanh
uname    mant     prec     swap     size     purge    d->r     atanh
history  xpon     round    drop     pos      vars     r->d     time
+        floor    default  drop2    sub      clusr    e        date
-        ceil     type     dropn    if       edit     ln       ticks
*        ip       hex      del      then     sto+     log
/        fp       dec      erase    else     sto-     lnp1
inv      min      bin      rot      end      sto*     exp
chs      max      base     dup      start    sto/     expm
neg      re       >        dup2     for      sneg     log10
^        im       >=       dupn     next     sinv     alog10
pow      conj     <        pick     step     eval     exp10
```

## Download

Available as source code and flatpak under [flathub](https://flathub.org/apps/category/Science) (_coming soon_).

## Manual

A reference manual is provided [here](MANUAL.md)

## Generation

rpn is written in C++ and is dynamically linked to GNU MP and GNU MPFR.
It integrates [linenoise-ng](https://github.com/louisrubet/linenoise-ng.git) and [mpreal](http://www.holoborodko.com/pavel/mpfr/) source code as git submodules.

It can be generated following the steps below:

## Generate and install under Ubuntu 20.04 LTS and superior

```shell
sudo apt install git cmake g++ libmpfr6 libmpfr-dev
git clone https://github.com/louisrubet/rpn/ 
mkdir -p rpn/build && cd rpn/build
cmake ..
make -j
sudo make install
```

## Generate and install under Fedora 35

```shell
sudo dnf install git cmake g++ mpfr mpfr-devel
git clone https://github.com/louisrubet/rpn/ 
mkdir -p rpn/build && cd rpn/build
cmake ..
make -j
sudo make install
```

## Contact, contribution, bug report

Please email me at [louis@rubet.fr](mailto:louis@rubet.fr)

You can also use [github issues](https://github.com/louisrubet/rpn/issues) and [pull requests](https://github.com/louisrubet/rpn/pulls)
