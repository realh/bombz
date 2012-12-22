#include "hsdl/Application.h"

#include "bombz/ScreenHub.h"

int main(int argc, char **argv)
{
    hsdl::Application application(argc, argv);
    bombz::ScreenHub hub(&application);
    application.setScreen((hgame::Screen *) hub.getMainMenuScreen());
    application.start();
    return 0;
}
