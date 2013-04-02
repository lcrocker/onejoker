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

The C library should be buildable just by typing:

    make

at the root of the git repository. The Makefiles will detect Linux and
Windows (MinGW/Cygwin) environments, and act accordingly. Other OSs are not yet
supported, but shouldn't be too difficult to port to. The code is all very
generic C, so porting should mostly involve how to make shared libraries on
your OS. You can use:

    make test

to run the unit tests. The "python" directory contains the python module with a
standard setup.py script. On Linux, you may have to run "ldconfig" after
building the library before running the tests.

Contents
--------

    CMakeLists.txt          Cmake build files (and in subdirectories)
    python/
        onejoker/           The .py files for the module itself
        scripts/            Scripts for tasks like building lookup tables.
        tests/              Samples of use of the module
    src/
        include/            C Header files
        lib/                Library code
        tests/              C-based tests for library

Credits and Copyrights
----------------------

[CC-0][4]: To the extent possible under law, [Lee Daniel Crocker][1] has waived
all copyright and related or neighboring rights to this work.

The JKISS pseudo-random number generation algorithm by [David Jones][5] of the
UCL Bioinformatics Group is also in the public domain.

[1]: http://www.etceterology.com
[4]: http://creativecommons.org/publicdomain/zero/1.0/
[5]: www.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf
