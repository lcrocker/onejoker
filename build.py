#!/usr/bin/env python3
#
# Build script for OneJoker library <https://github.com/lcrocker/OneJoker>.
#
import os, sys, logging, argparse

g_jdk = "/usr/lib/jvm/java-1.7.0-openjdk-i386"
if "JDK" in os.environ:
    g_jdk = os.environ["JDK"]

g_lib_sources = [
    "onejoker.c", "prng.c", "sequence.c", "text.c", "deckinfo.c",
    "combinatorics.c", "poker.c"
]
g_inclusions = {
    "combinatorics.c": [ "bctable.h" ],
    "poker.c": [ "ldctables.h" ]
}
g_tests = [ "t_random", "t_sequence", "t_iters" ]

g_jni_sources = [ "jni.c" ]
g_java_classes = [ "Context", "Sequence", "Iterator", "Game" ]

g_cflags = "-Wall"
g_ldflags = ""

if "nt" == os.name:
    g_libfile = "onejoker.dll"
    g_ldflags = " ".join([g_ldflags, "-lcrypt32"])
else:
    g_libfile = "libonejoker.so"
    g_ldflags = " ".join([g_ldflags, "-nostartfiles"])

g_debug_cflags = " ".join([g_cflags, "-g", "-pg", "-DDEBUG"])
g_release_cflags = " ".join([g_cflags, "-O3", "-DNDEBUG"])
g_debug_ldflags = g_ldflags
g_release_ldflags = g_ldflags

g_javac_flags = "-g"
g_extra_clean = [ "~*", "*.bak", "gmon.out" ]

# Evertything above is stuff that might have to be configured for your system.
# Below here is working code that shouldn't have to change much.

g_root = os.path.dirname(os.path.abspath(__file__))

class Configuration(object):
    pass

g_config = Configuration()

# Logging functions

g_log = logging.getLogger("com.onejoker.build")
h = logging.FileHandler(os.path.join(g_root, "build.log"), "w")
h.setFormatter(logging.Formatter("build: %(asctime)s %(levelname)s: %(message)s"))
g_log.addHandler(h)
g_log.setLevel(logging.INFO)

def debug(msg):
    g_log.debug(msg)
    if g_config.verbose:
        print(msg, file = sys.stderr)

def info(msg):
    g_log.info(msg)
    if g_config.verbose:
        print(msg, file = sys.stderr)

def warn(msg):
    g_log.warn(msg)
    if not g_config.quiet:
        print(msg, file = sys.stderr)

def error(msg):
    g_log.error(msg)
    print(msg, file = sys.stderr)
    sys.exit(1)

def cd(dir = None):
    if dir is None:
        dir = g_root
    else:
        dir = os.path.join(g_root, dir)

    info("cd \"{0}\".".format(dir))
    if not g_config.quiet:
        print("cd {0}".format(dir))
    os.chdir(dir)

def system(cmd):
    info(cmd)
    if not g_config.quiet:
        print(cmd)
    r = os.system(cmd)
    if r:
        error("\"{0}\" returned error {1}.".format(cmd, r))

def clean_library():
    cd("src/lib")
    system("rm -f {0} *.dll *.so *.o release.build java.build".format(
    " ".join(g_extra_clean)))

def clean_tests():
    cd("src/tests")
    if "nt" == os.name:
        system("rm -f {0} *.dll *.exe".format(" ".join(g_extra_clean)))
    else:
        system("rm -f {0} *.so {1}".format(" ".join(g_extra_clean),
        " ".join(g_tests)))

def clean_python():
    for d in ("python/onejoker", "python/scripts", "python/tests", "python"):
        cd(d)
        system("rm -f {0} *.pyc".format(" ".join(g_extra_clean)))
        system("rm -rf __pycache__")

def clean_java():
    cd("java/com/onejoker/onejoker")
    system("rm -f {0} *.class".format(" ".join(g_extra_clean)))
    cd("java")
    system("rm -f {0}".format(" ".join(g_extra_clean)))

def clean_all():
    clean_library()
    clean_tests()
    clean_python()
    clean_java()
    cd()
    system("rm -f {0} build.log".format(" ".join(g_extra_clean)))

def older(target, dependents):
    if not os.path.exists(target):
        return True
    tt = os.path.getmtime(target)
    for d in dependents:
        if os.path.getmtime(d) > tt:
            return True
    return False

def check_library_release_type():
    if os.path.exists("release.build") and not g_config.release:
        system("rm -f release.build *.o *.dll *.so")
    if os.path.exists("java.build") and not g_config.java:
        system("rm -f java.build *.dll *.so")

    flagfiles = []
    if g_config.release:
        flagfiles.append("release.build")
    if g_config.java:
        flagfiles.append("java.build")
    if flagfiles:
        system("touch {0}".format(" ".join(flagfiles)))

def compile_library(sources):
    flags = g_release_cflags if g_config.release else g_debug_cflags

    incdirs = [ ".", os.path.join(g_root, "src", "include") ]
    if g_config.java:
        incdirs.append(os.path.join(g_root, "java"))
        incdirs.append(os.path.join(g_jdk, "include"))

    cmd = "gcc -c {0} {1} {2}".format(flags,
    " ".join(("-I{0}".format(d)) for d in incdirs), " ".join(sources))
    system(cmd)

