/* uxmenu - Ultra-minimal dmenu alternative */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#define MAX_ITEMS 1024
#define MAX_INPUT 256
#define FONT "-*-fixed-medium-r-*-*-15-*-*-*-*-*-*-*"

typedef struct Item {
    char *text;
    struct Item *next;
} Item;

static Display *dpy;
static Window win, root;
static GC gc;
static XFontStruct *font;
static int screen;
static int sw, sh, bh;
static unsigned long fg, bg, selfg, selbg;

static Item *items = NULL;
static Item *matches = NULL;
static Item *current = NULL;
static char input[MAX_INPUT] = "";
static int inputlen = 0;

void die(const char *msg) {
    fprintf(stderr, "uxmenu: %s\n", msg);
    exit(1);
}

Item *item_new(const char *text) {
    Item *item = malloc(sizeof(Item));
    item->text = strdup(text);
    item->next = NULL;
    return item;
}

void read_items(void) {
    char buf[1024];
    Item *tail = NULL;
    
    while (fgets(buf, sizeof(buf), stdin)) {
        buf[strcspn(buf, "\n")] = 0;
        if (!buf[0]) continue;
        
        Item *item = item_new(buf);
        if (!items) {
            items = item;
        } else {
            tail->next = item;
        }
        tail = item;
    }
}

void filter_items(void) {
    matches = NULL;
    Item *tail = NULL;
    
    for (Item *it = items; it; it = it->next) {
        if (inputlen == 0 || strstr(it->text, input)) {
            Item *match = item_new(it->text);
            if (!matches) {
                matches = match;
                current = match;
            } else {
                tail->next = match;
            }
            tail = match;
        }
    }
}

void draw(void) {
    int y = 0;
    
    XSetForeground(dpy, gc, bg);
    XFillRectangle(dpy, win, gc, 0, 0, sw, bh);
    
    XSetForeground(dpy, gc, fg);
    char prompt[MAX_INPUT + 10];
    snprintf(prompt, sizeof(prompt), "> %s", input);
    XDrawString(dpy, win, gc, 5, font->ascent + 2, prompt, strlen(prompt));
    
    int cursor_x = 5 + XTextWidth(font, prompt, strlen(prompt));
    XDrawLine(dpy, win, gc, cursor_x, 2, cursor_x, bh - 2);
    
    y = bh + 2;
    int i = 0;
    for (Item *it = matches; it && i < 10; it = it->next, i++) {
        if (it == current) {
            XSetForeground(dpy, gc, selbg);
            XFillRectangle(dpy, win, gc, 0, y, sw, bh);
            XSetForeground(dpy, gc, selfg);
        } else {
            XSetForeground(dpy, gc, fg);
        }
        XDrawString(dpy, win, gc, 5, y + font->ascent + 2, 
                    it->text, strlen(it->text));
        y += bh;
    }
    
    XFlush(dpy);
}

void select_next(void) {
    if (!current || !current->next)
        current = matches;
    else
        current = current->next;
    draw();
}

void select_prev(void) {
    if (!matches) return;
    
    if (!current || current == matches) {
        current = matches;
        while (current->next)
            current = current->next;
    } else {
        Item *it = matches;
        while (it->next != current)
            it = it->next;
        current = it;
    }
    draw();
}

void handle_key(XKeyEvent *ev) {
    char buf[32];
    KeySym ksym;
    int len;
    
    len = XLookupString(ev, buf, sizeof(buf), &ksym, NULL);
    
    switch (ksym) {
    case XK_Return:
        if (current) {
            printf("%s\n", current->text);
            exit(0);
        }
        break;
    case XK_Escape:
        exit(1);
        break;
    case XK_BackSpace:
        if (inputlen > 0) {
            input[--inputlen] = 0;
            filter_items();
            draw();
        }
        break;
    case XK_Up:
    case XK_ISO_Left_Tab:
        select_prev();
        break;
    case XK_Down:
    case XK_Tab:
        select_next();
        break;
    default:
        if (len > 0 && inputlen < MAX_INPUT - 1) {
            for (int i = 0; i < len; i++) {
                if (buf[i] >= 32 && buf[i] < 127) {
                    input[inputlen++] = buf[i];
                    input[inputlen] = 0;
                }
            }
            filter_items();
            draw();
        }
        break;
    }
}

void setup(void) {
    XSetWindowAttributes wa;
    XGCValues gcv;
    
    dpy = XOpenDisplay(NULL);
    if (!dpy)
        die("cannot open display");
    
    screen = DefaultScreen(dpy);
    root = RootWindow(dpy, screen);
    sw = DisplayWidth(dpy, screen);
    sh = DisplayHeight(dpy, screen);
    
    font = XLoadQueryFont(dpy, FONT);
    if (!font)
        font = XLoadQueryFont(dpy, "fixed");
    if (!font)
        die("cannot load font");
    
    bh = font->ascent + font->descent + 4;
    
    fg = BlackPixel(dpy, screen);
    bg = WhitePixel(dpy, screen);
    XColor color;
    Colormap cmap = DefaultColormap(dpy, screen);
    if (XAllocNamedColor(dpy, cmap, "#eeeeee", &color, &color))
        selfg = color.pixel;
    else
        selfg = fg;
    if (XAllocNamedColor(dpy, cmap, "#005577", &color, &color))
        selbg = color.pixel;
    else
        selbg = bg;
    
    wa.override_redirect = True;
    wa.background_pixel = bg;
    wa.event_mask = ExposureMask | KeyPressMask;
    
    win = XCreateWindow(dpy, root, 0, 0, sw, bh * 11, 0,
                        CopyFromParent, InputOutput, CopyFromParent,
                        CWOverrideRedirect | CWBackPixel | CWEventMask, &wa);
    
    gcv.foreground = fg;
    gcv.background = bg;
    gcv.font = font->fid;
    gc = XCreateGC(dpy, win, GCForeground | GCBackground | GCFont, &gcv);
    
    XMapRaised(dpy, win);
    XSetInputFocus(dpy, win, RevertToParent, CurrentTime);
}

void run(void) {
    XEvent ev;
    
    while (!XNextEvent(dpy, &ev)) {
        switch (ev.type) {
        case Expose:
            if (ev.xexpose.count == 0)
                draw();
            break;
        case KeyPress:
            handle_key(&ev.xkey);
            break;
        }
    }
}

int main(void) {
    read_items();
    setup();
    filter_items();
    draw();
    run();
    return 0;
}
