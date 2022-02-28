# Generation

rpn is proposed for **GNU/Linux**.

It can be generated following the steps below.

## Version identification

rpn is dynamically linked against GNU MP and GNU MPFR.

and embeds the source code of linenoise-ng and as a submodule

It is necessary to get MPFR to generate rpn

## Install GNU MPFR headers

ubuntu 20.04
git libmpfr6 cmake

- download **GNU MPFR** from http://www.mpfr.org
- install it with usual autotools commands `./configure && make && make install`

## Generate rpn

- clone [rpn project](https://github.com/louisrubet/rpn/) or download a zipped version from [the release directory](https://github.com/louisrubet/rpn/releases)

```shell
git clone https://github.com/louisrubet/rpn.git
```

- make

CMake must be installed on the generation machine

```shell
cd rpn/
mkdir build
cd build && cmake .. && make
```

- install

```
cd rpn/build
sudo make install
```

- packages

    To build RPM package you should install CMake 2.6.0 or higher

```shell
cd rpn/build
sudo make package
[100%] Built target rpn
Run CPack packaging tool...
CPack: Create package using DEB
(...)
CPack: Create package using RPM
(...)
CPack: Create package using TGZ
(...)
```
