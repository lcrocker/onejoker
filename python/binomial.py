#!/usr/bin/env python3
# OneJoker library <https://github.com/lcrocker/OneJoker>
#
# Used to build lookup table for the fast binomial coefficient function.
#

import sys
if sys.version < "3.0":
    print("Python 3 required.")
    sys.exit(1)

sys.path.append(".")
import lookuptables

class App(object):
    def __init__(self, max):
        self.table = [[0 for k in range(max + 1)] for n in range(max + 1)]
        self.max = max

    def build(self):
        for n in range(self.max + 1):
            for k in range(self.max + 1):
                if k == 0:      b = 1
                elif  k > n:    b = 0
                elif k == n:    b = 1
                elif k == 1:    b = n
                elif k > n-k:   b = self.table[n][n-k]
                else:
                    b = self.table[n-1][k] + self.table[n-1][k-1]
                self.table[n][k] = b

    def generate(self):
        for n in range(self.max + 1):
            for k in range(self.max + 1):
                if n < 4 or k < 2 or k > n-k:
                    continue
                yield self.table[n][k]

    def output(self):
        b = lookuptables.Printer("bctable", "long long", "{0}LL")
        b.dump1d(self.generate())

    def run(self):
        self.build()
        self.output()
        return 0

def main(args):
    return App(54).run()

if __name__ == "__main__":
    sys.exit(main(sys.argv))
