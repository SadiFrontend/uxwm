/* uxterm - Ultra-minimal X11 Terminal Emulator */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <pty.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#define TERM_NAME "uxterm"
#define SHELL "/bin/bash"
#define FONT "-*-fixed-medium-r-*-*-15-*-*-*-*-*-*-*"

typedef struct {
    Display *dpy;
    Window win;
    GC gc;
    XFontStruct *font;
    int w, h;
    int cw, ch;
    int master;
    pid_t child;
    char **buffer;
    int rows, cols;
    int crow, ccol;
    unsigned long fg, bg;
} Term;

static Term term;

void die(const char *msg) {
    fprintf(stderr, "uxterm: %s\n", msg);
    exit(1);
}

void sigchld(int sig) {
    (void)sig;
    int status;
    if (waitpid(term.child, &status, WNOHANG) > 0)
        exit(0);
}

void term_init(void) {
    term.rows = 24;
    term.cols = 80;
    term.crow = 0;
    term.ccol = 0;
    
    term.buffer = malloc(term.rows * sizeof(char *));
    for (int i = 0; i < term.rows; i++) {
        term.buffer[i] = calloc(term.cols + 1, 1);
        memset(term.buffer[i], ' ', term.cols);
    }
}

void term_clear_line(int row) {
    memset(term.buffer[row], ' ', term.cols);
}

void term_scroll(void) {
    char *tmp = term.buffer[0];
    memmove(term.buffer, term.buffer + 1, (term.rows - 1) * sizeof(char *));
    term.buffer[term.rows - 1] = tmp;
    term_clear_line(term.rows - 1);
}

void term_putc(char c) {
    if (c == '\r') {
        term.ccol = 0;
    } else if (c == '\n') {
        term.ccol = 0;
        term.crow++;
        if (term.crow >= term.rows) {
            term.crow = term.rows - 1;
            term_scroll();
        }
    } else if (c == '\b') {
        if (term.ccol > 0)
            term.ccol--;
    } else if (c >= 32 && c < 127) {
        if (term.ccol >= term.cols) {
            term.ccol = 0;
            term.crow++;
            if (term.crow >= term.rows) {
                term.crow = term.rows - 1;
                term_scroll();
            }
        }
        term.buffer[term.crow][term.ccol++] = c;
    }
}

void term_draw(void) {
    XClearWindow(term.dpy, term.win);
    
    for (int i = 0; i < term.rows; i++) {
        XDrawString(term.dpy, term.win, term.gc,
                    2, (i + 1) * term.ch,
                    term.buffer[i], term.cols);
    }
    
    XFillRectangle(term.dpy, term.win, term.gc,
                   term.ccol * term.cw + 2,
                   term.crow * term.ch + 2,
                   term.cw, term.ch);
    
    XFlush(term.dpy);
}

void x11_init(void) {
    XSetWindowAttributes wa;
    XGCValues gcv;
    
    term.dpy = XOpenDisplay(NULL);
    if (!term.dpy)
        die("cannot open display");
    
    int screen = DefaultScreen(term.dpy);
    
    term.font = XLoadQueryFont(term.dpy, FONT);
    if (!term.font)
        term.font = XLoadQueryFont(term.dpy, "fixed");
    if (!term.font)
        die("cannot load font");
    
    term.cw = term.font->max_bounds.width;
    term.ch = term.font->ascent + term.font->descent;
    term.w = term.cols * term.cw + 4;
    term.h = term.rows * term.ch + 4;
    
    term.fg = BlackPixel(term.dpy, screen);
    term.bg = WhitePixel(term.dpy, screen);
    
    wa.background_pixel = term.bg;
    wa.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask;
    
    term.win = XCreateWindow(term.dpy, RootWindow(term.dpy, screen),
                             0, 0, term.w, term.h, 0,
                             CopyFromParent, InputOutput, CopyFromParent,
                             CWBackPixel | CWEventMask, &wa);
    
    XStoreName(term.dpy, term.win, TERM_NAME);
    
    gcv.foreground = term.fg;
    gcv.background = term.bg;
    gcv.font = term.font->fid;
    term.gc = XCreateGC(term.dpy, term.win,
                        GCForeground | GCBackground | GCFont, &gcv);
    
    XMapWindow(term.dpy, term.win);
}

void pty_init(void) {
    struct winsize ws = {term.rows, term.cols, 0, 0};
    
    term.child = forkpty(&term.master, NULL, NULL, &ws);
    if (term.child < 0)
        die("cannot fork");
    
    if (term.child == 0) {
        setenv("TERM", "xterm-256color", 1);
        execl(SHELL, SHELL, NULL);
        die("cannot exec shell");
    }
    
    signal(SIGCHLD, sigchld);
    fcntl(term.master, F_SETFL, O_NONBLOCK);
}

void handle_key(XKeyEvent *ev) {
    char buf[32];
    KeySym ksym;
    int len;
    
    len = XLookupString(ev, buf, sizeof(buf), &ksym, NULL);
    
    if (ev->state & ControlMask) {
        if (ksym == XK_c) {
            buf[0] = 0x03;
            len = 1;
        } else if (ksym == XK_d) {
            buf[0] = 0x04;
            len = 1;
        } else if (ksym == XK_l) {
            buf[0] = 0x0c;
            len = 1;
        }
    }
    
    if (len == 0) {
        switch (ksym) {
        case XK_Up:
            write(term.master, "\033[A", 3);
            return;
        case XK_Down:
            write(term.master, "\033[B", 3);
            return;
        case XK_Right:
            write(term.master, "\033[C", 3);
            return;
        case XK_Left:
            write(term.master, "\033[D", 3);
            return;
        case XK_Home:
            write(term.master, "\033[H", 3);
            return;
        case XK_End:
            write(term.master, "\033[F", 3);
            return;
        case XK_BackSpace:
            buf[0] = 0x7f;
            len = 1;
            break;
        case XK_Return:
            buf[0] = '\r';
            len = 1;
            break;
        }
    }
    
    if (len > 0)
        write(term.master, buf, len);
}

void run(void) {
    XEvent ev;
    fd_set rfds;
    char buf[4096];
    int xfd = ConnectionNumber(term.dpy);
    
    while (1) {
        FD_ZERO(&rfds);
        FD_SET(xfd, &rfds);
        FD_SET(term.master, &rfds);
        
        if (select((xfd > term.master ? xfd : term.master) + 1, &rfds, NULL, NULL, NULL) < 0)
            continue;
        
        if (FD_ISSET(term.master, &rfds)) {
            int n = read(term.master, buf, sizeof(buf));
            if (n > 0) {
                for (int i = 0; i < n; i++)
                    term_putc(buf[i]);
                term_draw();
            }
        }
        
        if (FD_ISSET(xfd, &rfds)) {
            while (XPending(term.dpy)) {
                XNextEvent(term.dpy, &ev);
                switch (ev.type) {
                case Expose:
                    term_draw();
                    break;
                case KeyPress:
                    handle_key(&ev.xkey);
                    break;
                }
            }
        }
    }
}

int main(void) {
    term_init();
    x11_init();
    pty_init();
    term_draw();
    run();
    return 0;
}
