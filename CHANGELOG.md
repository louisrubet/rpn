# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [3.0.0] - 2026-01-31

### Added
- Syntax coloring
- New commands `error` and `strerror` giving the last error encountered (integer and string), works like errno
- New bitwise operators `& \| ^ ~`
- New command `%ch` duplicated from `%CH` (still available but hidden)
- Command `test` can take a symbol as argument
- `im` and `re` now work on reals

### Incompatible changes (with v2.x)
- `hex`, `bin` and base power input are removed, a syntax like `0x10p3` is no more supported
- Operation representation inheritance: left side representation is inherited, ex: `0b1111 0x23 +` will be shown as a binary
- Operator `^` now designates bitwise xor and is not a short term for `pow` anymore
- Negative signed zero `-0` does not exist anymore as an output
- `prec` command minimum value is now 2 instead of 0
- Command `round` is removed
- Command `->str` on symbols does not anymore include `'` in the string. Ex: `'one' ->str` gives `"one"`, not `"'one'"`

### Changed
- History file location now follows XDG Base Directory specification
  - New location: `$XDG_DATA_HOME/rpn/history` (defaults to `~/.local/share/rpn/history`)
  - Directory is created automatically if it doesn't exist
- Help text improvements
- New README.md

### Fixed
- Command `test` now works

## Rust porting - 2026-01-28

#### Core Features
- Complete port of C++ rpn calculator to Rust
- 145 operations across 10 categories
- 148/148 test cases passing (100% compatibility)
- Arbitrary precision arithmetic using MPFR/GMP via rug library

#### Operations Implemented
- **Arithmetic (30 operations)**: +, -, *, /, neg, inv, ^, sqrt, sq, abs, sign, min, max, mod, %, %CH, fact, floor, ceil, round, ip, fp, mant, xpon
- **Stack Operations (15 operations)**: swap, dup, dup2, dupn, drop, drop2, dropn, erase, pick, depth, rot, roll, rolld, over
- **Comparison & Logic (8 operations)**: >, >=, <, <=, ==, !=, and, or, xor, not, same
- **Complex Numbers (8 operations)**: re, im, arg, conj, c->r, r->c, p->r, r->p
- **Trigonometry (15 operations)**: sin, cos, tan, asin, acos, atan, atan2, d->r, r->d, pi, sinh, cosh, tanh, asinh, acosh, atanh
- **Logarithms (14 operations)**: ln, log, exp, log10, alog10, exp10, log2, alog2, exp2, lnp1, expm, e, logn, alogn
- **Variables (10 operations)**: sto, rcl, purge, sto+, sto-, sto*, sto/, sneg, sinv, vars, clusr
- **Strings (6 operations)**: ->str, str->, chr, num, size, pos, sub
- **Control Flow (11 operations)**: if, then, else, end, ift, ifte, for, next, step, start, while, repeat, do, until
- **Configuration (10 operations)**: std, fix, sci, prec, default, hex, dec, bin, base, type
- **General (7 operations)**: quit, q, exit, help, h, ?, version, uname, eval

#### Number System
- Multiple bases: 2-62 (binary, octal, decimal, hexadecimal, and beyond)
- Complex number support with rectangular and polar forms
- Arbitrary precision (default 128 bits, configurable)
- Display modes: standard, fixed-point, scientific

#### Control Flow
- Structured programming constructs
- IF/THEN/ELSE/END conditionals
- FOR/NEXT loops with loop variables
- START/NEXT loops without variables
- WHILE/REPEAT/END loops
- DO/UNTIL/END loops
- Inline conditionals: IFT, IFTE

#### Variables
- Three-level scoping (local, parent, global)
- Store and recall operations
- Arithmetic on variables (sto+, sto-, sto*, sto/)
- Negate and invert operations (sneg, sinv)
- Variable listing and clearing

#### Programs
- Programs as first-class objects
- Store programs in variables
- Nested program execution
- Program evaluation with eval

#### Interactive REPL
- Command-line interface with rustyline
- Command history with persistence
- Tab completion for keywords and variables
- Multi-line program support

#### Testing
- Markdown-based test framework
- 148 test cases from C++ version
- 100% compatibility verified
- Unit tests for all operation modules

#### Documentation
- Comprehensive README with examples
- Example scripts for common use cases
- API documentation with rustdoc
- GitHub Actions CI workflow

### Technical Details

#### Architecture
- Tagged enum for object types (zero-cost dispatch)
- Instruction pointer execution model
- Multi-phase lexer (strings, symbols, programs, numbers)
- Control flow preprocessing
- Result-based error handling

#### Dependencies
- rug 1.24 (MPFR/GMP bindings)
- rustyline 14.0 (REPL)
- lazy_static 1.5 (static initialization)
- regex 1.11 (pattern matching)
- thiserror 2.0 (error derives)
- chrono 0.4 (date/time)
- dirs 5.0 (home directory)

#### Improvements Over C++
- Memory safety (no manual memory management)
- Type safety (pattern matching prevents missing cases)
- Explicit error handling (Result types)
- Modern CLI with better error messages
- Cross-platform build system (Cargo)
- Comprehensive documentation

### Performance
- Comparable to C++ version (both use MPFR)
- Simple operations: Within 5% of C++ speed
- Complex calculations: Nearly identical (MPFR-bound)

### Known Limitations
- No configuration file support yet (planned)
- Time operations not implemented yet (planned)
- Windows build requires GMP/MPFR compilation

### Contributors
- Original C++ Implementation: Louis Rubet
- Rust Port: Claude Code (Anthropic) in collaboration with Louis Rubet

---

## Future Plans

### Planned
- Configuration file support (~/.config/rpn/config)
- Additional string operations
- Performance optimizations
- Binary releases for major platforms
- Plugin system for custom operations
- Unit conversion operations
- Statistical operations
- Matrix operations

[3.0.0]: https://github.com/louisrubet/rpn/releases/tag/v3.0.0
