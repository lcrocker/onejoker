#!/usr/bin/env python3
# Build script for OneJoker library <https://github.com/lcrocker/OneJoker>.
#
import os, sys, argparse, glob

g_clib_objects = {
    "onejoker":         [],
    "prng":             [],
    "sequence":         [],
    "text":             [],
    "deckinfo":         [],
    "combinatorics":    [ "bctable.h" ],
    "poker":            [ "ldctables.h" ],
}
g_clib_tests = {
    "t_random":         [ [], [ "m" ] ],
    "t_sequence":       [ [], [] ],
    "t_iters":          [ [], [ "m" ] ],
    "t_004":            [ [], [] ],
}
g_jni_classes = {
    "Card":             [],
    "CardList":         [],
    "DeckType":         [],
}
g_java_classes = [
    "Card", "DeckType", "CardList", "CardCombinations" , "CardGame",
]
g_java_tests = {
    "Test001":          [],
}

g_root_dir = os.path.dirname(os.path.abspath(__file__))
g_java_home = os.environ.get("JAVA_HOME", "")
g_debug_cflags = "-g -pg -DDEBUG"
g_release_cflags = "-O3 -DNDEBUG"
g_debug_javac_flags = "-g -Werror"
g_release_javac_flags = "-g:none"

class Configuration(object):
    pass
g_config = Configuration()

def error(msg):
    print(msg, file = sys.stderr)
    sys.exit(1)

def rp(*args):
    return os.path.join(g_root_dir, *args)

def cd(dir):
    if g_config.verbose:
        print("cd {0}".format(rp(dir)))
    os.chdir(rp(dir))

def system(cmd):
    if not g_config.quiet:
        print(cmd)
    r = os.system(cmd)
    if r:
        error("\"{0}\" returned error {1}.".format(cmd, r))

def older(target, dependents):
    if not os.path.exists(target):
        return True
    tt = os.path.getmtime(target)
    for d in dependents:
        if os.path.getmtime(d) > tt:
            return True
    return False

def build_c_objects():
    incdirs = [ rp("src/clib"), rp("src/clib/include") ]
    needed = []
    for obj, deps in g_clib_objects.items():
        object = rp("build/clib", "{0}.o".format(obj))
        dependents = [
            rp("src/clib", "{0}.c".format(obj)),
            rp("src/clib/include/onejoker.h")
        ]
        dependents.extend([ rp("src/clib", f) for f in deps ])
        if older(object, dependents):
            needed.append(dependents[0])

    if "" != g_java_home and not g_config.nojava:
        incdirs.append(os.path.join(g_java_home, "include"))
        incdirs.append(rp("src/java"))

        for obj, deps in g_jni_classes.items():
            object = rp("build/clib", "jni{0}.o".format(obj))
            dependents = [
                rp("src/clib/jni", "jni{0}.c".format(obj)),
                rp("src/clib/include/onejoker.h"),
                rp("src/java/", "com_onejoker_onejoker_{0}.h".format(obj))
            ]
            dependents.extend([ rp("src/clib/jni", f) for f in deps ])
            if older(object, dependents):
                needed.append(dependents[0])

    if 0 != len(needed):
        cd("build/clib")
        flags = g_release_cflags if g_config.release else g_debug_cflags
        cmd = "gcc -c -Wall {0} {1} {2}".format(flags,
            " ".join("-I{0}".format(f) for f in incdirs),
            " ".join(needed))
        system(cmd)

def build_c_library():
    build_c_objects()
    if "nt" == os.name:
        libfile = "onejoker.dll"
        ldflags = "-lcrypt32"
    else:
        libfile = "libonejoker.so"
        ldflags = "-nostartfiles"

    objects = [ rp("build/clib", "{0}.o".format(f)) \
        for f in g_clib_objects.keys() ]
    if "" != g_java_home and not g_config.nojava:
        objects.extend([ rp("build/clib", "jni{0}.o".format(f)) \
            for f in g_jni_classes.keys() ])

    if older(rp("build/clib", libfile), objects):
        cd("build/clib")
        cmd = "gcc -shared -o {0} {1} {2}".format(libfile,
            " ".join(objects), ldflags)
        system(cmd)

def build_c_tests():
    build_c_library()
    exepat = "{0}.exe" if "nt" == os.name else "{0}"
    incdirs = [ rp("src/clib/tests"), rp("src/clib/include") ]

    here = False
    for obj, deps in g_clib_tests.items():
        executable = rp("build/clib", exepat.format(obj))
        dependents = [
            rp("src/clib/tests", "{0}.c".format(obj)),
            rp("src/clib/include/onejoker.h")
        ]
        dependents.extend([ rp("src/clib/tests", f) for f in deps[0] ])
        if older(executable, dependents):
            if not here:
                cd("build/clib")
                here = True

            flags = g_release_cflags if g_config.release else g_debug_cflags
            cmd = "gcc {0} -L. {1} -o {2} {3} -lonejoker {4}".format(
                flags,
                " ".join("-I{0}".format(f) for f in incdirs),
                executable, dependents[0],
                " ".join("-l{0}".format(f) for f in deps[1]))
            system(cmd)

