#!/usr/bin/env python3
# OneJoker library <https://github.com/lcrocker/OneJoker>
#
# Build the lookup tables for the poker evaluator. Of course, this requres an
# existing evalutator to calculate the table. I used the Suffecool/Senzee
# 5-card evaluator which is very fast and compact, but not included here.
#

import sys
if sys.version < "3.0":
    print("Python 3 required.")
    sys.exit(1)

from array import array

import onejoker as oj, cardnames as cn, lookuptables
sys.path.append(".")
import lookuptables

table5 = array('h', (-1 for i in range(oj.binomial(52, 5))))
table4 = array('h', (-1 for i in range(52 * 4680 + 1)))
table3 = array('l', (-1 for i in range(52 * 1599 + 1)))
table2 = array('l', (-1 for i in range(52 * 403 + 1)))
table1 = array('h', (-1 for i in range(53 * 53)))

def tolist(hand):
    r = []
    for card in hand:
        r.append(card - 1)
    return r

def colex_rank(seq):
    r = 0
    i = 1
    for v in seq:
        r += oj.binomial(v, i)
        i += 1
    return r

# This is the magic table compression trick. Create a unique ID for the
# set of cards given like this: if it's a flush, then all the ranks are
# unique, so calculate the colex rank of the sequence of card ranks.  If
# not a flush, then interpret the sequence as a base-13 number. The table
# is laid out with the non-flush group followed by the four flush groups.

def id2(seq):
    suit = seq[0] & 3
    flush = all( suit == (v & 3) for v in seq )
    s = [ (v >> 2) for v in seq ]
    s.sort()

    if flush:
        return suit * 78 + colex_rank(s)

    # Different ranks
    elif s[0] != s[1]:
        return    4 * 78 + colex_rank(s)

    # Pair
    else:
        return    5 * 78 + s[0]

def id3(seq):
    suit = seq[0] & 3
    flush = all( suit == (v & 3) for v in seq )
    s = [ (v >> 2) for v in seq ]
    s.sort()

    if flush:
        return suit * 286          + colex_rank(s)

    # All unique
    elif s[0] != s[1] and s[0] != s[2] and s[1] != s[2]:
        return    4 * 286          + colex_rank(s)

    # One pair
    elif s[0] == s[1] and s[0] != s[2]:
        return    5 * 286          + colex_rank([s[0], s[2]])
    elif s[1] == s[2] and s[0] != s[1]:
        return    5 * 286 +     78 + colex_rank([s[0], s[1]])

    # Trips
    else:
        assert(s[0] == s[1] and s[0] == s[2])
        return    5 * 286 + 2 * 78 + s[0]

def id4(seq):
    suit = seq[0] & 3
    flush = all( suit == (v & 3) for v in seq )
    s = [ (v >> 2) for v in seq ]
    s.sort()

    if flush:
        r =  suit * 715                    + colex_rank(s)

    # All unique
    elif s[0] != s[1] and s[0] != s[2] and s[0] != s[3] and s[1] != s[2] and s[1] != s[3] and s[2] != s[3]:
        r =     4 * 715                    + colex_rank(s)

    # One pair
    elif s[0] == s[1] and s[0] != s[2] and s[0] != s[3] and s[2] != s[3]:
        r =     5 * 715                    + colex_rank([s[0], s[2], s[3]])
    elif s[0] != s[1] and s[1] == s[2] and s[0] != s[3] and s[1] != s[3]:
        r =     5 * 715 +     286          + colex_rank([s[0], s[1], s[3]])
    elif s[0] != s[1] and s[0] != s[2] and s[1] != s[2] and s[2] == s[3]:
        r =     5 * 715 + 2 * 286          + colex_rank([s[0], s[1], s[2]])

    # Two pair
    elif s[0] == s[1] and s[0] != s[2] and s[2] == s[3]:
        r =     5 * 715 + 3 * 286          + colex_rank([s[0], s[2]])

    # Trips
    elif s[0] == s[1] and s[0] == s[2] and s[0] != s[3]:
        r =     5 * 715 + 3 * 286 +     78 + colex_rank([s[0], s[3]])
    elif s[0] != s[1] and s[1] == s[2] and s[1] == s[3]:
        r =     5 * 715 + 3 * 286 + 2 * 78 + colex_rank([s[0], s[1]])

    # Quads
    else:
        assert(s[0] == s[1] and s[0] == s[2] and s[0] == s[3])
        r =     5 * 715 + 3 * 286 + 3 * 78 + s[0]
    return r

