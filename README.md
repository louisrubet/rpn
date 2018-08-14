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
Display all 147 possibilities? (y or n)
nop      chs      bin      min      prec     not      depth    else     repeat   sinv     ln       cosh
help     neg      base     max      round    same     roll     end      repea    eval     log      acosh
h        *        sign     re       default  swap     rolld    start    sto      ->       lnp1     tanh
?        /        %        im       type     drop     over     for      rcl      pi       exp      atanh
quit     inv      %CH      conj     >        drop2    ->str    next     purge    sin      expm
q        ^        mod      arg      >=       dropn    str->    step     vars     asin     log10
exit     pow      fact     c->r     <        del      chr      ift      clusr    cos      alog10
test     sqrt     mant     r->c     <=       erase    num      ifte     edit     acos     exp10
version  sq       xpon     p->r     !=       rot      size     do       sto+     tan      log2
uname    sqr      floor    r->p     ==       dup      pos      until    sto-     atan     alog2
history  abs      ceil     std      and      dup2     sub      unti     sto*     d->r     exp2
+        dec      ip       fix      or       dupn     if       while    sto/     r->d     sinh
-        hex      fp       sci      xor      pick     then     whil     sneg     e        asinh
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
