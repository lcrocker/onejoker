#!/usr/bin/env python3
#
# Sample use of the OneJoker library.
#

import sys
sys.path.append(".")
import onejoker as oj

class App(object):
    def __init__(self):
        pass

    def run(self):
        s = oj.Sequence(5)
        print(s)
        s.append(15)
        print(s)
        s.append("Kc")
        print(s)
        s = oj.Sequence("Kd Ts 3c 2d 7s")
        print(s)

if __name__ == "__main__":
    App().run()
    sys.exit(0)
