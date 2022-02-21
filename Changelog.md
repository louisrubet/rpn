Changelog

- Better parser (now called lexer)
- Use of mpreal instead of raw mpfr for calc on reals
- Use of C++ complex class
- Removing old memory management, efficient but poorly maintainable
- Enhanced code quality and memory usage checks
- Added CircleCI checks: passing functional tests and valgrind mem checks at each pull request
- SonarCloud integration, Sonar way profile
- clang-format now based on google style
- [google c++ style guide](https://google.github.io/styleguide/cppguide.html) applied
- Test files are now markdown (.md) files, tests result are slightly changed
- Delivery as flatpak and snap
- it seems cosh was giving sinh  (!)

grosse perte en performances (!)
- v2.3.2 fibo: 0,01s user 0,01s system 97% cpu 0,017 total
- v3.0.0 fibo: 2,60s user 0,00s system 99% cpu 2,611 total
- facteur 150 environ
cf https://gmplib.org/manual/Custom-Allocation
cf https://www.geeksforgeeks.org/overloading-new-delete-operator-c/

New
- `«` and `»` are now valid as program delimiters. `<<` and `>>` are still valid
- entering the sign after the base (ex: 0x-1e2) is allowed
- rpn is delivered as flatpak and snap packages to be compatible with a maximum of Linux distribs. rpm and deb are no longer generated

Compatibility is broken on these points
- `<< <<` input doesn't lead to `««»»` but to `«<< »`, preventing to eval the real program content
- `1 2+` not allowed anymore, keep `1 2 +`, this corrects bad behaviors like `3b114` pushing `3b11` and `4`
- complexes are written in the form `(1,2)` instead of `(1, 2)` (space is removed)
- removed useless `unti`, `repea`, `whil` (prev.existing for HP28S compatibility)
- removed `sqr` function, please use `sq` instead (prev.existing for HP28S compatibility)
- the binary prefix is always 0b on display, but still can be 0b, 0B, 2b or 2B at input
- the hex prefix is always 0x on display, but still can be 0x, 0X or 16B at input
- `mant` and `xpon` now give binary (and not decimal) significand and exponent, as it is the norm in standard libs (libC, standard C++, mpfr, gmp)
- `dupn`, `roll`, `rolld` are not leaving anymore their argument in front of the stack in case of error
- `sto+` `sto-` `sto*` `sto/` don't accept anymore the syntax `'varname' value stoX`, but only `value 'varname' stoX`, ex: `3 'a' sto*`
- incomplete entry `(1,` is not available anymore
- signed zero is the sign of zero is subject to change  compared to previous version, for example `-3 sqrt` now equals `(0.000000,1.732051)` instead of `(-0.000000,1.732051)`

Debug 
- `sub` now only accepts boundaries between 1 and the string length
- `sto/` behavior: sto/ now correctly stores variable / constant and not constant / variable
- `cosh` now returns the hyp cosinus instead of the hyp sinus (!)
