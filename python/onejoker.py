#!/usr/bin/env python2.7
#
# Python language binding for OneJoker library.
#
from __future__ import print_function

import sys
from ctypes import *
ojlib = CDLL("libonejoker.so")

binomial = ojlib.ojc_binomial
binomial.restype = c_longlong

class Sequence(Structure):
    _fields_ = [
        ("_johnnymoss", c_int),
        ("allocation", c_int),
        ("length", c_int),
        ("cards", POINTER(c_int))
        ]

    def __init__(self, size):
        b = create_string_buffer(4 * size)
        ojlib.ojs_new(byref(self), size, cast(b, POINTER(c_int)))

    def fill(self, count):
        ojlib.ojs_fill(byref(self), count, 0)

    def sort(self):
        ojlib.ojs_sort(byref(self))

    def show(self):
        print("({0:2d})".format(self.length), end="")
        for i in range(self.length):
            print(" {0:2d}".format(self.cards[i]), end="")
        print()

def main():
    s = Sequence(52)
    s.show()

    s.fill(20)
    s.show()

    s.sort()
    s.show()

    return 0

if __name__ == "__main__":
    sys.exit(main())
