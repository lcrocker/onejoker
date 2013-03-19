#!/usr/bin/env python3
#
# lookuptables.py
#
# Utility to help build lookup tables for C code.
#

class Printer(object):
    def __init__(self, name, type = "int", format = "{0}", width = 78):
        self.name = name
        self.type = type
        self.format = format
        self.text_width = width

        self.first_item = 1
        self.column = 0

        self.min = (2**64 + 1)
        self.max = -(2**64 + 1)

    def output1d(self, val):
        text = self.format.format(val)
        if self.column + len(text) < self.text_width:
            if not self.first_item:
                print(", ", end="")
                self.column += 2
        else:
            if not self.first_item:
                print(",\n    ", end="")
                self.column = 4
        print(text, end="")

        self.column += len(text)
        self.first_item = 0
        self.count += 1

    def dump1d(self, source):
        self.count = 0
        print("{0} {1}[] = {{\n    ".format(self.type, self.name), end="");
        self.column = 4

        for val in source:
            self.output1d(val)
            if val < self.min:
                self.min = val
            if val > self.max:
                self.max = val

        print("\n}}; /* {0} Entries, Range {1}..{2} */\n".format(
            self.count, self.min, self.max));

    def output2d(self, val):
        text = self.format.format(val)
        if not self.first_item and 0 == self.row_count:
            print(" },\n  { ", end="")
            self.column = 4

        if self.column + len(text) < self.text_width:
            if not self.first_item and not 0 == self.row_count:
                print(", ", end="")
                self.column += 2
        else:
            if not self.first_item:
                print(",\n    ", end="")
                self.column = 4
        print(text, end="")

        self.row_count += 1
        if self.row_count == self.array_width:
            self.row_count = 0
            self.rows += 1

        self.column += len(text)
        self.first_item = 0
        self.count += 1

    def dump2d(self, width, source):
        self.count = 0
        self.rows = 0
        self.row_count = 0
        self.array_width = width

        print("{0} {1}[][{2}] = {{\n  {{ ".format(self.type, self.name, width), end="");
        self.column = 4

        for val in source:
            self.output2d(val)
            if val < self.min:
                self.min = val
            if val > self.max:
                self.max = val

        print(" }}\n}}; /* {0} Rows, {1} Entries, Range {2}..{3} */\n".format(
            self.rows, self.count, self.min, self.max));
