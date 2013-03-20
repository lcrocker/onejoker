#!/usr/bin/env python3
# OneJoker library <https://github.com/lcrocker/OneJoker>
#
# Functions for text I/O of cards. Uses regular expression module.
#

import sys
if sys.version < "3.0":
    print("Python 3 required.")
    sys.exit(1)

import re
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

def cardname(val):
    if (val < 0 or val > 53):
        return "XX"
    else:
        return ( "XX",
            "2c", "2d", "2h", "2s", "3c", "3d", "3h", "3s",
            "4c", "4d", "4h", "4s", "5c", "5d", "5h", "5s",
            "6c", "6d", "6h", "6s", "7c", "7d", "7h", "7s",
            "8c", "8d", "8h", "8s", "9c", "9d", "9h", "9s",
            "Tc", "Td", "Th", "Ts", "Jc", "Jd", "Jh", "Js",
            "Qc", "Qd", "Qh", "Qs", "Kc", "Kd", "Kh", "Ks",
            "Ac", "Ad", "Ah", "As", "JK", "J2" )[val]

def cardnames(vals, sep = ""):
    return sep.join(cardname(v) for v in vals)

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
