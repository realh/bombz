#!/usr/bin/python

# Takes output of mergestats.py on stdin

import sys

def get_order(fp):
    ratings = {}
    for ln in fp.readlines():
        lvl, rat = ln.split('=', 1)
        ratings[int(lvl)] = float(rat)
    lvls = ratings.keys()
    lvls.sort(key = lambda l: ratings[l])
    return lvls


ordered = get_order(sys.stdin)
for l in ordered:
    print "%02d" % l,
print
