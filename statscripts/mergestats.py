#!/usr/bin/python

import os
import sys

N_LEVELS = 42

# Weightings
WT_FAILED = 100         # 1 - (succeeded / started)
WT_MOVES = 1
WT_DETO = 1             # 10 - detonators_left, clamped >= 0
WT_TIME = 1             # - time_left

time_limits = None

def load_time_limits(levels_dir):
    levels_dir = os.path.dirname(os.path.dirname( \
            os.path.abspath(sys.argv[0]))) + "/assets/levels"
    global time_limits
    time_limits = []
    for n in range(N_LEVELS):
        fp = open("%s/%02d" % (levels_dir, n + 1), 'r')
        for m in range(16):
            l = fp.readline()
        time_limits.append(int(l))
        fp.close()


def load_stats(filename):
    fp = open(filename, 'r')
    stats = {}
    for l in stats.readlines():
        k, v = l.split('=', 1)
        if k == "version":
            version = int(v)
        else:
            k, lvl = k.rsplit('_', 1)
            lvl = int(lvl)
            sfl = stats.get(lvl, None)
            if not sfl:
                sfl = {}
                stats[lvl] = sfl
            sfl[k] = float(v)
    fp.close()
    if version == 1:
        global time_limits
        if not time_limits:
            load_time_limits()
        for lvl, sfl in stats.items():
            tt = time_limits[lvl - 1] - sfl["time_left"]
            sfl["time_left"] = time_limits[lvl - 1] * 7 / 4 - tt
    return stats


def stats_to_rating(stats):
    global WT_FAILED, WT_MOVES, WT_DETO, WT_TIME
    ratings = {}
    for lvl, sfl in stats.items():
        try:
            started = sfl["started"]
            succ = sfl["succeeded"]
            moves = sfl["moves"]
            deto = sfl["detonators_left"]
            tm = sfl["time_left"]
        except:
            continue
        deto = 10 - deto
        if deto < 0:
            deto = 0
        ratings[lvl] = WT_FAILED * (1.0 - succ / started) +
                WT_MOVES * moves +
                WT_DETO * deto +
                WT_TIME * -tm
    return ratings


def merge_ratings(all_ratings):
    " all_ratings is a list of ratings "
    totals = {}
    count = {}
    for rats in all_ratings:
        for lvl, val in rats.items():
            totals[lvl] = totals.get(lvl, 0.0) + val
            count[lvl] = count.get(lvl, 0) + 1
    for lvl in totals.keys():
        totals[lvl] /= count[lvl]
    return totals
