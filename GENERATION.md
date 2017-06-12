# **rpn** - generation

For now rpn is proposed only for **GNU/Linux**

It can be generated following the steps below

## version identification

rpn v2.1 is statically linked against
- GNU MP v6.1.2
- GNU MPFR v3.1.5
- linenoise-ng v1.0.1

## GNU MP v6.1.2

- download and install **GNU MP v6.1.2** from https://gmplib.org/

## GNU MPFR v3.1.5

- download and install **GNU MPFR v3.1.5** from http://www.mpfr.org

## linenoise-ng v1.0.1

- download **linenoise-ng v1.0.1** from https://github.com/arangodb/linenoise-ng

- checkout the tag v1.0.1

- apply the following patch to the source code

```
--- ../../../linenoise-ng/src/linenoise.cpp	2017-06-10 18:13:31.752976287 +0200
+++ linenoise.cpp	2017-06-12 18:54:50.481794824 +0200
@@ -2587,12 +2587,15 @@
 
     // ctrl-I/tab, command completion, needs to be before switch statement
     if (c == ctrlChar('I') && completionCallback) {
+      // lru for rpn
+      #if 0
       if (pos == 0)  // SERVER-4967 -- in earlier versions, you could paste
                      // previous output
         continue;    //  back into the shell ... this output may have leading
                      //  tabs.
       // This hack (i.e. what the old code did) prevents command completion
       //  on an empty line but lets users paste text with leading tabs.
+      #endif
 
       killRing.lastAction = KillRing::actionOther;
       historyRecallMostRecent = false;
```

## rpn

- clone [rpn project](https://github.com/louisrubet/rpn/) or download a zipped version from [the release directory](https://github.com/louisrubet/rpn/releases)
	```
	# git clone https://github.com/louisrubet/rpn.git
	```

- make
	
	CMake must be installed on the generation machine
    
    To build RPM package you should install CMake 2.6.0 or higher
	
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
    - DEB
	```
    # cd rpn/build
	# make package
	```
    - RPM
    rpmbuild must be installed
	```
    # cd rpn/build
    # cpack -G RPM
	```
    - TGZ
	```
    # cd rpn/build
    # cpack -G TGZ
	```
