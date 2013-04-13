#!/usr/bin/env python3
# OneJoker library <https://github.com/lcrocker/OneJoker>
#
# Functions for text I/O of cards. Uses regular expression module.
#

import sys, re
if sys.version < "3.0":
    print("Python 3 required.")
    sys.exit(1)

from ctypes import *
from ctypes.util import find_library
ojlib = CDLL(find_library("onejoker"))
ojlib.oj_cardname.restype = c_char_p
ojlib.oj_rankname.restype = c_char_p
ojlib.oj_suitname.restype = c_char_p

_cnamepattern = re.compile("""
    \\s*
    (
        (jk|j2|joker)
        |
        (
            (2|3|4|5|6|7|8|9|10|t|j|q|k|a)
            \\s*
            (c|d|h|s)
        )
    )""", re.IGNORECASE | re.VERBOSE)

_cardnames = [ (ojlib.oj_cardname(c).decode()) for c in range(1, 54) ]
_ranknames = [ (ojlib.oj_rankname(r).decode()) for r in range(14) ]
_suitnames = [ (ojlib.oj_suitname(s).decode()) for s in range(4) ]

def cardname(c):
    assert(c >= 1 and c <= 54);
    return _cardnames[c - 1]

def cardnames(vals, sep = ""):
    return sep.join(cardname(v) for v in vals)

def rankname(r):
    assert(r >= 0 and r <= 13)
    return _ranknames[r]

def suitname(s):
    assert(s >= 0 and s <= 3)
    return _suitnames[s]

def _cardnum(groups):
    if groups[1]:
        if "j2" == groups[1].lower():
            return 54
        else:
            return 53

    r = groups[3].lower()
    s = groups[4].lower()
    r = { "2":0, "3":1, "4":2, "5":3, "6":4, "7":5, "8":6, "9":7,
        "10":8, "t":8, "j":9, "q":10, "k":11, "a":12 }[r]
    s = { "c":1, "d":2, "h":3, "s": 4 }[s]
    return 4 * r + s

def cardnum(text):
    match = _cnamepattern.match(text)
    if match is not None:
        return _cardnum(match.groups())

def cardnums(text):
    res = []
    for match in _cnamepattern.findall(text):
        res.append(_cardnum(match))
    return res
