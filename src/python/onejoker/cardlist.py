# OneJoker library <https://github.com/lcrocker/OneJoker>

import sys, itertools as it
from ctypes import *
from ctypes.util import find_library
ojlib = CDLL(find_library("onejoker"))
from . import text

class CardList(Structure):
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
        return "({0})".format(text.cardnames(
            (self.cards[i] for i in range(self.length)), " "))

    def __eq__(self, other):
        if isinstance(other, CardList):
            return ojlib.ojs_equal(byref(self), byref(other))
        else:
            if isinstance(other, str):
                other = text.cardnums(other)
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
            val = text.cardnum(val)
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
            for c in text.cardnums(arg):
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
            for c in text.cardnums(arg):
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
            for c in text.cardnums(arg):
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
            card = text.cardnum(card)
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