def link_library(objects):
    flags = g_release_ldflags if g_config.release else g_debug_ldflags
    cmd = "gcc -shared -o {0} {1} {2}".format(g_libfile, " ".join(objects), flags)
    system(cmd)
    info("Built {0}.".format(g_libfile))

def build_library():
    cd("src/lib")
    check_library_release_type()

    sources = list(g_lib_sources)
    if g_config.java:
        javadir = os.path.join(g_root, "java")
        sources.extend((os.path.join(javadir, f) for f in g_jni_sources))

    needed = []
    objects = []
    for sf in sources:
        b, x = os.path.splitext(sf)
        obj = b + ".o"
        objects.append(obj)

        deps = [sf]
        if sf in g_inclusions:
            deps.extend(g_inclusions[sf])
        if older(obj, deps):
            needed.append(sf)

    if 0 != len(needed):
        compile_library(needed)

    if older(g_libfile, objects):
        link_library(objects)

def compile_tests(needed):
    incdirs = [ ".", os.path.join(g_root, "src", "include") ]
    incflags = " ".join("-I{0}".format(f) for f in incdirs)

    flags = g_release_cflags if g_config.release else g_debug_cflags
    flags = " ".join([flags, incflags])

    for src, exe in needed.items():
        cmd = "gcc {0} -L. -o {1} {2} -lonejoker -lm".format(flags, exe, src)
        system(cmd)
        info("Built {0}.".format(exe))

def run_tests():
    build_library()
    cd("src/tests")

    lib = os.path.join(g_root, "src", "lib", g_libfile)
    if older(g_libfile, [lib]):
        system("cp {0} .".format(lib))

    ext = ".exe" if "nt" == os.name else ""
    files = { "{0}.c".format(f): "{0}{1}".format(f, ext) for f in g_tests }

    if os.path.exists("release.build") and not g_config.release:
        system("rm -f release.build {0}".format(" ".join(values(files))))
    if g_config.release:
        system("touch release.build")

    header = os.path.join(g_root, "src", "include", "onejoker.h")
    needed = {}
    for src, exe in files.items():
        if older(exe, [src, header]):
            needed[src] = exe
    compile_tests(needed)
    info("Built {0}.".format(g_libfile))

    for exe in files.values():
        system("./{0}".format(exe))
    info("Tests passed.")

def build_java():
    cd("java")
    jdir = os.path.join(g_root, "java", "com", "onejoker", "onejoker")

    needed = []
    for name in g_java_classes:
        src = os.path.join(jdir, "{0}.java".format(name))
        cls = os.path.join(jdir, "{0}.class".format(name))
        if older(cls, [src]):
            needed.append(src)

    if 0 != len(needed):
        cmd = "javac {0} {1}".format(g_javac_flags, " ".join(needed))
        system(cmd)

    needed = []
    for name in g_java_classes:
        cls = os.path.join(jdir, "{0}.class".format(name))
        hdr = "com_onejoker_onejoker_{0}.h".format(name)
        if older(hdr, [cls]):
            needed.append("com.onejoker.onejoker.{0}".format(name))

    if 0 != len(needed):
        cmd = "javah -jni {0}".format(" ".join(needed))
        system(cmd)

def build_python():
    pass


target_aliases = {
    "test": "tests", "lib": "library", "all": "library",
    "cleantest": "cleantests", "cleanlib": "cleanlibrary", "cleansrc": "cleanlibrary",
    "jni": "java"
}

target_functions = {
    "library": build_library, "tests": run_tests,
    "cleanlibrary": clean_library, "cleanpython": clean_python,
    "cleanjava": clean_java, "cleantests": clean_tests,
    "clean": clean_all, "java": build_java, "python": build_python
}

class BuildApp(object):
    def __init__(self):
        pass

    def build_target(self, target):
        target = target.lower()
        target = target_aliases.get(target, target)
        f = target_functions.get(target, None)

        if f is None:
            error("Unknown target \"{0}\"".format(target))
        else:
            f()

    def parse_command_line(self):
        p = argparse.ArgumentParser(description = "Build OneJoker library",
            epilog = "Common targets: library, test, clean")
        p.add_argument("-v", "--verbose", action = "store_true",
            help = "show extra information")
        p.add_argument("-q", "--quiet", action = "store_true",
            help = "supress normal output")
        p.add_argument("-r", "--release", action = "store_true",
            help = "optimize build for release")
        p.add_argument("-d", "--debug", dest = "release", action = "store_false",
            help = "build for debugging (default)")
        p.add_argument("-j", "--java", action = "store_true",
            help = "add Java support to library")
        p.add_argument("target", nargs = "*",
            help = "what to build")
        p.parse_args(namespace = g_config)

    def run(self):
        self.parse_command_line()
        if 0 == len(g_config.target):
            g_config.target.append("library")

        cwd = os.getcwd()
        for t in g_config.target:
            self.build_target(t)
        cd(cwd)

if __name__ == "__main__":
    BuildApp().run()
    sys.exit(0)
