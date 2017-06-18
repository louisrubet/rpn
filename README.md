# **rpn** - **R**everse **P**olish **N**otation language  [![License: LGPLv3](https://www.gnu.org/graphics/lgplv3-88x31.png)](https://www.gnu.org/licenses/lgpl-3.0.en.html)

### a lisp-based math language using polish notation

```
rpn> 1 2 + 2 sqrt
2> 3
1> 1.4142135623730950488
```

### with arbitrary precision
```
rpn> 256 prec 200 std
rpn> pi 3 * 4 / cos
-0.707106781186547524400844362104849039284835937688474036588339868995366239(...)
```

### providing variables, programs
```
rpn> << rot * swap 2 / chs dup sq rot - sqrt >> 'quad' sto
rpn> << -> x y << x y + ln >> >> 'P' sto
```

### and a bunch of functions
```
rpn> 
Display all 140 possibilities? (y or n)
nop      test     sci      inv      dec      mod      fp       c->r     <=       same     dup      over     sub      next     while    vars     sneg     cos      ln       exp2
?        version  +        ^        hex      fact     min      r->c     !=       swap     dup2     ->str    if       step     whil     clusr    sinv     acos     exp      sinh
h        uname    -        pow      prec     mant     max      p->r     ==       drop     dupn     str->    then     ift      repeat   edit     eval     tan      log10    asinh
help     type     chs      sqrt     round    xpon     re       r->p     and      drop2    pick     chr      else     ifte     repea    sto+     ->       atan     alog10   cosh
q        default  neg      sq       sign     floor    im       >        or       dropn    depth    num      end      do       sto      sto-     pi       d->r     exp10    acosh
quit     std      *        sqr      %        ceil     conj     >=       xor      erase    roll     size     start    until    rcl      sto*     sin      r->d     log2     tanh
exit     fix      /        abs      %CH      ip       arg      <        not      rot      rolld    pos      for      unti     purge    sto/     asin     e        alog2    atanh
```

## Download

deb, rpm and tgz files can be found [there](https://github.com/louisrubet/rpn/releases)

## Manual

A reference manual is provided [here](MANUAL.md)

## Generation

Generation instructions can be found [here](GENERATION.md)

## Contact

Please email me at [louis@rubet.fr](mailto:louis@rubet.fr)

You can use [github issues](https://github.com/louisrubet/rpn/issues) too
