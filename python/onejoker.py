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

class DeckInfo(Structure):
    _fields_ = [
        ("size", c_int),
        ("cards", POINTER(c_int))
        ]
    def __init__(self):
        throw(NotImplementedException)

def get_deck_info(type):
    return cast(ojlib.oj_deck_info, POINTER(DeckInfo))[type]

cs_ascii2 = 0
cs_ascii3 = 1
cs_unicode2 = 2
cs_unicode3 = 3
cs_fulltext = 4
cs_html = 5
cs_htmlcolor = 6

_oj_card_from_text = ojlib.oj_card_from_text
_oj_card_from_text.argtypes = [c_char_p, POINTER(c_char_p)]
_oj_card_from_text.restype = c_int

def card_from_text(text):
    return _oj_card_from_text(c_char_p(text.encode()), None)

_oj_text_of_card = ojlib.oj_text_of_card
_oj_text_of_card.restype = c_char_p

def text_of_card(card):
    return (ojlib.oj_text_of_card(card, 0)).decode()

_oj_cards_from_text = ojlib.oj_cards_from_text
_oj_cards_from_text.argtypes = [c_int, POINTER(c_int), c_char_p]

def cards_from_text(text):
    buf = create_string_buffer(1024)
    added = _oj_cards_from_text(256, cast(buf, POINTER(c_int)), text.encode())
    return added, cast(buf, POINTER(c_int))

def seed(val):
    return ojlib.ojr_seed(val)

_ojr_next16 = ojlib.ojr_next16
_ojr_next16.restype = c_ushort

def next16():
    return _ojr_next16()

_ojr_next32 = ojlib.ojr_next32
_ojr_next32.restype = c_uint

def next32():
    return _ojr_next32()

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
        if not isinstance(arg, str):
            size = arg
            b = create_string_buffer(4 * size)
            ojlib.ojs_new(byref(self), size, cast(b, POINTER(c_int)))
        else:
            (size, buf) = cards_from_text(arg)
            b = create_string_buffer(4 * size)
            ojlib.ojs_new(byref(self), size, cast(b, POINTER(c_int)))
            memmove(b, buf, 4 * size)
            self.length = size

    def clear(self):
        ojlib.ojs_clear(byref(self))

    def deal_from(self):
        return ojlib.ojs_deal_from(byref(self))

    def deal_to(self, card):
        if not isinstance(card, str):
            return ojlib.ojs_deal_to(byref(self), card)
        (size, buf) = cards_from_text(card)
        b = create_string_buffer(4 * size)
        for i in range(size):
            ojlib.ojs_deal_to(byref(self), buf[i])

    def deal_from_head(self):
        return ojlib.ojs_deal_from_head(byref(self))

    def deal_to_head(self, card):
        if not isinstance(card, str):
            return ojlib.ojs_deal_to_head(byref(self), card)
        (size, buf) = cards_from_text(card)
        b = create_string_buffer(4 * size)
        for i in range(size):
            ojlib.ojs_deal_to_head(byref(self), buf[i])

    def pick(self, card):
        if isinstance(card, str):
            card = card_from_text(card)
        return ojlib.ojs_pick(byref(self), card)

    def move(self, src, count):
        return ojlib.ojs_move(byref(self), byref(src), count)

    def copy (self, src, count):
        return ojlib.ojs_copy(byref(self), byref(src), count)

    def copy_all(self, src):
        return ojlib.ojs_copy_all(byref(self), byref(src))

    def fill(self, count):
        ojlib.ojs_fill(byref(self), count, 0)

    def sort(self):
        ojlib.ojs_sort(byref(self))

    def shuffle(self):
        ojlib.ojs_shuffle(byref(self))

    def reverse(self):
        ojlib.ojs_reverse(byref(self))

    def equal(self, other):
        return ojlib.ojs_equal(byref(self), byref(other))

    def text(self):
        b = create_string_buffer(1024)
        c = ojlib.ojs_text(byref(self), byref(b), 1024, c_char(b" "))
        return b.value.decode()

def main():
    di = get_deck_info(dt_stripped32)
    for i in range(di.size):
        print(" {0}".format(di.cards[i]), end="")
    print()

    print(card_from_text("2c"), card_from_text(" 3H "),
        card_from_text(" 10d"), card_from_text("A S"))

    print(text_of_card(1), text_of_card(7),
        text_of_card(34), text_of_card(52))

    seed(54);
    print(next16(), next16(), next32(), next32())
    print(rand(20), rand(30), rand(40), rand(50))

    s = Sequence("3c As 9s 10h 5d")
    print(s.text())
    s = Sequence(10)
    s.deal_to(15)
    print(s.text())
    s.deal_to("Kh")
    print(s.text())
    s.deal_to("3c As 9s 10h 5d")
    print(s.text())

    return 0

if __name__ == "__main__":
    sys.exit(main())
