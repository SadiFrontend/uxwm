#ifndef LAYOUT_H
#define LAYOUT_H

#include "wm.h"

typedef struct {
    const char *symbol;
    void (*arrange)(Monitor *);
} Layout;

void tile(Monitor *m);
void monocle(Monitor *m);
void floating(Monitor *m);

static const Layout layouts[] = {
    { "[]=", tile },
    { "[M]", monocle },
    { "><>", floating },
};

#endif
