# **rpn** - **R**everse **P**olish **N**otation language[![License: LGPLv3](https://www.gnu.org/graphics/lgplv3-88x31.png)](https://www.gnu.org/licenses/lgpl-3.0.en.html)

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

The delivery directory is [there](https://github.com/louisrubet/rpn/tree/master/liv)

[download .deb](https://github.com/louisrubet/rpn/blob/master/liv/rpn-2.1-amd64.deb?raw=true)

[download .rpm](https://github.com/louisrubet/rpn/blob/master/liv/rpn-2.1-amd64.rpm?raw=true)

[download .tgz](https://github.com/louisrubet/rpn/blob/master/liv/rpn-2.1-amd64.tar.gz?raw=true)

## Generation

Generation instructions can be found [here](GENERATION.md)

## Contact

Please email me at [louis@rubet.fr](mailto:louis@rubet.fr)

You can use [github issues](https://github.com/louisrubet/rpn/issues) too
