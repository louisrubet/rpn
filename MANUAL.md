# **rpn v2.2**- reference manual

**rpn**
- is a **math-oriented language** inspired by Hewlett-Packard **R**everse **P**olish **L**isp (**HP28S** user manual is provided as a reference), it includes at least **stack**, **store**, **branch**, **test**, **trig** and **logs** commands
- is implemented as a a **command-line calculator** for GNU/Linux
- brings powerfull calculation facilities on floating point numbers with __arbitrary precision__, provided by **GNU MP** and **GNU MPFR** libraries
- uses that so cool **reverse polish notation**

## Quick examples

### easy calculation with **stacked results**
```
rpn> 1 2 +
3
rpn> 2 sqrt
2> 3
1> 1.4142135623730950488
```

### **programs** and **variables**
```
rpn> << rot * swap 2 / neg dup sq rot - sqrt >> 'quadratic_solution' sto
rpn> 1 2 -3 quadratic_solution
2> -1
1> 2
rpn> vars
var 1: name 'quadratic_solution', type program, value << rot * swap 2 / chs dup sq rot - sqrt  >>
```

### **local variables**
```
rpn> << -> x y << x y + ln >> >> 'P' sto
rpn> 1 2 P
rpn> 1.0986122886681096914
```

### **arbitrary precision**
Precision can be really high, up to 0x7FFFFFFFFFFFFFFF bits with GNU MPFR
```
rpn> 256 prec 200 std
rpn> pi 3 * 4 / cos
-0.707106781186547524400844362104849039284835937688474036588339868995366239(...)
rpn> 
```

### Objects

Following objects are managed: **floating numbers**, **complexes**, **symbols**, **strings**, **programs**, plus language **keywords** (commands and flow controls)
```
5> (1,-2.33)
4> 'symbol'
3> "string"
2> 12.3456
1> << -> n << 0 1 n for i i 2 * inv + next >> >>
rpn> 
```

### Command line

**rpn** is a cli interface with an **interactive editor** with autocompletion provided by **linenoise-ng**, see https://github.com/arangodb/linenoise-ng

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

## keywords

### general

|keyword|description|
|-|-|
|nop |		no operation |
|help |		(or h or ?) this help message
|quit |		(or q or exit) quit software
|version |	show rpn version
|uname |	show rpn complete identification string

### usual operations - real and complex

|keyword|description|
|-|-|
|+|		addition
|-|		substraction
|neg|	(or chs) negation
|*|		multiplication
|/|		division
|inv|	inverse
|^|		(or pow) power
|sqrt|	square root
|sq|	(or sqr) square
|abs|	absolute value for a number or sqrt(re*re+im*im) for a complex
|dec|	decimal representation
|hex|	hexadecimal representation
|sign|	sign of a real, unary vector in the same direction for a complex

### operations on reals

|keyword|description|
|-|-|
|%|		purcent
|%CH|	inverse purcent
|mod|	modulo
|fact|	n! for integer n or Gamma(x+1) for fractional x
|mant|	mantissa of a real number
|xpon|	exponant of a real number
|floor| largest number <=
|ceil| smallest number >=
|ip| integer part
|fp| fractional part
|min|	min of 2 real numbers
|max|	max of 2 real numbers

### operations on complexes

|keyword|description|
|-|-|
|re| complex real part
|im| complex imaginary part
|conj| complex conjugate
|arg| complex argument in radians
|r->p| rectangular to polar coordinates
|p->r| polar to rectangular coordinates
|r->c| transform 2 reals in a complex
|c->r| transform a complex in 2 reals

### mode

|keyword|description|
|-|-|
|std|	standard floating numbers representation. ex: [25] std
|fix|	fixed point representation. ex: 6 fix
|sci|	scientific floating point representation. ex: 20 sci
|prec|	get float precision in bits when first stack is not a number, set float precision in bits when first stack entry is a number. ex: ```256 prec```
|round|	set float rounding mode. Authoerized values are: ```["nearest", "toward zero", "toward +inf", "toward -inf", "away from zero"] round```. ex: ```"nearest" round```
|default |	set float representation and precision to default
|type |		show type of stack first entry

### test

|keyword|description|
|-|-|
|>|		binary operator >
|>=|	binary operator >=
|<|		binary operator <
|<=|	binary operator <=
|!=|	binary operator != (different)
|==|	binary operator == (equal)
|and|	boolean operator and
|or|	boolean operator or
|xor|	boolean operator xor
|not|	boolean operator not
|same|	boolean operator same (equal)

