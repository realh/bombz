#include "hsdl/Application.h"

#include "bombz/MenuScreen.h"
#include "bombz/ScreenHub.h"

int main(int argc, char **argv)
{
    hsdl::Application application(argc, argv);
    bombz::ScreenHub hub(&application);
    application.setScreen(hub.getMainMenuScreen());
    application.start();
    return 0;
}
