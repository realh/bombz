#include "hsdl/Application.h"

#include "bombz/ActivityHub.h"

int main(int argc, char **argv)
{
    hsdl::Application application(argc, argv);
    bombz::ActivityHub hub(&application);
    application.setActivity((hgame::Activity *) hub.getMainMenuScreen());
    application.start();
    return 0;
}
