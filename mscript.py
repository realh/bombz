#!/usr/bin/python

from maitch import *

SVGS_DIR = "${TOP_DIR}/svgs"
PNGS_DIR = "${TOP_DIR}/pngs"

ctx = Context(PACKAGE = "Bombz",
        SVGS_DIR = SVGS_DIR,
        PNGS_DIR = PNGS_DIR)

if ctx.mode == 'configure':
    
    ctx.find_prog_env("composite")
    ctx.find_prog_env("convert")
    ctx.find_prog_env("montage")
    
elif ctx.mode == 'build':

    size = 72
    # Create dest directories
    ctx.add_rule(Rule(rule = "mkdir -p ${TGT}",
            targets = ["${PNGS_DIR}/%d/alpha" % size,
            "${PNGS_DIR}/%d/tiles" % size]))
    # Dirt PNG from XCF
    ctx.add_rule(Rule(rule = "${CONVERT} ${SRC} ${TGT}",
            targets = "${PNGS_DIR}/%d/tiles/dirt.png" % size,
            sources = "${TOP_DIR}/texture/dirt.xcf",
            deps = "${PNGS_DIR}/%d/tiles" % size,
            where = TOP))
    # Floor (no composition)
    ctx.add_rule(Rule(rule = "${CONVERT} ${SRC} -geometry %dx%d ${TGT}" %
                    (size, size),
            targets = "${PNGS_DIR}/%d/tiles/floor.png" % size,
            sources = "${SVGS_DIR}/floor.svg",
            deps = "${PNGS_DIR}/%d/tiles" % size,
            where = TOP))
    # Foregrounds composited over floor texture
    for name in "bomb1 bomb2 match picket".split() + \
            ["explo%02d" % n for n in range(1, 12)]:
        ctx.add_rule(Rule(rule = "${COMPOSITE} -background #0000 " \
                "${SRC} -geometry %dx%d ${SVGS_DIR}/floor.svg ${TGT}" %
                (size, size),
                targets = "${PNGS_DIR}/%d/tiles/%s.png" % (size, name),
                sources = "${SVGS_DIR}/%s.svg" % name,
                deps = ["${PNGS_DIR}/%d/tiles" % size, 
                        "${PNGS_DIR}/%d/tiles/floor.png" % size],
                where = TOP))
    # Graphics with alpha
    for name in "bomb1 bomb2 droidup droidleft droiddown droidright".split():
        ctx.add_rule(Rule(rule = "${CONVERT} -background #0000 " \
                "${SRC} -geometry %dx%d ${TGT}" % (size, size),
                targets = "${PNGS_DIR}/%d/alpha/%s.png" % (size, name),
                sources = "${SVGS_DIR}/%s.svg" % name,
                deps = "${PNGS_DIR}/%d/alpha" % size,
                where = TOP))
    # Big flash for start of explosion
    ctx.add_rule(Rule(rule = "${CONVERT} -background #0000 " \
            "${SRC} -geometry %dx%d ${TGT}" % (size * 3, size * 3),
            targets = "${PNGS_DIR}/%d/alpha/explo00.png" % size,
            sources = "${SVGS_DIR}/explo00.svg",
            deps = "${PNGS_DIR}/%d/alpha" % size,
            where = TOP))
    
    # Each chrome piece is made up of 4 quarters. This function pastes them
    # together, where target file has name chromeXX.png where XX == num.
    # s is split into tl, tr, bl, br: "tl"..."vert" indicating which source
    # SVG to use.
    def montage_chrome(num, s):
        tl, tr, bl, br = s.split()
        # Use 2 stages, montage alpha chrome from 4 quarters then 
        # composite it over floor
        ctx.add_rule(Rule(rule = "${MONTAGE} -tile 2x2 -background #0000 " \
                "${SRC} -geometry %dx%d+0+0 ${TGT}" % (size / 2, size / 2),
                targets = "${PNGS_DIR}/%d/alpha/chrome%02d.png" % (size, num),
                sources = ["${SVGS_DIR}/chrome_%s.svg" % x \
                        for x in [tl, tr, bl, br]],
                deps = "${PNGS_DIR}/%d/alpha" % size,
                where = TOP))
        ctx.add_rule(Rule(rule = "${COMPOSITE} -background #0000 " \
                "${SRC} -geometry %dx%d ${SVGS_DIR}/floor.svg ${TGT}" %
                (size, size),
                targets = "${PNGS_DIR}/%d/tiles/chrome%02d.png" % (size, num),
                sources = "${PNGS_DIR}/%d/alpha/chrome%02d.png" % (size, num),
                deps = ["${PNGS_DIR}/%d/tiles" % size, 
                        "${PNGS_DIR}/%d/tiles/floor.png" % size],
                where = TOP))
    
    # Chrome pieces:
    ################
    # 00: tl corner             08: cross
    # 01: horiz straight        09: right end
    # 02: tr corner             10: bottom end
    # 03: vert straight         11: inverted T
    # 04: bl corner             12: T with leg facing left
    # 05: br corner             13: T
    # 06: top end               14: T with leg facing right
    # 07: left end              15: island
    montage_chrome(0, "tl horiz vert tl")
    montage_chrome(1, "horiz horiz horiz horiz")
    montage_chrome(2, "horiz tr tr vert")
    montage_chrome(3, "vert vert vert vert")
    montage_chrome(4, "vert bl bl horiz")
    montage_chrome(5, "br vert horiz br")
    montage_chrome(6, "tl tr vert vert")
    montage_chrome(7, "tl horiz bl horiz")
    montage_chrome(8, "br bl tr tl")
    montage_chrome(9, "horiz tr horiz br")
    montage_chrome(10, "vert vert bl br")
    montage_chrome(11, "br bl horiz horiz")
    montage_chrome(12, "br vert tr vert")
    montage_chrome(13, "horiz horiz tr tl")
    montage_chrome(14, "vert bl vert tl")
    montage_chrome(15, "tl tr bl br")


elif ctx.mode == 'clean' or ctx.mode == 'pristine':

    ctx.recursively_remove("${PNGS_DIR}")
    if ctx.mode == 'pristine':
        ctx.delete("${TOP_DIR}/maitch.pyc")
        ctx.recursively_remove("${TOP_DIR}/build")
        

ctx.run()
