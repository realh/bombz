#include "hsdl/Application.h"

#include "bombz/ActivityHub.h"

int main(int argc, char **argv)
{
    hsdl::Application application(argc, argv);
    bombz::ActivityHub hub(&application);
    // FIXME: Create an activity and set it to application
    application.start();
    return 0;
}
