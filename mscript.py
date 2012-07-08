#!/usr/bin/python

from maitch import *

SVGS_DIR = "${TOP_DIR}/svgs"
PNGS_DIR = "${TOP_DIR}/pngs"
TMP_PNGS_DIR = "${TOP_DIR}/tmp_pngs"

ctx = Context(PACKAGE = "Bombz",
        SVGS_DIR = SVGS_DIR,
        PNGS_DIR = PNGS_DIR,
        TMP_PNGS_DIR = TMP_PNGS_DIR)

if ctx.mode == 'configure':
    
    ctx.find_prog_env("convert")
    
elif ctx.mode == 'build':

    ctx.add_rule(Rule(rule = "mkdir -p ${TGT}",
            targets = ("${PNGS_DIR}", "${TMP_PNGS_DIR}")))
    
    size = 72
    ctx.add_rule(Rule(rule = "mkdir -p ${TGT}",
            targets = ("${PNGS_DIR}/%d" % size,
                    "${TMP_PNGS_DIR}/%d" % size),
            deps = "${PNGS_DIR}"))
    for name in "bomb1 bomb2 manup manleft mandown manright " \
            "match picket".split() + \
            ["explo%02d" % n for n in range(1, 12)]:
        ctx.add_rule(Rule(rule = "${CONVERT} -background #0000 " \
                "${SRC} -geometry %dx%d ${TGT}" % (size, size),
                targets = "${PNGS_DIR}/%d/%s.png" % (size, name),
                sources = "${SVGS_DIR}/%s.svg" % name,
                deps = "${PNGS_DIR}/%d" % size,
                where = TOP))

elif ctx.mode == 'clean' or ctx.mode == 'pristine':

    ctx.recursively_remove("${PNGS_DIR}")
    ctx.recursively_remove("${TMP_PNGS_DIR}")
    if ctx.mode == 'pristine':
        ctx.delete("${TOP_DIR}/maitch.pyc")
        ctx.recursively_remove("${TOP_DIR}/build")
        

ctx.run()
