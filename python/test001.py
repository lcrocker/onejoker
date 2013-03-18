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
        deck = oj.Sequence(52)
        deck.fill(52, oj.dt_standard)

        hand1 = oj.Sequence(7)
        hand2 = oj.Sequence(7)

        hand1.append("AcKd")
        hand2.append("2h2s")

        deck.remove(hand1)
        deck.remove(hand2)

        win1, win2, ties = 0, 0, 0

        boards = deck.iter_combinations(5)
        print("{0} total boards.".format(deck.iter.total))

        for board in boards:
            hand1.append(board)
            v1 = oj.poker_eval(hand1)
            hand1.truncate(2)

            hand2.append(board)
            v2 = oj.poker_eval(hand2)
            hand2.truncate(2)

            if (v1 < v2):   win1 += 1
            elif (v2 < v1): win2 += 1
            else:           ties += 1

        print("{0}={1}, {2}={3}, ties={4}".format(hand1, win1, hand2, win2, ties))

if __name__ == "__main__":
    App().run()
    sys.exit(0)
