#!/usr/bin/python

from maitch import *
import atlas


LOCAL_DATA_DIR = "${TOP_DIR}/share/bombz"
LOCAL_PNGS_DIR = "${LOCAL_DATA_DIR}/pngs"

ctx = Context(PACKAGE = "Bombz",
        LOCAL_DATA_DIR = LOCAL_DATA_DIR,
        LOCAL_PNGS_DIR = LOCAL_PNGS_DIR)

if ctx.mode == 'build':

    size = 72
    
    # Create dest directories
    ctx.add_rule(Rule(rule = "mkdir -p ${TGT}",
            targets = "${LOCAL_PNGS_DIR}/%d" % size))
    
    # Tile atlas
    def tile_atlas_rule_func(ctx, env, targets, sources):
        atlas.make_game_tile_atlas(targets[0], sources, size)
    ctx.add_rule(Rule(rule = tile_atlas_rule_func,
            targets = "${LOCAL_PNGS_DIR}/%d/tile_atlas.png" % size,
            sources = ["svgs/floor.svg", "texture/dirt.png"] + \
                    ["svgs/%s.svg" % s \
                            for s in "match picket bomb1 bomb2".split() + \
                            ["explo%02d" % e for e in range(1, 12)] + \
                            ["chrome_%s" % c \
                                    for c in "tl tr bl br horiz vert".split()]],
            wdeps = "${LOCAL_PNGS_DIR}/%d" % size,
            where = TOP))
        
    # Alpha atlas
    def alpha_atlas_rule_func(ctx, env, targets, sources):
        atlas.make_game_alpha_atlas(targets[0], sources, size)
    ctx.add_rule(Rule(rule = alpha_atlas_rule_func,
            targets = "${LOCAL_PNGS_DIR}/%d/alpha_atlas.png" % size,
            sources = ["svgs/%s.svg" % s for s in ["explo00"] + \
                    ["droid%s" % d for d in "left right up down".split()] + \
                    ["bomb1", "bomb2"]],
            wdeps = "${LOCAL_PNGS_DIR}/%d" % size,
            where = TOP))
        
elif ctx.mode == 'clean' or ctx.mode == 'pristine':

    ctx.recursively_remove("${TOP_DIR}/share")
    if ctx.mode == 'pristine':
        for f in ctx.glob("*.pyc", "${TOP_DIR}"):
            ctx.delete(f)
        ctx.recursively_remove("${TOP_DIR}/build")
        

ctx.run()