def run_c_tests():
    build_c_tests()
    cd("build/clib")
    exepat = "{0}.exe" if "nt" == os.name else "{0}"

    for t in [ rp("build/clib", exepat.format(f)) \
        for f in g_clib_tests.keys() ]:
        system(t)

def build_java_classes():
    needed = []
    for cls in g_java_classes:
        tgt = rp("build/java/com/onejoker/onejoker", "{0}.class".format(cls))
        deps = [ rp("src/java/com/onejoker/onejoker", "{0}.java".format(cls)) ]
        if older(tgt, deps):
            needed.append(deps[0])

    if 0 != len(needed):
        cd("build/java")
        flags = g_release_javac_flags if g_config.release \
            else g_debug_javac_flags
        cmd = "javac {0} -d {1} {2}".format(flags,
            rp("build/java"), " ".join(needed))
        system(cmd)

def build_java_headers():
    build_java_classes()
    needed = []
    for cls in g_java_classes:
        tgt = rp("build/java", "com_onejoker_onejoker_{0}.h".format(cls))
        deps = [ rp("build/java/com/onejoker/onejoker"),
            "{0}.class".format(cls) ]
        if older(tgt, deps):
            needed.append("com.onejoker.onejoker.{0}".format(cls))

    if 0 != len(needed):
        cd("build/java")
        cmd = "javah -jni {0}".format(" ".join(needed))
        system(cmd)

def build_java_tests():
    build_java_classes()
    needed = []

    for cls in g_java_tests:
        tgt = rp("build/java/com/onejoker/tests", "{0}.class".format(cls))
        deps = [ rp("src/java/com/onejoker/tests", "{0}.java".format(cls)) ]
        if older(tgt, deps):
            needed.append(deps[0])

    if 0 != len(needed):
        cd("build/java")
        flags = g_release_javac_flags if g_config.release \
            else g_debug_javac_flags
        cmd = "javac {0} -d {1} {2}".format(flags,
            rp("build/java"), " ".join(needed))
        system(cmd)

def run_java_tests():
    build_c_library()
    build_java_tests()
    cd("build/java")
    libdir = rp("build/clib")
    ea = "" if g_config.release else "-ea"
    for cls in g_java_tests:
        cmd = "java {0} -Djava.library.path={1} com.onejoker.onejoker.{2}" \
            .format(ea, libdir, cls);
        system(cmd)

def build_python():
    pass

def build_python_tests():
    pass

def run_python_tests():
    pass

def build_all_tests():
    build_c_tests()
    build_java_tests()
    build_python_tests()

def run_all_tests():
    run_c_tests()
    run_java_tests()
    run_python_tests()

def remove_files_only(dir, recurse = False):
    cd(dir)
    files = []
    dirs = []
    for f in glob.glob("*"):
        if os.path.isfile(f):
            files.append(f)
        else:
            dirs.append(f)

    if 0 != len(files):
        system("rm -f {0}".format(" ".join(files)))
    if recurse:
        for d in dirs:
            remove_files_only(os.path.join(dir, d), True)

def clean_java():
    remove_files_only("build/java", True)

def clean_python():
    remove_files_only("build/python", True)

def clean_clib():
    remove_files_only("build/clib", True)

def clean_all():
    clean_java()
    clean_python()
    clean_clib()
    remove_files_only("build", False)

target_aliases = {
    "test": "runtests",
    "lib": "library",
    "jni": "javaheaders"
}

target_functions = {
    "library": build_c_library,
    "ctests": build_c_tests,
    "runctests": run_c_tests,
    "javaclasses": build_java_classes,
    "javaheaders": build_java_headers,
    "javatests": build_java_tests,
    "runjavatests": run_java_tests,
    "python": build_python,
    "pytests": build_python_tests,
    "runpytests": run_python_tests,
    "tests": build_all_tests,
    "runtests": run_all_tests,
    "cleanjava": clean_java,
    "cleanpython": clean_python,
    "cleanclib": clean_clib,
    "clean": clean_all,
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
        p.add_argument("-n", "--nojava", action = "store_true",
            help = "remove Java support from library")
        p.add_argument("target", nargs = "*",
            help = "what to build")
        p.parse_args(namespace = g_config)

    def run(self):
        self.parse_command_line()
        if 0 == len(g_config.target):
            return

        cwd = os.getcwd()
        for t in g_config.target:
            self.build_target(t)
        cd(cwd)

if __name__ == "__main__":
    BuildApp().run()
    sys.exit(0)
