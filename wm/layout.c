#include <X11/Xlib.h>
#include "wm.h"
#include "layout.h"
#include "config.h"

void tile(Monitor *m)
{
    Client *c;
    int n = 0, i = 0;
    int mw, my, ty;

    for (c = m->clients; c; c = c->next)
        if (!c->isfloating && c->isvisible)
            n++;

    if (n == 0)
        return;

    if (n > m->nmaster)
        mw = m->nmaster ? m->ww * m->mfact : 0;
    else
        mw = m->ww - (enablegaps ? 2 * gappx : 0);

    my = ty = m->wy + (enablegaps ? gappx : 0);

    for (c = m->clients; c; c = c->next) {
        if (!c->isvisible || c->isfloating)
            continue;

        if (i < m->nmaster) {
            int h = (m->wh - my - (enablegaps ? gappx : 0)) / (MIN(n, m->nmaster) - i);
            resize(c, m->wx + (enablegaps ? gappx : 0), my, 
                   mw - 2 * borderpx - (enablegaps ? 2 * gappx : 0), 
                   h - 2 * borderpx - (enablegaps ? gappx : 0), 0);
            my += h;
        } else {
            int h = (m->wh - ty - (enablegaps ? gappx : 0)) / (n - i);
            resize(c, m->wx + mw + (enablegaps ? gappx : 0), ty, 
                   m->ww - mw - 2 * borderpx - (enablegaps ? 2 * gappx : 0),
                   h - 2 * borderpx - (enablegaps ? gappx : 0), 0);
            ty += h;
        }
        i++;
    }
}

void monocle(Monitor *m)
{
    Client *c;
    int n = 0;

    for (c = m->clients; c; c = c->next)
        if (!c->isfloating && c->isvisible)
            n++;

    if (n > 0) {
        for (c = m->clients; c; c = c->next) {
            if (!c->isvisible || c->isfloating)
                continue;
            resize(c, m->wx, m->wy, m->ww - 2 * borderpx, m->wh - 2 * borderpx, 0);
        }
    }
}

void floating(Monitor *m)
{
    Client *c;
    for (c = m->clients; c; c = c->next) {
        if (!c->isvisible)
            continue;
        if (!c->isfloating)
            resize(c, c->x, c->y, c->w, c->h, 0);
    }
}
