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
            return ""

        text = create_string_buffer(1024)
        o = ojlib.oj_text_from_cards(self.length, self.cards,
            1024, text, " ".encode())
        return "({0})".format(text.value.decode())

    def __len__(self):
        return self.length

    def __getitem__(self, index):
        return self.cards[index]

    def __setitem__(self, index, val):
        if index >= self.length:
            raise IndexError
        if isinstance(val, str):
            val = ojlib.oj_card_from_text(val.encode())
        self.cards[index] = val

    def __delitem__(self, index):
        pass

    def __contains__(self, card):
        pass

    def __iter__(self):
        pass

    def append(self, card):
        if isinstance(card, str):
            card = ojlib.oj_card_from_text(card.encode())
        ojlib.ojs_deal_to(byref(self), card)

    def extend(self, seq):
        pass

    def insert(self, index, val):
        pass

    def remove(self, val):
        pass

    def pop(self, index):
        pass

    def index(self, val):
        pass

    def sort(self):
        pass

    def reverse(self):
        pass

