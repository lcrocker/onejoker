#!/usr/bin/env python3
#
# Python script that calculates the lookup tables for the poker
# evaluator. Of course, it requres an existing evalutator to calculate
# the table. I initially used the Suffecool/Senzee 5-card evaluator
# which is fast and compact, but this code is deadly slow anyway. That's
# my fault, not theirs. It's here for historical interest.

import sys
from array import array
sys.path.append(".")
import onejoker as oj, cardnames as cn, tablebuilder

n52k5 = oj.binomial(52, 5)
n13k4 = oj.binomial(13, 4)
n13k3 = oj.binomial(13, 3)
n13k2 = oj.binomial(13, 2)

table5 = array('h', (0 for i in range(n52k5)))
table4 = array('h', (0 for i in range(52 * (13 ** 4 + 4 * n13k4))))
table3 = array('l', (0 for i in range(52 * (13 ** 3 + 4 * n13k3))))
table2 = array('l', (0 for i in range(52 * (13 ** 2 + 4 * n13k2))))
table1 = array('h', (0 for i in range(52 * 52)))

def id5(hand):
    a = [   hand[0] - 1, hand[1] - 1, hand[2] - 1,
            hand[3] - 1, hand[4] - 1 ]
    a.sort()

    r = 0
    for i in range(5):
        r += oj.binomial(a[i], i + 1)
    return r

def id4(hand):
    a = [   (hand[0] - 1) >> 2,
            (hand[1] - 1) >> 2,
            (hand[2] - 1) >> 2,
            (hand[3] - 1) >> 2 ]
    a.sort()

    s = (hand[0] - 1) & 3
    if (    s == ((hand[1] - 1) & 3) and
            s == ((hand[2] - 1) & 3) and
            s == ((hand[3] - 1) & 3) ):
        r = 0
        for i in range(4):
            r += oj.binomial(a[i], i + 1)
        r = 13**4 + (s * n13k4) + r
    else:
        r = (13**3) * a[0] + (13**2) * a[1] + 13 * a[2] + a[3]
    return r

def id3(hand):
    a = [   (hand[0] - 1) >> 2,
            (hand[1] - 1) >> 2,
            (hand[2] - 1) >> 2 ]
    a.sort()

    s = (hand[0] - 1) & 3
    if (    s == ((hand[1] - 1) & 3) and
            s == ((hand[2] - 1) & 3) ):
        r = 0
        for i in range(3):
            r += oj.binomial(a[i], i + 1)
        r = 13**3 + (s * n13k3) + r
    else:
        r = (13**2) * a[0] + 13 * a[1] + a[2]
    return r

def id2(hand):
    a = [   (hand[0] - 1) >> 2,
            (hand[1] - 1) >> 2 ]
    a.sort()

    s = (hand[0] - 1) & 3
    if s == ((hand[1] - 1) & 3):
        r = oj.binomial(a[0], 1) + oj.binomial(a[1], 2)
        r = 13 * 13 + (s * n13k2) + r
    else:
        r = 13 * a[0] + a[1]
    return r

class App(object):
    def __init__(self):
        self.deck = oj.Sequence(52)
        self.deck.fill()

    def make_table5(self):
        print("Table 5 contains {0} entries.".format(len(table5)), file=sys.stderr)
        allhands = self.deck.iter_combinations(5)
        print("{0:8d} total combinations.".format(self.deck.iter.total), file=sys.stderr)

        for h in allhands:
            if 0 == (self.deck.iter.remaining & 0xFFFF):
                print("\r{0:8d} remaining.".format(self.deck.iter.remaining), end="", file=sys.stderr)

            r = self.deck.iter.total - self.deck.iter.remaining
            v = oj.poker_eval(h)
            table5[r] = v
        print("\r       0 remaining.", file=sys.stderr)

    def make_table4(self):
        print("Table 4 contains {0} entries.".format(len(table4)), file=sys.stderr)
        allhands = self.deck.iter_combinations(4)
        print("{0:8d} total combinations.".format(self.deck.iter.total), file=sys.stderr)

        for h in allhands:
            if 0 == (self.deck.iter.remaining & 0x0FFF):
                print("\r{0:8d} remaining.".format(self.deck.iter.remaining), end="", file=sys.stderr)

            r = id4(h)
            if 0 != table4[52 * r]:
                continue

            h2 = oj.Sequence(5, h)
            h2.append(53)
            for c in range(52):
                if (c + 1) in h:
                    table4[52 * r + c] = 9999
                else:
                    h2[4] = c + 1
                    table4[52 * r + c] = table5[id5(h2)]
        print("\r       0 remaining.", file=sys.stderr)

    def make_table3(self):
        print("Table 3 contains {0} entries.".format(len(table3)), file=sys.stderr)
        allhands = self.deck.iter_combinations(3)
        print("{0:8d} total combinations.".format(self.deck.iter.total), file=sys.stderr)

        for h in allhands:
            if 0 == (self.deck.iter.remaining & 0x0FFF):
                print("\r{0:8d} remaining.".format(self.deck.iter.remaining), end="", file=sys.stderr)

            r = id3(h)
            if 0 != table3[52 * r]:
                continue

            h2 = oj.Sequence(5, h)
            h2.append(53)
            for c in range(52):
                if (c + 1) in h:
                    table3[52 * r + c] = 9999
                else:
                    h2[3] = c + 1
                    table3[52 * r + c] = id4(h2)
        print("\r       0 remaining.", file=sys.stderr)

    def make_table2(self):
        print("Table 2 contains {0} entries.".format(len(table2)), file=sys.stderr)
        allhands = self.deck.iter_combinations(2)
        print("{0:8d} total combinations.".format(self.deck.iter.total), file=sys.stderr)

        for h in allhands:
            r = id2(h)
            if 0 != table3[52 * r]:
                continue

            h2 = oj.Sequence(5, h)
            h2.append(53)
            for c in range(52):
                if (c + 1) in h:
                    table2[52 * r + c] = -1
                else:
                    h2[2] = c + 1
                    table2[52 * r + c] = id3(h2)

    def make_table1(self):
        h = oj.Sequence(5)

        for i in range(52):
            for j in range(52):
                h.clear()
                h.append([i+1, j+1])
                table1[52 * i + j] = id2(h)

    def make_tables(self):
        # self.make_table5()
        # self.make_table4()
        self.make_table3()
        self.make_table2()
        self.make_table1()

    def write_tables(self):
        b = tablebuilder.TableBuilder("table3", "unsigned")
        b.dump1d(table3)

        b = tablebuilder.TableBuilder("table2", "unsigned")
        b.dump1d(table2)

        b = tablebuilder.TableBuilder("table1", "unsigned short")
        b.dump1d(table1)

    def run(self):
        self.make_tables()
        self.write_tables()

if __name__ == "__main__":
    App().run()
    sys.exit(0)
