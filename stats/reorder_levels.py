#!/usr/bin/python

# Takes output of show_order on stdin and reorders levels accordingly

import os, shutil, sys

def load_order(fp):
    order = fp.read().split()
    return [int(l) for l in order]


levels_dir = os.path.join( \
        os.path.dirname(os.path.dirname(os.path.abspath(sys.argv[0]))),
        "assets", "levels")
tmp_dir = levels_dir + "_tmp"


def rename_level(old, new):
    global levels_dir, tmp_dir
    shutil.move(os.path.join(levels_dir, "%02d" % old),
            os.path.join(tmp_dir, "%02d" % new))


def rename_all(order, renamer):
    for n in range(len(order)):
        renamer(n + 1, order[n])


if not os.path.isdir(tmp_dir):
    os.mkdir(tmp_dir)
order = load_order(sys.stdin)
rename_all(order, rename_level)
os.rmdir(levels_dir)
os.rename(tmp_dir, levels_dir)
