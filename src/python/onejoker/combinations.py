# OneJoker library <https://github.com/lcrocker/OneJoker>

import sys, itertools as it
from ctypes import *
from ctypes.util import find_library
ojlib = CDLL(find_library("onejoker"))
from .cardlist import CardList

class Combinations(Structure):
    _fields_ = [
        ("_johnnymoss", c_int),
        ("k", c_int),
        ("a", POINTER(c_int)),
        ("total", c_longlong),
        ("count", c_longlong),
        ("remaining", c_longlong),
        ("deck", POINTER(CardList)),
        ("hand", POINTER(CardList)),
        ("deck_invert", c_int * 64)
    ]

    def __init__(self, deck, k):
        self.h = CardList(k)
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
            hand = CardList(self.k, hand)
        return ojlib.ojc_rank(byref(hand), byref(self))

    def hand_at(self, rank):
        h = CardList(self.k)
        ojlib.ojc_hand_at(c_longlong(rank), byref(h), byref(self))
        return h
