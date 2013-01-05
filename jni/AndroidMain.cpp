#include "handroid/Application.h"

#include "bombz/ScreenHub.h"

extern "C" {
void android_main(struct android_app *state);
}

void android_main(struct android_app *state);
{
    handroid::Application application(state);
    bombz::ScreenHub hub(&application);
    // FIXME: Ultimately we should support state saving and set initial
    // screen based on saved state
    application.setScreen((hgame::Screen *) hub.getMainMenuScreen());
    application.start();
}
