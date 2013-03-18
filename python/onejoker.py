#!/usr/bin/env python3
#
# Python language binding for OneJoker library.
#

import sys
from ctypes import *
ojlib = CDLL("libonejoker.so")

dt_standard = 0
dt_1joker = 1
dt_2jokers = 2
dt_stripped32 = 3
dt_stripped40 = 4
dt_stripped40j = 5

def seed(val):
    return ojlib.ojr_seed(val)

def rand(limit):
    return ojlib.ojr_rand(limit)

_ojc_binomial = ojlib.ojc_binomial
_ojc_binomial.restype = c_longlong

def binomial(n, k):
    if (n <= 54 and k <= 54):
        return _ojc_binomial(n, k)
    b = 1
    for i in range(1, k + 1):
        b *= (n - (k - i));
        b //= i
    return b

class Iterator(Structure):
    pass

class Sequence(Structure):
    _fields_ = [
        ("_johnnymoss", c_int),
        ("allocation", c_int),
        ("length", c_int),
        ("cards", POINTER(c_int))
    ]

    def __init__(self, arg):
        if isinstance(arg, str):
            cards = cast(create_string_buffer(1024), POINTER(c_int))
            allocation = ojlib.oj_cards_from_text(256, cards, arg.encode())
            length = allocation
        else:
            allocation = arg
            length = 0

        buf = cast(create_string_buffer(4 * allocation), POINTER(c_int))
        ojlib.ojs_new(byref(self), allocation, buf)
        if length > 0:
            memmove(buf, cards, 4 * length)
            self.length = length

    def __str__(self):
        if 0 == self.length:
            return "()"

        text = create_string_buffer(1024)
        o = ojlib.oj_text_from_cards(self.length, self.cards,
            1024, text, " ".encode())
        return "({0})".format(text.value.decode())

    def __len__(self):
        return self.length

    def __eq__(self, other):
        return ojlib.ojs_equal(byref(self), byref(other))

    def __getitem__(self, index):
        return self.cards[index]

    def __setitem__(self, index, val):
        if index >= self.length:
            raise IndexError
        if isinstance(val, str):
            val = ojlib.oj_card_from_text(val.encode())
        self.cards[index] = val

    def __delitem__(self, index):
        ojlib.ojs_delete(byref(self), index)

    def __contains__(self, card):
        if isinstance(card, str):
            card = ojlib.oj_card_from_text(card.encode())
        return (-1 != ojlib.ojs_index(byref(self), card))

    def __iter__(self):
        for i in range(self.length):
            yield self.cards[i]

    def clear(self):
        self.length = 0

    def truncate(self, length):
        if (length <= self.length):
            self.length = length

    def append(self, arg):
        if isinstance(arg, str):
            cards = cast(create_string_buffer(1024), POINTER(c_int))
            count = ojlib.oj_cards_from_text(256, cards, arg.encode())
            for i in range(count):
                ojlib.ojs_append(byref(self), cards[i])
        elif isinstance(arg, Sequence):
            for i in range(arg.length):
                ojlib.ojs_append(byref(self), arg.cards[i])
        else:
            ojlib.ojs_append(byref(self), card)

    def insert(self, index, card):
        if isinstance(card, str):
            card = ojlib.oj_card_from_text(card.encode())
        ojlib.ojs_insert(byref(self), index, card)

    def remove(self, arg):
        if isinstance(arg, str):
            cards = cast(create_string_buffer(1024), POINTER(c_int))
            count = ojlib.oj_cards_from_text(256, cards, arg.encode())
            for i in range(count):
                v = ojlib.ojs_remove(byref(self), cards[i])
            return v
        elif isinstance(arg, Sequence):
            for i in range(arg.length):
                v = ojlib.ojs_remove(byref(self), arg.cards[i])
            return v
        else:
            return ojlib.ojs_remove(byref(self), arg)

    # Slight deviation from the Python model: doesn't take index
    def pop(self):
        return ojlib.ojs_pop(byref(self))

    def index(self, card):
        if isinstance(card, str):
            card = ojlib.oj_card_from_text(card.encode())
        return ojlib.ojs_index(byref(self), card)

    def fill(self, count, type):
        ojlib.ojs_fill(byref(self), count, type)

    def sort(self):
        ojlib.ojs_sort(byref(self))

    def reverse(self):
        ojlib.ojs_sort(byref(self))

    def _combination_generator(self):
        yield self.iter.hand.contents
        while ojlib.ojc_iter_next(byref(self.iter)):
            yield self.iter.hand.contents

    def iter_combinations(self, k):
        self.iter = Iterator(self, k, 0)
        return self._combination_generator()

    def _montecarlo_generator(self):
        yield self.iter.hand.contents
        while ojlib.ojc_iter_next_random(byref(self.iter)):
            yield self.iter.hand.contents

    def iter_montecarlo(self, k, count):
        self.iter = Iterator(self, k, count)
        return self._montecarlo_generator()

_ojc_iter_new = ojlib.ojc_iter_new
_ojc_iter_new.restype = c_longlong
_ojc_iter_new.argtypes = [ c_void_p, c_void_p, c_void_p,
    c_int, POINTER(c_int), c_longlong ]

class Iterator(Structure):
    _fields_ = [
        ("_johnnymoss", c_int),
        ("k", c_int),
        ("a", POINTER(c_int)),
        ("total", c_longlong),
        ("remaining", c_longlong),
        ("deck", POINTER(Sequence)),
        ("hand", POINTER(Sequence))
    ]

    def __init__(self, deck, k, count):
        self.h = Sequence(k)
        buf = cast(create_string_buffer(4 * k), POINTER(c_int))
        _ojc_iter_new(byref(self), byref(deck), byref(self.h), k, buf, count)

def poker_eval(hand):
    return ojlib.ojp_eval(byref(hand))
