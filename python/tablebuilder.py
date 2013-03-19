#!/usr/bin/env python3
#
# tableprinter.py
#
# Utility to help build lookup tables for C code.
#

class TableBuilder(object):
    def __init__(self, name, type = "int", format = "{0}", width = 78):
        self.name = name
        self.type = type
        self.format = format
        self.width = width
        self.first_item = 1
        self.column = 0

    def output(self, val):
        text = self.format.format(val)
        if self.column + len(text) < self.width:
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

    def dump1d(self, source):
        count = 0
        print("{0} {1}[] = {{\n    ".format(self.type, self.name), end="");
        self.column = 4

        for val in source:
            self.output(val)
            count += 1
        print("\n}}; /* {0} Entries */\n".format(count));

    def dump2d(self, width, source):
        pass
