## tickets

Categories:
+ evolution
d debug
n non-compatibility

- hex, bin and base representations must never show fractional parts
- user cannot enter fractional parts to hex, bin and base
  - eg 0x12.34, 0b110.11, 5b0.11 are not correct real numbers entries, these should be treated like symbols
- commands std, sci, fix, have no action on hex, bin and base representations
- a decimal number with fractional part converted to hex, bin or base must not show its fractional part
  - ex:
    12.34 hex -> display should be 0xc
    dec -> display should be 12.34

| Category | Done | Evolution                                                                                                                    |
| -------- | ---- | ---------------------------------------------------------------------------------------------------------------------------- |
| +        | x    | now Rust-zested                                                                                                              |
| +        | x    | syntax coloring                                                                                                              |
| +        | x    | new commands `error` and `strerror` giving the last error encountered (integer and string), works like errno                 |
| +        | x    | new bitwise operators `& \| ^ ~`                                                                                             |
| +        | x    | new command `%ch` duplicated from `%CH` (still available but hidden)                                                         |
| +        | x    | history now stored in more standard "$XDG_DATA_HOME/rpn/history" instead of ~/.rpn_history                                   |
| +        | x    | Command `test` can take a symbol as argument                                                                                 |
| n        | x    | operation representation inheritance                                                                                         |
| n        | x    | `^` now designates bitwise xor and is not a short term for `pow` anymore                                                     |
| n        | x    | `-0` as output does not exist anymore, see ancient test                                                                      |
| n        | x    | `prec` command minimum value is now 2 instead of 0                                                                           |
| n        | x    | command `round` is removed                                                                                                   |
| n        | x    | `hex` power input are removed, like former 0x10p3                                                                            |
| n        | x    | command `->str` on variable names now parses out character '                                                                 |
| d        | x    | `test` command debugged                                                                                                      |
| d        | x    | `im` and `re` now work on reals                                                                                              |
| +        |      | new command `run`                                                                                                            |
| +        |      | Add help on commands, ex: `help sto+`                                                                                        |
| +        |      | new command `store` for storing the variables                                                                                |
| +        |      | arithmetic and bitwise operators `+ - * / % & ^ ~` can stick to the right of inputted numbers like `1 2+` instead of `1 2 +` |

## tests

* [x] "005-test-framework.md"
* [x] "010-mode.md"
* [x] "020-general.md"
* [x] "021-parse-string.md"
* [x] "022-parse-symbol.md"
* [x] "023-parse-number.md"
* [x] "024-parse-complex.md"
* [x] "025-parse-other.md"
* [x] "026-parse-program.md"
* [x] "027-parse-hex-bin-base.md"
* [x] "030-branch.md"
* [x] "040-stack.md"
* [x] "050-real.md"
* [x] "051-real-representations.md"
* [x] "060-string.md"
* [x] "070-test.md"
* [x] "080-store.md"
* [x] "090-program.md"
* [x] "100-complex.md"
* [x] "110-time.md"
* [x] "120-trig.md"
* [x] "130-logs.md"
* [x] "140-error.md"
* [x] "150-bitwise.md"
* [x] "999-manual-tests.md"
* [x] "all.md"

## doc

* [x] help
* [x] CHANGELOG.md
* [x] README.md
* [x] license inside the files
* [ ] files headers / comments

## process changes

* [ ] cleaning gha
* [ ] cleaning tickets, trash gh project if decided
* [ ] simplest git branching model, no-ticket commits allowed

## deploying

* [ ] flathub
* [ ] ubuntu
* [ ] fedora

* [ ] crate

* [ ] snap
* [ ] arch
