#!/usr/bin/env python3

import sys, os

try:
    from tkinter import *
    from tkinter import ttk
except ImportError:
    print(
"""Your Python 3 installation is incomplete, and does not include the
"tkinter" module. If you are running Ubuntu Linux, you need to install
the "python3-tk" package to fix this. On other systems, you may need to
install a different Python 3 implementation.
""");
    sys.exit(1)

class App(object):
    def __init__(self):
        pass

    def cancel(self):
        self.root.quit()

    def detect_environment(self):
        pass

    def install(self):
        print("Installed!")
        self.root.quit()

    def build_window(self):
        self.root.title("OneJoker Installer")
        f = ttk.Frame(self.root, padding = "8 8 12 12")
        f.grid(column = 0, row = 0, sticky = (N, W, E, S))
        f.columnconfigure(0, weight = 1)
        f.rowconfigure(0, weight = 1)

        e1 = ttk.Entry(f, width = 40, textvariable = self.lpath)
        e1.grid(column = 2, row = 1, sticky = (W, E))

        ttk.Label(f, text = "Shared library:").grid(
            column = 2, row = 2, sticky = (W, E))
        ttk.Button(f, text = "Cancel", command = self.cancel).grid(
            column = 2, row = 3, sticky = W)
        ttk.Button(f, text = "Install", command = self.install).grid(
            column = 3, row = 3, sticky = W)

        e1.focus()
        self.root.bind("<Return>", self.install)

    def run_gui(self):
        self.root = Tk()
        self.root.minsize(width = 400, height = 300)
        self.root.resizable(height = True, width = True)

        self.lpath = StringVar()
        self.ipath = StringVar()
        self.jpath = StringVar()
        self.ppath = StringVar()

        self.build_window()
        self.root.mainloop()

    def run(self):
        self.detect_environment()
        self.run_gui()

if __name__ == "__main__":
    App().run()
    sys.exit(0)