def id5(seq):
    s = sorted(seq)
    v = colex_rank(s)
    return v


class App(object):
    def __init__(self):
        self.deck = oj.Sequence(52)
        self.deck.fill()

    def make_table5(self):
        allhands = self.deck.iter_combinations(5)
        print("Evaluating {0} hands for table 5.".format(self.deck.iter.total),
            file = sys.stderr)

        for h in allhands:
            r = self.deck.iter.total - self.deck.iter.remaining
            v = oj.poker_eval(h)
            table5[r] = v

    def make_table4(self):
        print("Building table 4.", file = sys.stderr)
        flags = array("B", (0 for i in range(7462)))
        table4[0] = 0

        for h in self.deck.iter_combinations(4):
            l4 = tolist(h)
            r4 = id4(l4)

            if -1 != table4[52 * r4 + 1]:
                continue

            for c in range(52):
                if c in l4:
                    table4[52 * r4 + 1 + c] = 0
                else:
                    l4.append(c)
                    v = id5(l4)
                    l4.pop()
                    table4[52 * r4 + 1 + c] = table5[v]
                    flags[table5[v] - 1] = 1
        assert(all(flags))

    def make_table3(self):
        print("Building table 3.", file = sys.stderr)
        flags = array("B", (0 for i in range(4680)))
        table3[0] = 0

        for h in self.deck.iter_combinations(3):
            l3 = tolist(h)
            r3 = id3(l3)

            if -1 != table3[52 * r3 + 1]:
                continue

            for c in range(52):
                if c in l3:
                    table3[52 * r3 + 1 + c] = 0
                else:
                    l3.append(c)
                    v = id4(l3)
                    flags[v] = 1
                    l3.pop()
                    table3[52 * r3 + 1 + c] = 52 * v
        assert(all(flags))

    def make_table2(self):
        print("Building table 2.", file = sys.stderr)
        flags = array("B", (0 for i in range(1599)))
        table2[0] = 0

        for h in self.deck.iter_combinations(2):
            l2 = tolist(h)
            r2 = id2(l2)

            if -1 != table2[52 * r2 + 1]:
                continue

            for c in range(52):
                if c in l2:
                    table2[52 * r2 + 1 + c] = 0
                else:
                    l2.append(c)
                    v = id3(l2)
                    flags[v] = 1
                    l2.pop()
                    table2[52 * r2 + 1 + c] = 52 * v
        assert(all(flags))

    def make_table1(self):
        print("Building table 1.", file = sys.stderr)
        flags = array("B", (0 for i in range(403)))
        for i in range(53):
            for j in range(53):
                if 0 == i or 0 == j or i == j:
                    table1[53 * i + j] = 0
                else:
                    v = id2([i - 1, j - 1])
                    table1[53 * i + j] = 52 * v
                    flags[v] = 1
        assert(all(flags))

    def make_tables(self):
        self.make_table5()
        self.make_table4()
        self.make_table3()
        self.make_table2()
        self.make_table1()

    def write_tables(self):
        b = lookuptables.Printer("t1", "static unsigned short")
        b.dump1d(table1)
        b = lookuptables.Printer("t2", "static unsigned int")
        b.dump1d(table2)
        b = lookuptables.Printer("t3", "static unsigned int")
        b.dump1d(table3)
        b = lookuptables.Printer("t4", "static unsigned short")
        b.dump1d(table4)

    def run(self):
        self.make_tables()
        self.write_tables()

if __name__ == "__main__":
    App().run()
    sys.exit(0)
