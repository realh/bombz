#include "hgame/Log.h"

int main(int argc, char **argv)
{
    hgame::Log log("main");
    log.i("Hello world!");
    return 0;
}
