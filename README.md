**OneJoker** is a general-purpose card game and simulation library created by
Lee Daniel Crocker in 2013.

OneJoker consists of a shared library written in pure C, and a Python module
that calls into the shared library. It is designed primarily to be used with
its Python binding, but the C code can be used as a C library in its own right
if you need the greatest possible performance.

The best documentation for OneJoker is currently the [wiki][2] at GitHub:

[2]: https://github.com/lcrocker/OneJoker/wiki

Build and Install
-----------------

To build the library, you will need Python 3 and the GCC tool chain. On Linux,
the standard minimal development evironment will do fine. On Windows, you'll
have to install Python 3 and something like MinGW or Cygwin to get GCC (this
will also provide a few needed standard Unix utilities like "rm").

The `build.py` script at the root of the source tree is used to build the
library and do all the other things usually done by "make" or some other build
automation tool. It is pure Python, so it should be more flexible, more
portable, and smarter than any such generic tool. Anything you might need to
change for your installation should be at the top of the script, but it should
run without any changes on most systems. Just type:

    python3 ./build.py

And it should figure things out. If you want to build the library including
JNI functions to use with Java, you will need to set your environment variable
"JDK" to the directory of your Java installation add a `-j` flag to the build.
This will not affect the usability of the library in C or Python.

    python3 ./build.py -h

will give you a more complete list of build flags and targets. There are also
dummy Makefiles in several directories that just call the build script for
those of you like me whose fingers just type "make" automatically.

Contents
--------

    build.py                Build script
    src/
        include/            C Header files
        lib/                Library code
        tests/              C-based tests for library
    python/
        onejoker/           The .py files for the module itself
        scripts/            Scripts for tasks like building lookup tables.
        tests/              Samples of use of the module
    java/                   JNI bindings and Java tests
        com/
            onejoker/
                onejoker/   Java package and test sources

Credits and Copyrights
----------------------

[CC-0][4]: To the extent possible under law, [Lee Daniel Crocker][1] has waived
all copyright and related or neighboring rights to this work.

The JKISS pseudo-random number generation algorithm by [David Jones][5] of the
UCL Bioinformatics Group is also in the public domain.

[1]: http://www.etceterology.com
[4]: http://creativecommons.org/publicdomain/zero/1.0/
[5]: www.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf
