# **rpn** - **R**everse **P**olish **N**otation language  [![License: LGPLv3](https://www.gnu.org/graphics/lgplv3-88x31.png)](https://www.gnu.org/licenses/lgpl-3.0.en.html)

### a math functional language using reverse polish notation

```
rpn> 1 2 + 2 sqrt
2> 3
1> 1.4142135623730950488
```

### arbitrary precision provided by GNU MPFR
```
rpn> 256 prec
rpn> pi
3.1415926535897932384626433832795028841971693993751058209749445923078164062862
rpn>
```

### variables, programs
```
rpn> << rot * swap 2 / chs dup sq rot - sqrt >> 'quad' sto
rpn> << -> x y << x y + ln >> >> 'P' sto
```

### and a bunch of functions
```
rpn>
Display all 146 possibilities? (y or n)
nop      exit     chs      sqrt     base     xpon     re       r->p     type     and      drop2    dupn     str->    then     ift      repeat   edit     eval     tan      exp      sinh
help     test     neg      sq       sign     floor    im       std      >        or       dropn    pick     chr      else     ifte     repea    sto+     ->       atan     log10    asinh
h        version  *        sqr      %        ceil     conj     fix      >=       xor      del      depth    num      end      do       sto      sto-     pi       d->r     alog10   cosh
?        uname    /        abs      %CH      ip       arg      sci      <        not      erase    roll     size     start    until    rcl      sto*     sin      r->d     exp10    acosh
q        history  inv      dec      mod      fp       c->r     prec     <=       same     rot      rolld    pos      for      unti     purge    sto/     asin     e        log2     tanh
quit     +        ^        hex      fact     min      r->c     round    !=       swap     dup      over     sub      next     while    vars     sneg     cos      ln       alog2    atanh
q        -        pow      bin      mant     max      p->r     default  ==       drop     dup2     ->str    if       step     whil     clusr    sinv     acos     log      exp2
```

## Download

deb, rpm and tgz files can be found [there](http://nvie.com/img/git-model@2x.png)

## Manual

A reference manual is provided [here](MANUAL.md)

## Methods

Development methods are set at [this page](METHODS.md)

## Generation

Generation instructions can be found [here](GENERATION.md)

## Contact, contribution, bug report

Please email me at [louis@rubet.fr](mailto:louis@rubet.fr)

You can also use [github issues](https://github.com/louisrubet/rpn/issues) and [pull requests](https://github.com/louisrubet/rpn/pulls)
