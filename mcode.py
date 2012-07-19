#!/usr/bin/python

from maitch import *
import mcommon

__already_run = False

def add_envs(envs):
    envs["BIN_DIR"] = "${TOP_DIR}/bin"
    envs["SRC_DIR"] = "${TOP_DIR}"
    envs["HCXXFLAGS"] = "${CXXFLAGS} -I ${SRC_DIR}/hgame"

def init(ctx):
    global __already_run
    if __already_run:
        return
    __already_run = True
    
    if ctx.mode == 'configure':
    
        ctx.pkg_config("sdl SDL_image", "SDL")
    
    elif ctx.mode == 'build':
    
        sources = ctx.glob_src("*.cpp", "hgame", False) + ["SDLMain.cpp"]
        mprint("sources for bin:", str(change_suffix(sources, ".cpp", ".o")))
        for c in sources:
            ctx.add_rule(CxxRule(sources = c,
                cxxflags = "${HCXXFLAGS} ${SDL_CFLAGS}",
                wdeps = "${BUILD_DIR}/hgame"))
        ctx.add_rule(CxxProgramRule(
            sources = change_suffix(sources, ".cpp", ".o"),
            targets = "${BIN_DIR}/bombz",
            cxxflags = "${HCXXFLAGS} ${SDL_CFLAGS}",
            libs = "${SDL_LIBS}",
            wdeps = "${BIN_DIR}"))
        ctx.add_rule(Rule(rule = "mkdir -p ${TGT}",
                targets = "${BIN_DIR} ${BUILD_DIR}/hgame"))
            
    elif ctx.mode == 'clean' or ctx.mode == 'pristine':
    
        ctx.recursively_remove("${BIN_DIR}/share")
        
        
if __name__ == '__main__':
    add_envs(mcommon.envs)
    ctx = Context(**mcommon.envs)
    init(ctx)
    ctx.run()
