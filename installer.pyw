#!/usr/bin/env python3
#
import sys, os, shutil
from collections import OrderedDict

if os.name not in ("nt", "posix"):
    print(
"""
Sorry, this installation program only knows about Linux and Windows.
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

g_installer_dir = os.path.dirname(os.path.abspath(__file__))
def sp(path):
    return os.path.join(g_installer_dir, path)

g_root = Tk()
g_root.title("Choose installation directories")

if "nt" == os.name:
    g_root.wm_iconbitmap(bitmap = sp("oj-icon.ico"))
    Style().theme_use("winnative")
else:
    g_root.wm_iconbitmap(bitmap = "@" + sp("oj-icon.xbm"))
    Style().theme_use("alt")

g_xpad, g_ypad = 16, 16

g_label_fonts = (
    ("Petrona", 12), ("Ubuntu", 12), ("Verdana", 12), ("Arial", 12)
)
g_entry_fonts = (
    ("Source Code Pro", 11), ("Lucida Sans Mono", 11)
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
    g_entry_font = ("Courier", 11)

opj = os.path.join

def systemroot():
    if "SystemRoot" in os.environ:
        sr = os.environ["SystemRoot"]
        return sr[0].lower() + sr[1:]
    else:
        d, p = os.path.splitdrive(os.getcwd())
        return opj("{0}", "Windows").format(d)

def find_library_path():
    if "posix" == os.name:
        return "/usr/local/lib"

    if "nt" == os.name:
        return opj(systemroot(), "System32")

def find_include_path():
    if "posix" == os.name:
        return "/usr/local/include"

    if "nt" == os.name:
        d, p = os.path.splitdrive(os.getcwd())
        p = "{0}\\MinGW".format(d)
        if os.path.exists(p): return opj(p, "include")

        p = "{0}\\Cygwin".format(d)
        if os.path.exists(p): return opj(p, "include")

        p = "{0}\\Program Files\\Microsoft SDKs\\Windows".format(d)
        if os.path.exists(p):
            dl = [f for f in os.listdir(p) if os.path.isdir(opj(p, f))]
            last = sorted(dl)[-1]
            return opj(p, last, "Include")

        return os.path.expanduser("~")

def find_java_path():
    if "JAVA_HOME" in os.environ:
        return opj(os.environ["JAVA_HOME"], "jre", "lib", "ext")

    if sys.platform.startswith("linux"):
        p = "/usr/lib/jvm"
        dl = [f for f in os.listdir(p) if os.path.isdir(opj(p, f))]
        if dl:
            return opj(p, sorted(dl)[-1], "jre/lib/ext")
        return "/usr/java/packages/lib/ext"

    if sys.platform.startswith("solaris"):
        return "/usr/jdk/packages/lib/ext"

    if "nt" == os.name:
        d, p = os.path.splitdrive(os.getcwd())
        p = "{0}\\Program Files\\Java".format(d)
        dl = [f for f in os.listdir(p) if os.path.isdir(opj(p, f))]
        dl2 = [f for f in dl if f.startswith("jdk")]
        if dl2:
            return opj(p, sorted(dl2)[-1], "jre\\lib\\ext")
        elif dl:
            return opj(p, sorted(dl)[-1], "lib\\ext")
        else:
            return opj(systemroot(), "Sun\\Java\\lib\\ext")

def find_python_path():
    if "posix" == os.name:
        major, minor = sys.version_info.major, sys.version_info.minor
        return "/usr/local/lib/python{0}.{1}/site-packages" \
            .format(major, minor)

    if "nt" == os.name:
        p = os.path.dirname(sys.executable)
        return opj(p, "lib\\site-packages")

def make_dest_dirs(path):
    try:
        if not os.path.exists(path):
            os.makedirs(path)
    except:
        print("makedirs {0} failed.").format(path)
        raise
        return False
    return True

def copy_file(name, src, dest):
    try:
        shutil.copy(os.path.join(src, name), dest)
    except:
        print("copy({0}, {1}) failed.".format(os.path.join(src, name), dest))
        raise
        return False
    return True

def install_library(path):
    if not make_dest_dirs(path):
        return False

    if "nt" == os.name:
        f = "onejoker.dll"
        if not copy_file(f, sp("clib"), path):
            return False
        try:
            os.system("regsvr32 {0}".format(os.path.join(path, f)))
        except:
            return False
    else:
        f = "libonejoker.so"
        if not copy_file(f, sp("clib"), path):
            return False

    return True;

def install_include(path):
    if not make_dest_dirs(path):
        return False
    if not copy_file("onejoker.h", sp("clib"), path):
        return False
    return True;

def install_java(path):
    if not make_dest_dirs(path):
        return False
    if not copy_file("onejoker.jar", sp("java"), path):
        return False
    return True;

def install_python(path):
    path = os.path.join(path, "onejoker")
    if not make_dest_dirs(path):
        return False
    for f in ("__init__", "core", "text", "cardlist", "combiner"):
        if not copy_file("{0}.py".format(f), sp("python/onejoker"), path):
            return False
    return True;

g_paths = OrderedDict([
    ("library",     "Library"),
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
        self.iv.trace("w", self.checkbox_callback)
        self.frame = None
        self.check = None
        self.label = None
        self.status = None
        self.entry = None
        self.installed = False;

    def get(self):
        return self.sv.get()

    def set(self, text):
        self.sv.set(text)

    def focus(self):
        self.entry.focus()

    def enabled(self):
        return self.iv.get()

    def disable(self):
        self.entry["state"] = ["disabled"]
        self.check["state"] = ["disabled"]

    def setstatus(self, msg):
        self.status.configure(text = msg)

    def checkbox_callback(self, *args):
        self.entry["state"] = ["!disabled"] \
            if self.iv.get() else ["disabled"]

    def getframe(self, p):
        if self.frame is None:
            f = Frame(p)
            self.check = Checkbutton(f, variable = self.iv)
            self.check.grid(row = 0, column = 0, sticky = "es")
            s = Frame(f, width = "2pt")
            s.grid(row = 0, column = 1)
            self.label = Label(f, text = g_paths[self.name])
            self.label.grid(row = 0, column = 2, sticky = "ws")
            self.status = Label(f, text = "", foreground = "#C03")
            self.status.grid(row = 0, column = 3, sticky = "es")

            s = Frame(f, height = "2pt")
            s.grid(row = 1, column = 0)
            self.entry = Entry(f, width = 50, style = "TEntry",
                textvariable = self.sv, font = g_entry_font)
            self.entry.grid(row = 2, column = 2, sticky = "wn",
                columnspan = 2)
            s = Frame(f, width = g_xpad)
            s.grid(row = 0, column = 4)
            f.columnconfigure(0, weight = 0)
            f.columnconfigure(1, weight = 2)
            f.columnconfigure(2, weight = 3)
            f.columnconfigure(3, weight = 3)
            f.columnconfigure(4, weight = 0)
            self.frame = f
        return self.frame

class App(object):
    def __init__(self):
        self.paths = {}
        for name in g_paths.keys():
            self.paths[name] = PathBox(name)
        self.button1 = None
        self.button2 = None

    def detect_environment(self):
        for p in g_paths.keys():
            f = "find_{0}_path".format(p)
            self.paths[p].set(globals()[f]())

    def cancel(self):
        g_root.quit()

    def install(self):
        anyfail = False
        for name in g_paths.keys():
            p = self.paths[name]
            if p.enabled():
                f = "install_{0}".format(name)
                if globals()[f](p.get()):
                    p.setstatus("OK")
                    p.disable()
                else:
                    p.setstatus("Failed")
                    anyfail = True
        if not anyfail:
            self.button1["state"] = ["disabled"]
            self.button2.configure(text = "OK", command = self.cancel)

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
        self.button1 = Button(f, text = "Cancel", command = self.cancel)
        self.button1.grid(row = 0, column = 0, sticky = "ne")
        s = Frame(f, width = g_xpad)
        s.grid(row = 0, column = 1)
        self.button2 = Button(f, text = "Install", command = self.install)
        self.button2.grid(row = 0, column = 2, sticky = "ne")
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
