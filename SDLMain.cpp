#include "hgame/Log.h"
#include "sdl/Platform.h"

int main(int argc, char **argv)
{
    hgame::Log log("main");
    log.i("Hello world!");
    sdl::SDLPlatform platform(argc, argv);
    return 0;
}
