#!/usr/bin/python

from maitch import *
import mcommon

__already_run = False

def add_envs(envs):
    envs["BIN_DIR"] = "${TOP_DIR}/bin"
    envs["SRC_DIR"] = "${TOP_DIR}"
    envs["HCXXFLAGS"] = "${CXXFLAGS} -I${SRC_DIR} -I${BUILD_DIR} -D_GNU_SOURCE"
    envs["BOMBZ_CXXFLAGS"] = "${HCXXFLAGS} ${SDL_CFLAGS} ${OPENGL_CFLAGS}"
    envs["BOMBZ_LIBS"] = "${SDL_LIBS} ${OPENGL_LIBS}"
    envs["BUILD_SUBDIRS"] = "${BUILD_DIR}/hgame " \
            "${BUILD_DIR}/hgl " \
            "${BUILD_DIR}/hsdl " \
            "${BUILD_DIR}/bombz " \
            "${BUILD_DIR}/tests "

def init(ctx):
    global __already_run
    if __already_run:
        return
    __already_run = True
    
    if ctx.mode == 'configure':
    
        ctx.arg_disable("opengl", "Disable OpenGL rendering " \
                "(for future compatibility - currently OpenGL is compulsory)")
        ctx.arg_disable("shadows", "Disable drop shadows under menu text")
        
        ctx.pkg_config("sdl SDL_image SDL_ttf", "SDL")
        ctx.pkg_config("libpng")
        
        if ctx.env['ENABLE_OPENGL']:
            ctx.pkg_config("gl", "OPENGL")
            ctx.define("ENABLE_OPENGL", 1)
        else:
            raise Exception("OpenGL is currently compulsory")
            ctx.env['OPENGL_CFLAGS'] = ""
            ctx.env['OPENGL_LIBS'] = ""
            ctx.define("ENABLE_OPENGL", 0)
        
        ctx.define_from_var("APPNAME_LOWER")
        ctx.define("WITH_SDL", 1)
        # FIXME: Font should be configurable with default to shipped Bitstream
        ctx.define("WITH_FONT",
                "/usr/share/fonts/truetype/freefont/FreeSans.ttf")
        ctx.define_from_var("ENABLE_SHADOWS")
    
    elif ctx.mode == 'build':
    
        lib_sources = ctx.glob_src("*.cpp", "hgame", False) + \
                ctx.glob_src("*.cpp", "hsdl", False) + \
                ctx.glob_src("*.cpp", "hgl", False)
        bombz_sources = ctx.glob_src("*.cpp", "bombz", False)
        sources = lib_sources + bombz_sources + ["SDLMain.cpp"]
        for c in sources:
            ctx.add_rule(CxxRule(sources = c,
                cxxflags = "${BOMBZ_CXXFLAGS}",
                wdeps = "${BUILD_SUBDIRS}"))
        ctx.add_rule(CxxProgramRule(
            sources = change_suffix(sources, ".cpp", ".o"),
            targets = "${BIN_DIR}/bombz",
            cxxflags = "${BOMBZ_CXXFLAGS}",
            libs = "${BOMBZ_LIBS}",
            wdeps = "${BIN_DIR}"))
        ctx.add_rule(Rule(rule = "mkdir -p ${TGT}",
                targets = "${BIN_DIR} ${BUILD_SUBDIRS}"))
        
        ctx.add_rule(CxxRule(sources = "tests/sdlfont.cpp",
            cxxflags = "${BOMBZ_CXXFLAGS} ${LIBPNG_CFLAGS}",
            wdeps = "${BUILD_SUBDIRS}"))
        ctx.add_rule(CxxProgramRule(
            sources = change_suffix(lib_sources + bombz_sources + \
                    ["tests/sdlfont.cpp"],
                    ".cpp", ".o"),
            targets = "tests/sdlfont",
            cxxflags = "${BOMBZ_CXXFLAGS} ${LIBPNG_CFLAGS}",
            libs = "${BOMBZ_LIBS} ${LIBPNG_LIBS}",
            wdeps = "tests"))
            
    elif ctx.mode == 'clean' or ctx.mode == 'pristine':
    
        ctx.recursively_remove("${BIN_DIR}/share")
        
        
if __name__ == '__main__':
    add_envs(mcommon.envs)
    ctx = Context(**mcommon.envs)
    init(ctx)
    ctx.run()
