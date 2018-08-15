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
Display all 150 possibilities? (y or n)
nop      pow      fp       >=       dupn     next     sto*     exp
help     sqrt     min      <        pick     step     sto/     expm
h        sq       max      <=       depth    ift      sneg     log10
?        sqr      re       !=       roll     ifte     sinv     alog10
quit     abs      im       ==       rolld    do       eval     exp10
q        dec      conj     and      over     until    ->       log2
exit     hex      arg      or       ->str    unti     pi       alog2
test     bin      c->r     xor      str->    while    sin      exp2
version  base     r->c     not      chr      whil     asin     sinh
uname    sign     p->r     same     num      repeat   cos      asinh
history  %        r->p     swap     size     repea    acos     cosh
+        %CH      std      drop     pos      sto      tan      acosh
-        mod      fix      drop2    sub      rcl      atan     tanh
chs      fact     sci      dropn    if       purge    d->r     atanh
neg      mant     prec     del      then     vars     r->d     time
*        xpon     round    erase    else     clusr    e        date
/        floor    default  rot      end      edit     ln       ticks
inv      ceil     type     dup      start    sto+     log
^        ip       >        dup2     for      sto-     lnp1
```

## Download

deb, rpm and tgz files can be found [there](liv/)

## Manual

A reference manual is provided [here](MANUAL.md)

## Methods

Development methods are set at [this page](METHODS.md)

## Generation

Generation instructions can be found [here](GENERATION.md)

## Contact, contribution, bug report

Please email me at [louis@rubet.fr](mailto:louis@rubet.fr)

You can also use [github issues](https://github.com/louisrubet/rpn/issues) and [pull requests](https://github.com/louisrubet/rpn/pulls)
