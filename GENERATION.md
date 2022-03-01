# Generation

rpn is proposed for **GNU/Linux**.

It can be generated following the steps below.

rpn is dynamically linked against GNU MP and GNU MPFR and integrates [linenoise-ng](https://github.com/louisrubet/linenoise-ng.git) and [mpreal](http://www.holoborodko.com/pavel/mpfr/) code as git submodules.

## Generate and install under Ubuntu 20.04 LTS

```shell
sudo apt install git cmake g++ libmpfr6 libmpfr-dev
git clone https://github.com/louisrubet/rpn/ 
mkdir -p rpn/build && cd rpn/build
cmake ..
make -j
sudo make install
```

## Generate and install under Fedora 35

```shell
sudo dnf install git cmake g++ mpfrf mpfr-devel
git clone https://github.com/louisrubet/rpn/ 
mkdir -p rpn/build && cd rpn/build
cmake ..
make -j
sudo make install
```
