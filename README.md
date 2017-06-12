# **rpn** - **R**everse **P**olish **N**otation language

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
-0.70710678118654752440084436210484903928483593768847403658833986899536623923105962592591087473479525356117497671223960240783675485777817360566073272153486395308799122357513534343724299243077135552002446
```

### providing variables, programs
```
rpn> << rot * swap 2 / chs dup sq rot - sqrt >> 'quad' sto
rpn> << -> x y << x y + ln >> >> 'P' sto
```

### and a bunch of functions
```
rpn> 
Display all 128 possibilities? (y or n)
!=       ->       >        acosh    atan     cosh     drop     e        exit     for      ifte     max      not      pos      r->d     rot      sinh     sqrt     sto+     tan      unti     xor      
%        ->str    >=       alog     atanh    d->r     drop2    edit     exp      h        inv      min      num      pow      rcl      round    sinv     start    sto-     tanh     until    xpon     
%CH      /        ?        alog2    chr      dec      dropn    else     exp10    help     ln       mod      or       prec     repea    same     size     std      sto/     test     vars     
*        <        ^        and      chs      default  dup      end      exp2     hex      log      neg      over     purge    repeat   sci      sneg     step     str->    then     version  
+        <=       abs      asin     clusr    depth    dup2     erase    fact     if       log2     next     pi       q        roll     sign     sq       sto      sub      type     whil     
-        ==       acos     asinh    cos      do       dupn     eval     fix      ift      mant     nop      pick     quit     rolld    sin      sqr      sto*     swap     uname    while    
rpn> 
```

## Manual

A reference manual is provided [here](MANUAL.md)

## Installation

The delivery directory is [there]()

[download .deb](https://github.com/louisrubet/rpn/blob/master/liv/rpn-2.1-amd64.deb?raw=true)

[download .rpm](https://github.com/louisrubet/rpn/blob/master/liv/rpn-2.1-amd64.rpm?raw=true)

[download .tgz](https://github.com/louisrubet/rpn/blob/master/liv/rpn-2.1-amd64.tar.gz?raw=true)

## Generation

Generation instructions can be found [here](GENERATION.md)

## Contact

Please email me at [louis@rubet.fr](mailto:louis@rubet.fr)

You can use [github issues](https://github.com/louisrubet/rpn/issues) too
