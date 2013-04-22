
import sys, os
from collections import OrderedDict

if os.name not in ("nt", "posix"):
    print(
"""
Sorry, this installation program only knows about Linux and
Windows systems.
""")
    sys.exit(1)

try:
    from tkinter import *
    from tkinter.ttk import *
    import tkinter.font as tfont
except ImportError:
    print(
"""
Your Python 3 installation does not include the "tkinter" module.
This is an optional package on some installations. On Windows, re-run
the Python setup program and choose the "Tcl/Tk" option.
On Ubuntu Linux, you will need to install the "python3-tk" package.
""");
    sys.exit(1)

g_root = Tk()
g_root.title("Choose installation directories")

if "nt" == os.name:
    g_root.wm_iconbitmap("@oj-icon.xbm")
else:
    g_root.wm_iconbitmap("oj-icon.ico")

g_label_fonts = (
    ("Petrona", 12), ("Ubuntu", 12), ("Verdana", 12), ("Arial", 12)
)
g_entry_fonts = (
    ("Source Code Pro", 12), ("Lucida Sans Mono", 12)
)

avail = tfont.families()
g_label_font = None
g_entry_font = None

for f in g_label_fonts:
    if f[0] in avail:
        g_label_font = (f[0], f[1])
        break
if g_label_font is None:
    g_label_font = ("Helvetica", 12)

for f in g_entry_fonts:
    if f[0] in avail:
        g_entry_font = (f[0], f[1])
        break
if g_entry_font is None:
    g_entry_font = ("Courier", 12)

if "nt" == os.name:
    Style().theme_use("winnative")
elif "posix" == os.name:
    Style().theme_use("alt")
else:
    assert False

g_xpad, g_ypad = 16, 16

opj = os.path.join

def find_library_path():
    if "posix" == os.name:
        return "/usr/local/lib"

    if "nt" == os.name:
        if "SystemRoot" in os.environ:
            sr = os.environ["SystemRoot"]
        else:
            d, p = os.path.splitdrive(os.getcwd())
            sr = opj("{0}", "Windows").format(d)
        return opj(sr, "System32")

def find_include_path():
    if "posix" == os.name:
        return "/usr/local/include"

    if "nt" == os.name:
        d, p = os.path.splitdrive(os.getcwd())
        p = opj("{0}", "MinGW").format(d)
        if os.path.exists(p): return opj(p, "include")

        p = opj("{0}", "Cygwin").format(d)
        if os.path.exists(p): return opj(p, "include")

        p = opj("{0}", "Program Files", "Microsoft SDKs", "Windows").format(d)
        if os.path.exists(p):
            dl = [f for f in os.listdir(p) if os.path.isdir(opj(p, f))]
            last = sorted(dl)[-1]
            return opj(p, last, "Include")

        return os.path.expanduser("~")

def find_java_path():
    if "JAVA_HOME" in os.environ:
        return opj(os.environ["JAVA_HOME"], "lib")

    if "posix" == os.name:
        return "/usr/local/jdk"

    if "nt" == os.name:
        d, p = os.path.splitdrive(os.getcwd())
        p = opj("{0}", "Program Files", "Java").format(d)
        dl = [f for f in os.listdir(p) if os.path.isdir(opj(p, f))]
        dl2 = [f for f in dl if f.startswith("jdk")]
        if dl2:
            last = sorted(dl2)[-1]
        else:
            last = sorted(dl)[-1]
        return opj(p, last, "lib")

def find_python_path():
    if "posix" == os.name:
        mj, mn = sys.version_info.major, sys.version_info.minor
        return "/usr/local/lib/python{0}.{1}/site-packages" \
            .format(mj, mn)

    if "nt" == os.name:
        p = os.path.dirname(sys.executable)
        return opj(p, "lib", "site-packages")


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
                textvariable = self.sv, font = g_entry_font)
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
        self.paths["library"].set(find_library_path())
        self.paths["include"].set(find_include_path())
        self.paths["java"].set(find_java_path())
        self.paths["python"].set(find_python_path())

    def cancel(self):
        g_root.quit()

    def install(self):
        if "nt" == os.name:
            cmd = "regsvr32 {0}/{1}".format()
        elif "posix" == os.name:
            pass
        else:
            assert False
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
        s.configure("TLabel", font = g_label_font)
        s.configure("TButton", font = g_label_font)
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
