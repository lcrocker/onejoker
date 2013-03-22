#!/usr/bin/env python3
# OneJoker library <https://github.com/lcrocker/OneJoker>
#
# Build the lookup tables for the poker evaluator. Of course, this requres an
# existing evalutator to calculate the table. I originally used the "Cactus"
# Kevin Suffecool/Paul Senzee 5-card evaluator which is very fast and compact,
# but not included here.
#

sys.exit(1) # This is currently not working!!!

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
table1 = array('l', (-1 for i in range(53 * 53)))

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

# This is the magic table compression trick. Create a unique ID for the set of
# cards given like this: sort the combination, group first by flush/no flush,
# remove the suits and group by pairs, and assign IDs by the rank within group.

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
    # Unique ranks
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
    # Unique
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

# Table 5 is just the rank of all 2598960 poker hands in colex order
#
def id5(seq):
    s = sorted(seq)
    v = colex_rank(s)
    return v

def progress(count):
    if (0 == (count & 0x1FFF)):
        print("\r{0:8d} remaining.".format(count), end = "", file = sys.stderr)


class App(object):
    def __init__(self):
        self.deck = oj.Sequence(52)
        self.deck.fill()

    def make_table5(self):
        hands = oj.Iterator(self.deck, 5)
        print("\n{0:8d} total hands.".format(hands.total), file = sys.stderr)

        for h in hands.all():
            progress(hands.remaining)
            r = hands.total - hands.remaining - 1
            table5[r] = oj.poker_eval(h)

    def make_table4(self):
        hands = oj.Iterator(self.deck, 4)
        print("\n{0:8d} hands at level 4.".format(hands.total), file = sys.stderr)

        for h in hands.all():
            progress(hands.remaining)
            l4 = tolist(h)
            r4 = id4(l4)
            l5 = list(l4)

            if -1 != table4[54 * r4]:
                continue

            for c in range(52):
                if c in l4:
                    table4[52 * r4 + c] = 9999
                else:
                    l5.append(c)
                    v = id5(l5)
                    l5.pop()
                    table4[52 * r4 + c] = table5[v]
        print()

    def make_table3(self):
        hands = oj.Iterator(self.deck, 3)
        print("\n{0:8d} hands at level 3.".format(hands.total), file = sys.stderr)

        for h in hands.all():
            progress(hands.remaining)
            l3 = tolist(h)
            r3 = id3(l3)
            l4 = list(l3)

            if -1 != table4[54 * r4]:
                continue

            for c in range(52):
                if c in l3:
                    table3[52 * r3 + c] = 999999
                else:
                    l4.append(c)
                    v = id4(l4)
                    l4.pop()
                    table3[52 * r3 + c] = 52 * v

    def make_table2(self):
        hands = oj.Iterator(self.deck, 2)
        print("\n{0:8d} hands at level 2.".format(hands.total), file = sys.stderr)
        table2[0] = 0

        for h in hands.all():
            l2 = tolist(h)
            r2 = id2(l2)
            l3 = list(l2)

            if -1 != table4[54 * r4]:
                continue

            for c in range(52):
                if c in l2:
                    table2[52 * r2 + c] = 999999
                else:
                    l3.append(c)
                    v = id3(l3)
                    l3.pop()
                    table2[52 * r2 + c] = 52 * v

    def make_table1(self):
        for i in range(52):
            for j in range(52):
                if i == j:
                    table1[52 * i + j] = 29999
                else:
                    v = id2([i, j])
                    table1[52 * i + j] = 52 * v

    def make_tables(self):
        self.make_table5()
        self.make_table4()
        self.make_table3()
        self.make_table2()
        self.make_table1()

    def write_tables(self):
        b = lookuptables.Printer("lct1", "static unsigned short")
        b.dump1d(table1)
        b = lookuptables.Printer("lct2", "static unsigned int")
        b.dump1d(table2)
        b = lookuptables.Printer("lct3", "static unsigned int")
        b.dump1d(table3)
        b = lookuptables.Printer("lct4", "static unsigned short")
        b.dump1d(table4)

    def run(self):
        self.make_tables()
        self.write_tables()

if __name__ == "__main__":
    App().run()
    sys.exit(0)
