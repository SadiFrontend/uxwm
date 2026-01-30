#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include "atoms.h"
#include "wm.h"

Atom wmatom[WMLast];

void initatoms(void)
{
    wmatom[WMProtocols] = XInternAtom(dpy, "WM_PROTOCOLS", False);
    wmatom[WMDelete] = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    wmatom[WMState] = XInternAtom(dpy, "WM_STATE", False);
    wmatom[WMTakeFocus] = XInternAtom(dpy, "WM_TAKE_FOCUS", False);
    wmatom[NetActiveWindow] = XInternAtom(dpy, "_NET_ACTIVE_WINDOW", False);
    wmatom[NetSupported] = XInternAtom(dpy, "_NET_SUPPORTED", False);
    wmatom[NetWMName] = XInternAtom(dpy, "_NET_WM_NAME", False);
    wmatom[NetWMState] = XInternAtom(dpy, "_NET_WM_STATE", False);
    wmatom[NetWMStateFullscreen] = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", False);
    wmatom[NetWMWindowType] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE", False);
    wmatom[NetWMWindowTypeDialog] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DIALOG", False);
    wmatom[NetClientList] = XInternAtom(dpy, "_NET_CLIENT_LIST", False);

    XChangeProperty(dpy, root, wmatom[NetSupported], XA_ATOM, 32,
                    PropModeReplace, (unsigned char *)wmatom, WMLast);
}
