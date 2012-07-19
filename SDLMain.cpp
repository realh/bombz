#include "hgame/Log.h"

int main(int argc, char **argv)
{
    hgame::Log *log = new hgame::Log("main");
    HGAME_I(log, "Hello world!");
    return 0;
}
