#ifndef ATOMS_H
#define ATOMS_H

#include <X11/Xlib.h>

enum {
    WMProtocols,
    WMDelete,
    WMState,
    WMTakeFocus,
    NetActiveWindow,
    NetSupported,
    NetWMName,
    NetWMState,
    NetWMStateFullscreen,
    NetWMWindowType,
    NetWMWindowTypeDialog,
    NetClientList,
    WMLast
};

extern Atom wmatom[WMLast];

void initatoms(void);

#endif
