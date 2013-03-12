#!/usr/bin/env python3
#
# binomial.py
#
# Used to build the lookup table for the fast binomial coefficient function
# in the OneJoker library.
#

import sys

class App(object):
    def __init__(self):
        self.table = [[0 for k in range(55)] for n in range(55)]

    def build(self):
        for n in range(55):
            for k in range(55):
                if k == 0:
                    self.table[n][k] = 1
                if k > n:
                    self.table[n][k] = 0
                elif k == n:
                    self.table[n][k] = 1
                elif k == 1:
                    self.table[n][k] = n
                elif k > n-k:
                    self.table[n][k] = self.table[n][n-k]
                else:
                    self.table[n][k] = self.table[n-1][k] + self.table[n-1][k-1]

    def output(self, val):
        text = " {0}LL,".format(val)
        if self.column + len(text) > 80:
            print("\n   ", end = "")
            self.column = 3
        print(text, end = "")
        self.column += len(text)

    def dump(self):
        count = 0
        self.column = 80
        for n in range(55):
            for k in range(55):
                if n < 4 or k < 2 or k > n-k:
                    continue
                self.output(self.table[n][k])
                count += 1
        print()

    def run(self):
        self.build()
        self.dump()


def main(args):
    return App().run()

if __name__ == "__main__":
    sys.exit(main(sys.argv))

