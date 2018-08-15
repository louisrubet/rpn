# **rpn v2.3.2** - generation

For now rpn is proposed only for **GNU/Linux**

It can be generated following the steps below

## version identification

rpn is dynamically linked against GNU MP and GNU MPFR

and embeds the source code of linenoise-ng as a submodule

It is necessary to get MPFR to generate rpn

## install GNU MPFR headers

- download **GNU MPFR** from http://www.mpfr.org
- install it with usual autotools commands `./configure && make && make install`

## generate rpn

- clone [rpn project](https://github.com/louisrubet/rpn/) or download a zipped version from [the release directory](https://github.com/louisrubet/rpn/releases)
	```
	# git clone https://github.com/louisrubet/rpn.git
	```

- make
	
	CMake must be installed on the generation machine
    
	```
	# cd rpn/
	# mkdir build
	# cd build && cmake .. && make
	```

- install
	```
    # cd rpn/build
	# sudo make install
	```

- packages

    To build RPM package you should install CMake 2.6.0 or higher

	```
    # cd rpn/build
	# sudo make package
    [100%] Built target rpn
    Run CPack packaging tool...
    CPack: Create package using DEB
    (...)
    CPack: Create package using RPM
    (...)
    CPack: Create package using TGZ
    (...)
    ```
