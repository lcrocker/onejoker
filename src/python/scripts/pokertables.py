#!/usr/bin/env python3
# OneJoker library <http://lcrocker.github.io/OneJoker>
#
# Build the lookup tables for the poker evaluator. Of course, this requres an
# existing evaluator to calculate the table. I originally used the "Cactus"
# Kevin Suffecool/Paul Senzee 5-card evaluator which is very fast and compact,
# but not included here.
#

import sys
if sys.version < "3.0":
    print("Python 3 required.")
    sys.exit(1)

from array import array
import itertools as it

import onejoker as oj, cardnames as cn, lookuptables
sys.path.append(".")
import lookuptables

table4 = array('h', (-1 for i in range(52 * 4680)))
table3 = array('l', (-1 for i in range(52 * 1599)))
table2 = array('l', (-1 for i in range(52 * 403)))
table1 = array('h', (0 for i in range(52 * 52)))

deck = oj.Sequence(52)
deck.fill()

# Return the rank of the give sequence in colexicographical order
#
def clrank(seq):
    r = 0
    i = 1
    for v in seq:
        r += oj.binomial(v, i)
        i += 1
    return r

# Check that the sequence matches the given pattern's equality relationships.
#
def eqpattern(p, seq):
    for i in range(len(seq) - 1):
        for j in range(i + 1, len(seq)):
            if ((seq[i] == seq[j]) != (p[i] == p[j])):
                return False
    return True

# This is the magic table compression trick. Create a unique ID for the set of
# cards given like this: sort the combination, group first by flush/no flush,
# then remove the suits and group by pairs, and assign IDs by the rank within
# the group.
#

def id2(seq):
    suit = seq[0] & 3
    flush = all( (suit == (v & 3)) for v in seq )
    s = [ (v >> 2) for v in seq ]
    s.sort()

    if flush:
        return suit * 78 + clrank(s)
    elif s[0] != s[1]:
        return    4 * 78 + clrank(s)
    else:
        return    5 * 78 + s[0]

def id3(seq):
    suit = seq[0] & 3
    flush = all( (suit == (v & 3)) for v in seq )
    s = [ (v >> 2) for v in seq ]
    s.sort()

    if flush:
        return suit * 286          + clrank(s)
    elif eqpattern("ABC", s):
        return    4 * 286          + clrank(s)
    elif eqpattern("AAB", s):
        return    5 * 286          + clrank([s[0], s[2]])
    elif eqpattern("ABB", s):
        return    5 * 286 +     78 + clrank([s[0], s[1]])
    else:
        return    5 * 286 + 2 * 78 + s[0]

def id4(seq):
    suit = seq[0] & 3
    flush = all( (suit == (v & 3)) for v in seq )
    s = [ (v >> 2) for v in seq ]
    s.sort()

    if flush:
        return suit * 715                    + clrank(s)
    elif eqpattern("ABCD", s):
        return    4 * 715                    + clrank(s)
    elif eqpattern("AABC", s):
        return    5 * 715                    + clrank([s[0], s[2], s[3]])
    elif eqpattern("ABBC", s):
        return    5 * 715 +     286          + clrank([s[0], s[1], s[3]])
    elif eqpattern("ABCC", s):
        return    5 * 715 + 2 * 286          + clrank([s[0], s[1], s[2]])
    elif eqpattern("AABB", s):
        return    5 * 715 + 3 * 286          + clrank([s[0], s[2]])
    elif eqpattern("AAAB", s):
        return    5 * 715 + 3 * 286 +     78 + clrank([s[0], s[3]])
    elif eqpattern("ABBB", s):
        return    5 * 715 + 3 * 286 + 2 * 78 + clrank([s[0], s[1]])
    else:
        return    5 * 715 + 3 * 286 + 3 * 78 + s[0]

def progress(count):
    if (0 == (count & 0x1FFF)):
        print("\r{0:8d}".format(count), end = "", file = sys.stderr)

def make_table4():
    hands = oj.Iterator(deck, 4)
    print("Building table 4.", file = sys.stderr)

    h5 = oj.Sequence(5)
    for h in hands.all():
        progress(hands.remaining)
        l4 = [ (c - 1) for c in h ]
        r4 = id4(l4)

        for c in range(52):
            if (c in l4) or (-1 != table4[52 * r4 + c]):
                continue

            h5.set(h)
            h5.append(c + 1)
            table4[52 * r4 + c] = oj.poker_eval(h5)
    print("\r", end = "", file = sys.stderr)

def make_table3():
    hands = oj.Iterator(deck, 3)
    print("Building table 3.", file = sys.stderr)

    for h in hands.all():
        progress(hands.remaining)
        l3 = [ (c - 1) for c in h ]
        r3 = id3(l3)
        l4 = list(l3)

        for c in range(52):
            if (c in l3) or (-1 != table3[52 * r3 + c]):
                continue

            l4.append(c)
            v = id4(l4)
            l4.pop()
            table3[52 * r3 + c] = 52 * v
    print("\r", end = "", file = sys.stderr)

def make_table2():
    hands = oj.Iterator(deck, 2)
    print("Building table 2.", file = sys.stderr)

    for h in hands.all():
        l2 = [ (c - 1) for c in h ]
        r2 = id2(l2)
        l3 = list(l2)

        for c in range(52):
            if (c in l2) or (-1 != table2[52 * r2 + c]):
                continue

            l3.append(c)
            v = id3(l3)
            l3.pop()
            table2[52 * r2 + c] = 52 * v

def make_table1():
    print("Building table 1.", file = sys.stderr)
    for i in range(52):
        for j in range(52):
            if i == j:
                table1[52 * i + j] = 0
            else:
                table1[52 * i + j] = 52 * id2([i, j])

class App(object):
    def __init__(self):
        pass

    def make_tables(self):
        make_table4()
        make_table3()
        make_table2()
        make_table1()

    def write_tables(self):
        print("Writing tables.", file = sys.stderr)

        print(
"""/* OneJoker library <http://lcrocker.github.io/OneJoker>
 * Lookup tables for LDC poker hand evaluator.
 * This file is automatically generated by pokertables.py.
 */
""")
        b = lookuptables.Printer("ldc1", "static const short")
        b.dump1d(it.chain([0], table1))

        b = lookuptables.Printer("ldc2", "static const int")
        b.dump1d(it.chain([0], table2))

        b = lookuptables.Printer("ldc3", "static const int")
        b.dump1d(it.chain([0], table3))

        b = lookuptables.Printer("ldc4", "static const short")
        b.dump1d(it.chain([0], table4))

    def run(self):
        self.make_tables()
        self.write_tables()

if __name__ == "__main__":
    App().run()
    sys.exit(0)
