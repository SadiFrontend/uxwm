/* uxwm - Ultra-minimal X11 Window Manager */
#include "x11.h"

int main(void)
{
    setup();
    run();
    cleanup();
    return 0;
}
