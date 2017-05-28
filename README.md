# **rpn** - **R**everse **P**olish **N**otation language

**rpn**
- is a **math-oriented language** inspired by Hewlett-Packard **R**everse **P**olish **L**isp (**HP28S** user manual is provided as a reference), it includes at least **stack**, **store**, **branch**, **test**, **trig** and **logs** commands
- is implemented as a a **command-line calculator** for GNU/Linux
- brings powerfull calculation facilities on floating point numbers with __arbitrary precision__, provided by **GNU MPFR** library
- uses that so cool **reverse polish notation**

Quick examples:

- easy calculation with **stacked results**
```
rpn> 1 exp 3 *
8.1548454853771357061
rpn> 2 sqrt
2> 8.1548454853771357061
1> 1.4142135623730950488
rpn> 
```

- **programs** and **variables**, eg same example as in HP28S Quick Reference:
```
rpn> << rot * swap 2 / chs dup sq rot - sqrt >> 'quadratic_solution' sto
rpn> 1 2 -3 quadratic_solution
2> -1
1> 2
rpn> vars
var 1: name 'quadratic_solution', type program, value << rot * swap 2 / chs dup sq rot - sqrt  >>
```

- **local variables**, always from the same reference:
```
rpn> << -> x y << x y + ln >> >> 'P' sto
rpn> 1 2 P
rpn> 1.0986122886681096914
```

- **arbitrary precision** (up to 0x7FFFFFFFFFFFFFFF bits with GNU MPFR !)
```
rpn> 256 prec 200 std
rpn> pi 3 * 4 / cos
-0.70710678118654752440084436210484903928483593768847403658833986899536623923105962592591087473479525356117497671223960240783675485777817360566073272153486395308799122357513534343724299243077135552002446
rpn> 
```

- Following objects are managed: **floating numbers**, **symbols**, **strings**, **programs**, plus language **keywords** (commands and flow controls)
```
4> 'symbol'
3> "string"
2> 12.3456
1> << -> n << 0 1 n for i i 2 * inv + next >> >>
rpn> 
```

- A __GNU-readline__-based interactive editor with autocompletion is provided.

## keywords

#### general

|keyword|description|
|-|-|
|nop	| no operation |
|help |	(or h or ?) this help message
|quit |	(or q or exit) quit software
|version |	show rpn version
|uname |	show rpn complete identification string
|type |	show type of stack first entry
|default |	set float representation and precision to default
|prec |	get float precision in bits when first stack is not a number, set float precision in bits when first stack entry is a number. ex: ```256 prec```
|round|	set float rounding mode. Authoerized values are: ```["nearest", "toward zero", "toward +inf", "toward -inf", "away from zero"] round```. ex: ```"nearest" round```

#### real

|keyword|description|
|-|-|
|+|	addition
|-|	substraction
|neg|	negation
|*|	multiplication
|/|	division
|inv|	inverse
|%|	purcent
|%|CH	inverse purcent
|^|	(or pow) power
|sqrt|	square root
|sq|	(or sqr) square
|mod|	modulo
|abs|	absolute value

#### real representation

|keyword|description|
|-|-|
|dec|	decimal representation
|hex|	hexadecimal representation
|bin|	binary representation
|std|	standard floating numbers representation. ex: [25] std
|fix|	fixed point representation. ex: 6 fix
|sci|	scientific floating point representation. ex: 20 sci

#### test

|keyword|description|
|-|-|
|>|	binary operator >
|>=|	binary operator >=
|<|	binary operator <
|<=|	binary operator <=
|!=|	binary operator != (different)
|==|	binary operator == (equal)
|and|	boolean operator and
|or|	boolean operator or
|xor|	boolean operator xor
|not|	boolean operator not
|same|	boolean operator same (equal)

#### stack

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
|roll| move a stack entry to the top of the stack
|rolld| move the element on top of the stack to a higher stack position
|over| push a copy of the element in stack level 2 onto the stack

#### string

|keyword|description|
|-|-|
|->str|	convert an object into a string
|str->|	convert a string into an object

#### branch

|keyword|description|
|-|-|
|if|	test-instructions
|then|	true-instructions
|else|	false-instructions
|end|	(end of if structure)
|start|	repeat instructions several times
|for|	repeat instructions several times with variable
|next|	ex: ```1 10 start <instructions> next```
|step|	ex: ```1 100 start <instructions> 4 step```

#### store

|keyword|description|
|-|-|
|sto|	store a variable. ex: ```1 'name' sto```
|rcl|	recall a variable. ex: ```'name' rcl```
|purge|	delete a variable. ex: ```'name' purge```
|vars|	list all variables
|edit|	edit a variable content

#### program

|keyword|description|
|-|-|
|eval|	evaluate (run) a program, or recall a variable. ex: ```'my_prog' eval```
|->|	load program local variables. ex: ```<< -> n m << 0 n m for i i + next >> >>```

#### trig

|keyword|description|
|-|-|
|pi|	pi constant
|sin|	sinus
|asin|	arg sinus
|cos|	cosinus
|acos|	arg cosinus
|tan|	tangent
|atan|	arg tangent
|d|->r	convert degrees to radians
|r|->d	convert radians to degrees

#### logs

|keyword|description|
|-|-|
|e|	exp(1) constant
|log|	logarithm base 10
|alog|	(or exp10) exponential base 10
|log2|	logarithm base 2
|alog2|	(or exp2) exponential base 2
|ln|	logarithm base e
|exp|	exponential
|sinh|	hyperbolic sine
|asinh|	inverse hyperbolic sine
|cosh|	hyperbolic cosine
|acosh|	inverse hyperbolic cosine
|tanh|	hyperbolic tangent
|atanh|	inverse hyperbolic tangent

#### default

Default float mode is 'std' with 20 digits

Default floating point precision is 128 bits

Default rounding mode is 'nearest' 

## Tests

Unit tests are given as txt files in the test subdirectory.

Use the command 'test' to run a test file, eg
```
# cd src_directory/
# rpn
rpn> "test/01-all.txt"
rpn> test
## ENTRY TEST
(...)
# real decimal PASSED
# real hex PASSED
# real binary PASSED
(...)
```

Test output is done on stdout and is not stacked in rpn.

## generation and installation

At this time the software is proposed for **GNU/Linux** only and provided as a source code.

No binary package is provided, user must compile its version by following these 3 steps:

#### GNU GMP 6.1.2

- download and install **GNU GMP v6.1.2** at [https://gmplib.org/](https://gmplib.org/)

#### GNU MPFR 3.1.5

- download and install **GNU MPFR v3.1.5** from [http://www.mpfr.org](http://www.mpfr.org)

#### rpn

- clone this project or download a zipped version from [the release directory](https://github.com/louisrubet/rpn/releases)
	```
	# git clone https://github.com/louisrubet/rpn.git
	```
- configure and make
	```
	# cd rpn/
	# ./configure && make
	```
- install
	```
	# sudo make install
	```