### stack

|keyword|description|
|-|-|
|swap|	swap 2 first stack entries
|drop|	drop first stack entry
|drop2|	drop 2 first stack entries
|dropn|	drop n first stack entries
|erase|	drop all stack entries
|rot|	rotate 3 first stack entries
|dup|	duplicate first stack entry
|dup2|	duplicate 2 first stack entries
|dupn|	duplicate n first stack entries
|pick|	push a copy of  the given stack level onto the stack
|depth|	give stack depth
|roll|	move a stack entry to the top of the stack
|rolld|	move the element on top of the stack to a higher stack position
|over|	push a copy of the element in stack level 2 onto the stack

### string

|keyword|description|
|-|-|
|->str|	convert an object into a string
|str->|	convert a string into an object
|chr|	convert ASCII character code in stack level 1 into a string
|num|	return ASCII code of the first character of the string in stack level 1 as a real number
|size|	return the length of the string
|pos|	seach for the string in level 1 within the string in level 2
|sub|	return a substring of the string in level 3

### branch

|keyword|description|
|-|-|
|if| if <test-instruction> then <true-instructions> else <false-instructions> end
|then| used with if
|else| used with if
|end| used with various branch instructions
|ift|	similar to if-then-end, <test-instruction> <true-instruction> ift"
|ifte|	similar to if-then-else-end, <test-instruction> <true-instruction> <false-instruction> ifte"
|start| start> <end> start <instructions> next|<step> step
|for| start> <end> for <variable> <instructions> next|<step> step
|next| used with start and for
|step| used with start and for
|do| do <instructions> until <condition> end
|until | (or unti) used with do
|while| (or whil) while <test-instruction> repeat <loop-instructions> end
|repeat| (or repea) used with while

### store

|keyword|description|
|-|-|
|sto|	store a variable. ex: ```1 'name' sto```
|rcl|	recall a variable. ex: ```'name' rcl```
|purge|	delete a variable. ex: ```'name' purge```
|vars|	list all variables
|clusr|	erase all variables
|edit|	edit a variable content
|sto+|	add to a stored variable. ex: 1 'name' sto+ 'name' 2 sto+
|sto-|	substract to a stored variable. ex: 1 'name' sto- 'name' 2 sto-
|sto*|	multiply a stored variable. ex: 3 'name' sto* 'name' 2 sto*
|sto/|	divide a stored variable. ex: 3 'name' sto/ 'name' 2 sto/
|sneg|	negate a variable. ex: 'name' sneg
|sinv|	inverse a variable. ex: 1 'name' sinv

### program

|keyword|description|
|-|-|
|eval|	evaluate (run) a program, or recall a variable. ex: ```'my_prog' eval```
|->|	load program local variables. ex: ```<< -> n m << 0 n m for i i + next >> >>```

### trig on reals and complexes

|keyword|description|
|-|-|
|pi|	pi constant
|sin|	sinus
|asin|	arg sinus
|cos|	cosinus
|acos|	arg cosinus
|tan|	tangent
|atan|	arg tangent
|d->r|	convert degrees to radians
|r->d|	convert radians to degrees

### logs on reals and complexes

|keyword|description|
|-|-|
|e|	exp(1) constant
|ln| (or log) logarithm base e
|exp|	exponential
|log10|	logarithm base 10
|alog10| (or exp10) exponential base 10
|log2|	logarithm base 2
|alog2|	(or exp2) exponential base 2
|sinh|	hyperbolic sine
|asinh|	inverse hyperbolic sine
|cosh|	hyperbolic cosine
|acosh|	inverse hyperbolic cosine
|tanh|	hyperbolic tangent
|atanh|	inverse hyperbolic tangent

### default

Default float mode is 'std' with 20 digits

Default floating point precision is 128 bits

Default rounding mode is 'nearest' 

## Tests

Unit tests are given as txt files in the [test](https://github.com/louisrubet/rpn/tree/master/test) subdirectory.

Use the command 'test' to run a test files, eg
```
# cd <test_directory>/
# rpn
rpn> "all.txt"
rpn> test
rpn> test

01-mode.txt: MODE
# std (1) PASSED
# std (2) PASSED
# std (3) PASSED
(...)
Total: run 386 tests: 386 passed, 0 failed (495 steps: 495 passed, 0 failed)
```

Test output is done on stdout and is not stacked in rpn.
