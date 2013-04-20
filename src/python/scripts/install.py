#!/usr/bin/env python3

import sys, os
from collections import OrderedDict

try:
    from tkinter import *
    from tkinter.ttk import *
except ImportError:
    print(
"""
Your Python 3 installation does not include the standard
"tkinter" module. This is an optional package on some installations.
On Windows, re-run the Python setup program and choose this option.
On Ubuntu Linux, you need to install the "python3-tk" package.
""");
    sys.exit(1)

g_root = Tk()
g_root.title("Choose installation directories")
Style().theme_use("alt")
g_xpad, g_ypad = 16, 16

g_paths = OrderedDict([
    ("library",     "Shared library"),
    ("include",     "C header files"),
    ("java",        "Java classes"),
    ("python",      "Python module"),
])

class PathBox(object):
    def __init__(self, name):
        self.name = name
        self.sv = StringVar()
        self.iv = IntVar()
        self.iv.set(1)
        self.iv.trace("w", self.callback)
        self.frame = None
        self.check = None
        self.label = None
        self.entry = None

    def set(self, text):
        self.sv.set(text)

    def focus(self):
        self.entry.focus()

    def callback(self, *args):
        self.entry["state"] = ["!disabled"] \
            if self.iv.get() else ["disabled"]

    def getframe(self, p):
        if self.frame is None:
            f = Frame(p)
            self.check = Checkbutton(f, variable = self.iv)
            self.check.grid(row = 0, column = 0, sticky = "es")
            s = Frame(f, width = "2pt")
            s.grid(row = 0, column = 1)
            self.label = Label(f, style = "TLabel",
                text = g_paths[self.name])
            self.label.grid(row = 0, column = 2, sticky = "ws")
            s = Frame(f, height = "2pt")
            s.grid(row = 1, column = 0)
            self.entry = Entry(f, width = 40, style = "TEntry",
                textvariable = self.sv, font = "Courier 12")
            self.entry.grid(row = 2, column = 2, sticky = "wn")
            s = Frame(f, width = g_xpad)
            s.grid(row = 0, column = 3)
            f.columnconfigure(0, weight = 0)
            f.columnconfigure(1, weight = 2)
            f.columnconfigure(2, weight = 3)
            self.frame = f
        return self.frame

class App(object):
    def __init__(self):
        self.paths = {}
        for name in g_paths.keys():
            self.paths[name] = PathBox(name)

    def detect_environment(self):
        self.paths["library"].set("/usr/local/lib")
        self.paths["include"].set("/usr/local/include")
        self.paths["java"].set("/usr/local/jdk")
        self.paths["python"].set("/usr/local/lib/python3.2")

    def cancel(self):
        g_root.quit()

    def install(self):
        print("Installed!")
        g_root.quit()

    def entrybox(self, p):
        f = Frame(p, padding = 0)
        for i, name in enumerate(g_paths):
            if 0 != i:
                s = Frame(f, height = g_ypad)
                s.grid(row = 2 * i - 1, column = 0)
            pf = self.paths[name].getframe(f)
            pf.grid(row = 2 * i, column = 0, sticky = "nw")
        return f

    def buttonbox(self, p):
        f = Frame(p, padding = 0)
        b = Button(f, text = "Cancel", command = self.cancel)
        b.grid(row = 0, column = 0, sticky = "ne")
        s = Frame(f, width = g_xpad)
        s.grid(row = 0, column = 1)
        b = Button(f, text = "Install", command = self.install)
        b.grid(row = 0, column = 2, sticky = "ne")
        return f

    def build_window(self):
        f = Frame(g_root, padding = 10)
        f.grid(column = 0, row = 0, sticky = "wnes")
        f.columnconfigure(0, weight = 1)
        f.rowconfigure(0, weight = 1)

        # l1 = Label(f, style = "TLabel",
        #     text = "Choose installation directories")
        # l1.grid(row = 0, column = 0)
        eb = self.entrybox(f)
        eb.grid(row = 1, column = 0)
        s = Frame(f, height = 2 * g_ypad)
        s.grid(row = 2, column = 0)
        bb = self.buttonbox(f)
        bb.grid(row = 3, column = 0)

        s = Style()
        s.configure("TEntry", padding = 2)
        s.configure("TLabel", font = "Helvetica 12")
        s.configure("TButton", font = "Helvetica 12")
        self.paths["library"].focus()

    def run_gui(self):
        self.build_window()
        g_root.bind("<Return>", self.install)
        g_root.mainloop()

    def run(self):
        self.detect_environment()
        self.run_gui()

if __name__ == "__main__":
    App().run()
    sys.exit(0)
