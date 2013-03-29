#!/usr/bin/env python3
# OneJoker library <https://github.com/lcrocker/OneJoker>
#
# Python language binding for OneJoker library. Uses ctypes to access the
# shared library.
#

import sys, itertools as it
if sys.version < "3.0":
    print("Python 3 required.")
    sys.exit(1)

from ctypes import *
from ctypes.util import find_library

ojlib = CDLL(find_library("onejoker"))
ojlib.ojc_binomial.restype = c_longlong
ojlib.ojc_rank.restype = c_longlong

import onejoker.text as ot

r_deuce = 0
r_trey = 1
r_four = 2
r_five = 3
r_six = 4
r_seven = 5
r_eight = 6
r_nine = 7
r_ten = 8
r_jack = 9
r_queen = 10
r_king = 11
r_ace = 12
r_joker = 13

s_club = 0
s_diamond = 1
s_heart = 2
s_spade = 3

c_joker = 53
c_joker2 = 54

dt_standard = 0
dt_1joker = 1
dt_2jokers = 2
dt_stripped32 = 3
dt_stripped40 = 4
dt_stripped40j = 5

pg_standard = 0
pg_acetofive = 1
pg_deucetoseven = 2
pg_acetosix = 3
pg_badugi = 4

def rank(c):
    return (c - 1) >> 2

def suit(c):
    return (c - 1) & 3

def seed(val):
    return ojlib.ojr_seed(val)

def rand(limit):
    return ojlib.ojr_rand(limit)

def binomial(n, k):
    if (n <= 54 and k <= 54):
        return ojlib.ojc_binomial(n, k)
    b = 1
    for i in range(1, k + 1):
        b *= (n - (k - i));
        b //= i
    return b

class Sequence(Structure):
    _fields_ = [
        ("_johnnymoss", c_int),
        ("allocation", c_int),
        ("length", c_int),
        ("cards", POINTER(c_int))
    ]

    def __init__(self, allocation, init = None):
        self.buf = cast(create_string_buffer(4 * allocation), POINTER(c_int))
        ojlib.ojs_new(byref(self), allocation, self.buf)
        if init:
            self.append(init)

    def __len__(self):
        return self.length

    def __iter__(self):
        for i in range(self.length):
            yield self.cards[i]

    def __str__(self):
        return "({0})".format(ot.cardnames(
            (self.cards[i] for i in range(self.length)), " "))

    def __eq__(self, other):
        if isinstance(other, Sequence):
            return ojlib.ojs_equal(byref(self), byref(other))
        else:
            if isinstance(other, str):
                other = ot.cardnums(other)
            return all( a == b for a, b in it.zip_longest(other,
                (self.cards[i] for i in range(self.length)), fillvalue = 0) )

    def __getitem__(self, index):
        if index >= self.length:
            raise IndexError
        return self.cards[index]

    def __setitem__(self, index, val):
        if index >= self.length:
            raise IndexError
        if isinstance(val, str):
            val = cardnum(val)
        self.cards[index] = val

    def __contains__(self, val):
        if isinstance(val, str):
            val = ot.cardnum(val)
        return (-1 != ojlib.ojs_index(byref(self), val))

    def clear(self):
        self.length = 0

    def delete(self, index):
        return ojlib.ojs_delete(byref(self), index)

    def truncate(self, length):
        if (length <= self.length):
            self.length = length

    def append(self, arg):
        if isinstance(arg, str):
            for c in ot.cardnums(arg):
                ojlib.ojs_append(byref(self), c)
        elif hasattr(arg, "__iter__"):
            for v in arg:
                ojlib.ojs_append(byref(self), v)
        else:
            ojlib.ojs_append(byref(self), arg)

    def set(self, arg):
        self.length = 0
        self.append(arg)

    def insert(self, index, arg):
        if isinstance(arg, str):
            for c in ot.cardnums(arg):
                ojlib.ojs_insert(byref(self), index, c)
                index += 1
        elif hasattr(arg, "__iter__"):
            for v in arg:
                ojlib.ojs_insert(byref(self), index, v)
                index += 1
        else:
            ojlib.ojs_insert(byref(self), index, arg)

    def remove(self, arg):
        if isinstance(arg, str):
            for c in ot.cardnums(arg):
                v = ojlib.ojs_remove(byref(self), c)
            return v
        elif hasattr(arg, "__iter__"):
            for v in arg:
                v = ojlib.ojs_remove(byref(self), v)
            return v
        else:
            return ojlib.ojs_remove(byref(self), arg)

    def pop(self):
        return ojlib.ojs_pop(byref(self))

    def index(self, card):
        if isinstance(card, str):
            card = ot.cardnum(card)
        return ojlib.ojs_index(byref(self), card)

    def fill(self, count = 52, type = None):
        if type is None:
            d = { 32: dt_stripped32, 40: dt_stripped40,
                41: dt_stripped40j, 53: dt_1joker, 54: dt_2jokers }
            if count in d:
                type = d[count]
            else:
                type = dt_standard
        ojlib.ojs_fill(byref(self), count, type)

    def sort(self):
        ojlib.ojs_sort(byref(self))

    def reverse(self):
        ojlib.ojs_sort(byref(self))

    def shuffle(self):
        ojlib.ojs_sort(byref(self))


class Iterator(Structure):
    _fields_ = [
        ("_johnnymoss", c_int),
        ("k", c_int),
        ("a", POINTER(c_int)),
        ("total", c_longlong),
        ("count", c_longlong),
        ("remaining", c_longlong),
        ("deck", POINTER(Sequence)),
        ("hand", POINTER(Sequence)),
        ("deck_invert", c_int * 64)
    ]

    def __init__(self, deck, k):
        self.h = Sequence(k)
        self.buf = cast(create_string_buffer(4 * k), POINTER(c_int))
        ojlib.ojc_iter_new(byref(self), byref(deck), byref(self.h),
            k, self.buf, c_longlong(0))

    def all(self):
        while ojlib.ojc_iter_next(byref(self)):
            yield self.hand.contents

    def random(self, count):
        self.count = count
        self.remaining = count
        while ojlib.ojc_iter_next_random(byref(self)):
            yield self.hand.contents

    def rank(self, hand):
        if isinstance(hand, str):
            hand = Sequence(self.k, hand)
        return ojlib.ojc_rank(byref(hand), byref(self))

    def hand_at(self, rank):
        h = Sequence(self.k)
        ojlib.ojc_hand_at(c_longlong(rank), byref(h), byref(self))
        return h

class PokerGame(object):
    def __init__(self):
        pass


def poker_eval5(hand):
    return ojlib.ojp_eval5(byref(hand))

def poker_best5(hand):
    b = Sequence(5)
    v = ojlib.ojp_best5(byref(hand), byref(b))
    return v, b
