#!/usr/bin/python

from maitch import *

SVGS_DIR = "${TOP_DIR}/svgs"
PNGS_DIR = "${TOP_DIR}/pngs"

ctx = Context(PACKAGE = "Bombz",
        SVGS_DIR = SVGS_DIR,
        PNGS_DIR = PNGS_DIR)

if ctx.mode == 'configure':
    
    ctx.find_prog_env("convert")
    
elif ctx.mode == 'build':

    size = 72
    # Create dest directories
    ctx.add_rule(Rule(rule = "mkdir -p ${TGT}",
            targets = ("${PNGS_DIR}/%d/fg" % size,
            "${PNGS_DIR}/%d/tiles" % size)))
    # Dirt PNG from XCF
    ctx.add_rule(Rule(rule = "${CONVERT} ${SRC} ${TGT}",
            targets = "${PNGS_DIR}/%d/tiles/texture.png" % size,
            sources = "${TOP_DIR}/texture/dirt.xcf",
            deps = "${PNGS_DIR}/%d/tiles" % size,
            where = TOP))
    # Floor (not transparent)
    ctx.add_rule(Rule(rule = "${CONVERT} ${SRC} -geometry %dx%d ${TGT}" %
                    (size, size),
            targets = "${PNGS_DIR}/%d/tiles/floor.png" % size,
            sources = "${SVGS_DIR}/floor.svg",
            deps = "${PNGS_DIR}/%d/tiles" % size,
            where = TOP))
    # Foregrounds to be overlaid over floor texture, and droid character
    for name in "bomb1 bomb2 droidup droidleft droiddown droidright " \
            "match picket".split() + \
            ["explo%02d" % n for n in range(1, 12)]:
        ctx.add_rule(Rule(rule = "${CONVERT} -background #0000 " \
                "${SRC} -geometry %dx%d ${TGT}" % (size, size),
                targets = "${PNGS_DIR}/%d/fg/%s.png" % (size, name),
                sources = "${SVGS_DIR}/%s.svg" % name,
                deps = "${PNGS_DIR}/%d/fg" % size,
                where = TOP))
    # Big flash for start of explosion
    ctx.add_rule(Rule(rule = "${CONVERT} -background #0000 " \
            "${SRC} -geometry %dx%d ${TGT}" % (size * 3, size * 3),
            targets = "${PNGS_DIR}/%d/fg/explo00.png" % size,
            sources = "${SVGS_DIR}/explo00.svg",
            deps = "${PNGS_DIR}/%d/fg" % size,
            where = TOP))

elif ctx.mode == 'clean' or ctx.mode == 'pristine':

    ctx.recursively_remove("${PNGS_DIR}")
    ctx.delete(ctx.glob("*.png", "${TOP_DIR}/texture"))
    if ctx.mode == 'pristine':
        ctx.delete("${TOP_DIR}/maitch.pyc")
        ctx.recursively_remove("${TOP_DIR}/build")
        

ctx.run()
