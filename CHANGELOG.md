# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.4.0] - 2022-02-28

### Added

- Better parser (in fact just a lexer, because that's all it takes).
- Use of [mpreal](http://www.holoborodko.com/pavel/mpfr/) instead of raw mpfr for calc on reals.
- Use of the standard C++ complex class.
- Enhanced code quality and memory usage checks.
- Added github actions to pass functional tests and valgrind mem checks at each pull request.
- Applied [google c++ style guide](https://google.github.io/styleguide/cppguide.html)
  - points covered are very numerous: c++17, define guard, name and order of inclusions, static and global variables, common patterns, classes, naming conventions etc.
  - cpplint used with a CPPLINT.cfg removing some warnings.
  - clang-format is now based on google style.
- Test files are now markdown (.md) files, tests result are slightly changed.
- Delived as flatpak, not as rpm and deb anymore.
- Automatic version string from git.
- hex and bin numbers accept floating point input and C99 standard output.
- `«` and `»` are now valid as program delimiters. `<<` and `>>` are still valid.
- Entering the sign after the base (ex: 0x-1e2) is allowed.
- rpn is delivered as flatpak and snap packages to be compatible with a maximum of Linux distribs. rpm and deb are no longer generated.

### Changed

Be carefull, compatibility is broken on these points compared to last version 2.3.2:

- `<< <<` input doesn't lead to `««»»` but to `«<< »`, preventing to eval the real program content.
- Making an operation stick to the previous entry is not possible anymore (ex: `1 2+` instead of `1 2 +`), this corrects bad behaviors like `3b114` pushing `3b11` and `4`.
- Complexes are written in the form `(1,2)` instead of `(1, 2)` (space is removed).
- The binary prefix is always `0b` on display, but the input can be `0b`, `0B`, `2b` or `2B`.
- The hex prefix is always `0x` on display, but the input still can be `0x`, `0X` or `16B` at input.
- `mant` and `xpon` now give binary (and not decimal) significand and exponent, as it is the norm in the standard libs (libC, standard C++, mpfr, gmp).
- `dupn`, `roll`, `rolld` are not leaving anymore their argument in front of the stack in case of error.
- `sto+` `sto-` `sto*` `sto/` don't accept anymore the syntax `'varname' value stoX`, but only `value 'varname' stoX`, ex: `3 'a' sto*`.
- Incomplete entry `(1,` is not available anymore.

### Removed

- Removed useless `unti`, `repea`, `whil` (prev. existing for HP28S compatibility).
- Removed `sqr` function, please use `sq` instead (prev. existing for HP28S compatibility).
- Removed CtrlC for interrupting a program (considered useless).
- Removed old memory management, efficient but poorly maintainable.
- Removed man page generation.

### Fixed

- `sub` now only accepts boundaries between 1 and the string length.
- `sto/` behavior: sto/ now correctly stores variable divided by constant and not constant divided by variable.
- `cosh` now returns the hyp cosinus instead of the hyp sinus.
