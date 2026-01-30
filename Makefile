# UX Ecosystem Build System
VERSION = 1.0

PREFIX = /usr/local
BINDIR = ${PREFIX}/bin

# Compiler
CC = cc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Os
LDFLAGS = -lX11

# Paths
X11INC = /usr/include
X11LIB = /usr/lib

# Component-specific flags
UXWM_CFLAGS = ${CFLAGS} -I${X11INC} -Iwm -DXINERAMA
UXWM_LDFLAGS = ${LDFLAGS} -lXinerama
UXTERM_LDFLAGS = ${LDFLAGS} -lutil
UXCOMP_LDFLAGS = ${LDFLAGS} -lXcomposite -lXdamage -lXrender

# WM sources
UXWM_SRC = wm/uxwm.c wm/x11.c wm/wm.c wm/util.c wm/atoms.c wm/layout.c wm/input.c wm/drw.c
UXWM_OBJ = ${UXWM_SRC:.c=.o}

all: options uxwm uxterm uxmenu uxcomp

options:
	@echo "UX Ecosystem build options:"
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

# Window Manager
uxwm: ${UXWM_OBJ}
	@echo "Linking uxwm..."
	@${CC} -o $@ ${UXWM_OBJ} ${UXWM_LDFLAGS}

wm/%.o: wm/%.c
	@echo "Compiling $<..."
	@${CC} -c ${UXWM_CFLAGS} -o $@ $<

# Terminal Emulator
uxterm: terminal/uxterm.c
	@echo "Building uxterm..."
	@${CC} ${CFLAGS} -o $@ $< ${UXTERM_LDFLAGS}

# Application Menu
uxmenu: menu/uxmenu.c
	@echo "Building uxmenu..."
	@${CC} ${CFLAGS} -o $@ $< ${LDFLAGS}

# Compositor
uxcomp: compositor/uxcomp.c
	@echo "Building uxcomp..."
	@${CC} ${CFLAGS} -o $@ $< ${UXCOMP_LDFLAGS}

clean:
	@echo "Cleaning build files..."
	@rm -f uxwm uxterm uxmenu uxcomp wm/*.o

install: all
	@echo "Installing to ${BINDIR}..."
	@mkdir -p ${DESTDIR}${BINDIR}
	@cp -f uxwm uxterm uxmenu uxcomp ${DESTDIR}${BINDIR}
	@cp -f scripts/ux-session scripts/uxmenu_run ${DESTDIR}${BINDIR}
	@chmod 755 ${DESTDIR}${BINDIR}/uxwm
	@chmod 755 ${DESTDIR}${BINDIR}/uxterm
	@chmod 755 ${DESTDIR}${BINDIR}/uxmenu
	@chmod 755 ${DESTDIR}${BINDIR}/uxcomp
	@chmod 755 ${DESTDIR}${BINDIR}/ux-session
	@chmod 755 ${DESTDIR}${BINDIR}/uxmenu_run
	@echo "Installation complete!"

uninstall:
	@echo "Uninstalling..."
	@rm -f ${DESTDIR}${BINDIR}/uxwm
	@rm -f ${DESTDIR}${BINDIR}/uxterm
	@rm -f ${DESTDIR}${BINDIR}/uxmenu
	@rm -f ${DESTDIR}${BINDIR}/uxcomp
	@rm -f ${DESTDIR}${BINDIR}/ux-session
	@rm -f ${DESTDIR}${BINDIR}/uxmenu_run
	@echo "Uninstallation complete!"

dist: clean
	@echo "Creating distribution tarball..."
	@mkdir -p uxwm-${VERSION}
	@cp -R wm terminal menu compositor scripts docs README.md Makefile LICENSE uxwm-${VERSION}
	@tar -czf uxwm-${VERSION}.tar.gz uxwm-${VERSION}
	@rm -rf uxwm-${VERSION}
	@echo "Created uxwm-${VERSION}.tar.gz"

.PHONY: all options clean install uninstall dist
