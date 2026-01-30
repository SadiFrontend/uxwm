#!/bin/bash
# UX Ecosystem - Complete File Structure Setup Script

set -e

echo "============================================"
echo "  UX Ecosystem - File Structure Creator"
echo "============================================"
echo ""

BASE_DIR="$HOME/uxwm"

echo "Creating directory structure in $BASE_DIR..."
mkdir -p "$BASE_DIR"/{wm,terminal,menu,compositor,scripts,docs}

echo "✓ Directories created"
echo ""
echo "Directory structure:"
echo "~/uxwm/"
echo "├── wm/              # Window Manager files"
echo "├── terminal/        # Terminal Emulator"
echo "├── menu/            # Application Launcher"
echo "├── compositor/      # Compositor"
echo "├── scripts/         # Helper scripts"
echo "└── docs/            # Documentation"
echo ""

echo "Creating placeholder files..."

cd "$BASE_DIR/wm"
touch uxwm.c wm.c wm.h x11.c x11.h atoms.c atoms.h \
      layout.c layout.h input.c input.h util.c util.h \
      drw.c drw.h config.def.h config.h

touch "$BASE_DIR/terminal/uxterm.c"
touch "$BASE_DIR/menu/uxmenu.c"
touch "$BASE_DIR/compositor/uxcomp.c"

touch "$BASE_DIR/scripts/ux-session"
touch "$BASE_DIR/scripts/uxmenu_run"
chmod +x "$BASE_DIR/scripts"/*

cd "$BASE_DIR"
touch README.md Makefile install.sh LICENSE setup.sh
chmod +x install.sh setup.sh

touch docs/{INSTALL.md,KEYBINDINGS.md,CUSTOMIZATION.md,ROADMAP.md}

echo "✓ Files created"
echo ""
echo "============================================"
echo "  File structure created successfully!"
echo "============================================"
echo ""
echo "Next steps:"
echo "  1. Copy the code from each artifact into the corresponding file"
echo "  2. Run: cd $BASE_DIR && make"
echo "  3. Install: sudo make install or ./install.sh"
echo ""
echo "Happy hacking! 🚀"
