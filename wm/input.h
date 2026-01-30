#ifndef INPUT_H
#define INPUT_H

#include <X11/Xlib.h>

void grabkeys(void);
void handle_keypress(XEvent *e);

#endif
