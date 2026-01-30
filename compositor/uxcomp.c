/* uxcomp - Ultra-minimal X11 Compositor */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>
#include <X11/extensions/Xrender.h>

typedef struct {
    Window id;
    XWindowAttributes attr;
    Damage damage;
    Picture picture;
    Picture alpha_picture;
} Win;

static Display *dpy;
static Window root;
static int composite_opcode;
static int damage_error_base;
static int render_event_base;
static Picture root_picture;
static Win *windows = NULL;
static int num_windows = 0;
static int opacity = 0xd0000000;

void die(const char *msg) {
    fprintf(stderr, "uxcomp: %s\n", msg);
    exit(1);
}

int has_extension(const char *name) {
    int event_base, error_base;
    return XQueryExtension(dpy, name, &composite_opcode, &event_base, &error_base);
}

Win *find_win(Window id) {
    for (int i = 0; i < num_windows; i++) {
        if (windows[i].id == id)
            return &windows[i];
    }
    return NULL;
}

Picture create_alpha_picture(void) {
    Pixmap pixmap;
    Picture picture;
    XRenderPictureAttributes pa;
    XRenderPictFormat *format;
    
    pixmap = XCreatePixmap(dpy, root, 1, 1, 8);
    format = XRenderFindStandardFormat(dpy, PictStandardA8);
    pa.repeat = True;
    picture = XRenderCreatePicture(dpy, pixmap, format, CPRepeat, &pa);
    
    XRenderColor c = {0, 0, 0, opacity};
    XRenderFillRectangle(dpy, PictOpSrc, picture, &c, 0, 0, 1, 1);
    XFreePixmap(dpy, pixmap);
    
    return picture;
}

void add_win(Window id) {
    Win w;
    XRenderPictFormat *format;
    XRenderPictureAttributes pa;
    
    w.id = id;
    if (!XGetWindowAttributes(dpy, id, &w.attr))
        return;
    
    if (w.attr.class == InputOnly)
        return;
    
    w.damage = XDamageCreate(dpy, id, XDamageReportNonEmpty);
    
    format = XRenderFindVisualFormat(dpy, w.attr.visual);
    pa.subwindow_mode = IncludeInferiors;
    w.picture = XRenderCreatePicture(dpy, id, format, CPSubwindowMode, &pa);
    
    w.alpha_picture = create_alpha_picture();
    
    windows = realloc(windows, sizeof(Win) * (num_windows + 1));
    windows[num_windows++] = w;
}

void remove_win(Window id) {
    Win *w = find_win(id);
    if (!w) return;
    
    XDamageDestroy(dpy, w->damage);
    XRenderFreePicture(dpy, w->picture);
    XRenderFreePicture(dpy, w->alpha_picture);
    
    int idx = w - windows;
    memmove(&windows[idx], &windows[idx + 1], 
            sizeof(Win) * (num_windows - idx - 1));
    num_windows--;
}

void paint_all(void) {
    XRenderColor c = {0x2222, 0x2222, 0x2222, 0xffff};
    XRenderFillRectangle(dpy, PictOpSrc, root_picture, &c,
                         0, 0, DisplayWidth(dpy, 0), DisplayHeight(dpy, 0));
    
    for (int i = 0; i < num_windows; i++) {
        Win *w = &windows[i];
        
        if (w->attr.map_state != IsViewable)
            continue;
        
        XRenderComposite(dpy, PictOpOver, w->picture, w->alpha_picture,
                        root_picture, 0, 0, 0, 0,
                        w->attr.x, w->attr.y, w->attr.width, w->attr.height);
    }
}

void handle_damage(XDamageNotifyEvent *ev) {
    Win *w = find_win(ev->drawable);
    if (!w) return;
    
    XDamageSubtract(dpy, w->damage, None, None);
    paint_all();
}

void scan_windows(void) {
    Window root_return, parent;
    Window *children;
    unsigned int nchildren;
    
    if (XQueryTree(dpy, root, &root_return, &parent, &children, &nchildren)) {
        for (unsigned int i = 0; i < nchildren; i++) {
            add_win(children[i]);
        }
        if (children)
            XFree(children);
    }
}

void setup(void) {
    XRenderPictFormat *format;
    XRenderPictureAttributes pa;
    int composite_event_base, composite_error_base;
    int damage_event_base;
    int render_error_base;
    
    dpy = XOpenDisplay(NULL);
    if (!dpy)
        die("cannot open display");
    
    root = DefaultRootWindow(dpy);
    
    if (!has_extension("Composite"))
        die("Composite extension not available");
    if (!has_extension("DAMAGE"))
        die("DAMAGE extension not available");
    if (!has_extension("RENDER"))
        die("RENDER extension not available");
    
    XCompositeQueryExtension(dpy, &composite_event_base, &composite_error_base);
    XDamageQueryExtension(dpy, &damage_event_base, &damage_error_base);
    XRenderQueryExtension(dpy, &render_event_base, &render_error_base);
    
    XCompositeRedirectSubwindows(dpy, root, CompositeRedirectManual);
    
    format = XRenderFindVisualFormat(dpy, DefaultVisual(dpy, 0));
    pa.subwindow_mode = IncludeInferiors;
    root_picture = XRenderCreatePicture(dpy, root, format, CPSubwindowMode, &pa);
    
    XSelectInput(dpy, root, SubstructureNotifyMask);
    
    scan_windows();
}

void run(void) {
    XEvent ev;
    
    paint_all();
    
    while (1) {
        XNextEvent(dpy, &ev);
        
        switch (ev.type) {
        case CreateNotify:
            add_win(ev.xcreatewindow.window);
            break;
        case DestroyNotify:
            remove_win(ev.xdestroywindow.window);
            paint_all();
            break;
        case MapNotify:
            {
                Win *w = find_win(ev.xmap.window);
                if (w) {
                    w->attr.map_state = IsViewable;
                    paint_all();
                }
            }
            break;
        case UnmapNotify:
            {
                Win *w = find_win(ev.xunmap.window);
                if (w) {
                    w->attr.map_state = IsUnmapped;
                    paint_all();
                }
            }
            break;
        case ConfigureNotify:
            {
                Win *w = find_win(ev.xconfigure.window);
                if (w) {
                    w->attr.x = ev.xconfigure.x;
                    w->attr.y = ev.xconfigure.y;
                    w->attr.width = ev.xconfigure.width;
                    w->attr.height = ev.xconfigure.height;
                    paint_all();
                }
            }
            break;
        default:
            if (ev.type == damage_error_base + XDamageNotify) {
                handle_damage((XDamageNotifyEvent *)&ev);
            }
            break;
        }
    }
}

int main(void) {
    setup();
    run();
    return 0;
}
