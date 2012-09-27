#include "hsdl/Application.h"

#include "bombz/Activity.h"

int main(int argc, char **argv)
{
    bombz::Activity activity;
    hsdl::Application application(argc, argv, &activity);
    application.start();
    return 0;
}
