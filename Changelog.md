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

grosse perte en performances (!)

New
- `«` and `»` are now valid as program delimiters. `<<` and `>>` are still valid
- added `norm` as a synonym of `abs`
- `test` command can also take a symbol as a parameter ; if the symbol contains a valid
- entering the sign after the base (ex: 0x-1e2) is allowed

Compatibility is broken on these points
- `<< <<` input doesn't lead to `««»»` but to `«<< »`, preventing to eval the real program content
- `1 2+` not allowed anymore, keep `1 2 +`, this corrects bad behaviors like `3b114` pushing `3b11` and `4`
- complexes are written in the form `(1,2)` instead of `(1, 2)` (space is removed)
- removed useless `unti`, `repea`, `whil` (prev.existing for HP28S compatibility)
- removed `sqr` function, please use `sq` instead (prev.existing for HP28S compatibility)
- the binary prefix is always 0b on display, but still can be 0b, 0B, 2b or 2B at input
- the hex prefix is always 0x on display, but still can be 0x, 0X or 16B at input
- `mant` and `xpon` now give binary significand and exponent, as it is the norm in standard libs (libC, standard C++, mpfr, gmp)
- `dupn`, `roll`, `rolld` are not leaving anymore their argument in front of the stack in case of error
- `sub` now only accepts boundaries between 1 and the string length
- rpn is not delivered as deb and rpm anymore, since it is too much bound to particular OS
- `sto/` bug correction: 3 'a' sto/ now correctly does a/3 -> a (although it did 3/a -> a)
- `sto+` `sto-` `sto*` `sto/` don't accept anymore the syntax `'varname' value stoX`, but only `value 'varname' stoX`, ex: `3 'a' sto*`

missing tests
- nested loops / if / while / do + access to local variables
- les arguments d'une fonction en erreur doivent ils êre consommés ?
    ex embettant : sto+
- sto+ * - / : ajouter des cas de test avec type incorrects
- `10 -> n << n >>` plante
- `1 'i' sto while i <= 2 repeat 0 'j' sto while j <= 1 repeat i (1,0) * j (0,1) * + 1 'j' sto+ end 1 'i' sto+ end` plante