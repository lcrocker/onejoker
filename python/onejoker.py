#!/usr/bin/env python3
#
# Python language binding for OneJoker library.
# Uses ctypes to access shared lib.
#

import sys, itertools
import cardnames as cn
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
        return "({0})".format(cn.cardnames(
            (self.cards[i] for i in range(self.length)), " "))

    def __eq__(self, other):
        if isinstance(other, Sequence):
            return ojlib.ojs_equal(byref(self), byref(other))
        else:
            if isinstance(other, str):
                other = cn.cardnums(other)
            return all( a == b for a, b in itertools.zip_longest(other,
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

    def __delitem__(self, index):
        v = ojlib.ojs_delete(byref(self), index)

    def __contains__(self, val):
        if isinstance(val, str):
            val = cn.cardnum(val)
        return (-1 != ojlib.ojs_index(byref(self), val))

    def clear(self):
        self.length = 0

    def truncate(self, length):
        if (length <= self.length):
            self.length = length

    def append(self, arg):
        if isinstance(arg, str):
            for c in cn.cardnums(arg):
                ojlib.ojs_append(byref(self), c)
        elif hasattr(arg, "__iter__"):
            for v in arg:
                ojlib.ojs_append(byref(self), v)
        else:
            ojlib.ojs_append(byref(self), arg)

    def insert(self, index, arg):
        if isinstance(arg, str):
            for c in cn.cardnums(arg):
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
            for c in cn.cardnums(arg):
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
            card = cn.cardnum(card)
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
        self.buf = cast(create_string_buffer(4 * k), POINTER(c_int))
        _ojc_iter_new(byref(self), byref(deck), byref(self.h), k,
            self.buf, count)

def poker_eval(hand):
    return ojlib.ojp_eval(byref(hand))
