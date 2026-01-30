#!/bin/bash
# UX Ecosystem - Complete File Structure Setup Script
# This creates the entire directory structure

set -e

echo "============================================"
echo "  UX Ecosystem - File Structure Creator"
echo "============================================"
echo ""

# Base directory
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

# WM files
cd "$BASE_DIR/wm"
touch uxwm.c wm.c wm.h x11.c x11.h atoms.c atoms.h \
      layout.c layout.h input.c input.h util.c util.h \
      drw.c drw.h config.def.h config.h

# Other components
touch "$BASE_DIR/terminal/uxterm.c"
touch "$BASE_DIR/menu/uxmenu.c"
touch "$BASE_DIR/compositor/uxcomp.c"

# Scripts
touch "$BASE_DIR/scripts/ux-session"
touch "$BASE_DIR/scripts/uxmenu_run"
chmod +x "$BASE_DIR/scripts"/*

# Root files
cd "$BASE_DIR"
touch README.md Makefile install.sh LICENSE

# Docs
touch docs/{INSTALL.md,KEYBINDINGS.md,CUSTOMIZATION.md,ROADMAP.md}

echo "✓ Files created"
echo ""

echo "============================================"
echo "  File structure created successfully!"
echo "============================================"
echo ""
echo "Next steps:"
echo "  1. Copy the code from each artifact into the corresponding file"
echo "  2. Use the mapping guide to know which artifact goes where"
echo "  3. Run: cd $BASE_DIR && make"
echo "  4. Install: sudo make install"
echo ""
echo "File mapping:"
echo "  README.md           ← ux_ecosystem_readme artifact"
echo "  Makefile            ← final_makefile artifact"
echo "  install.sh          ← ux_install_script artifact"
echo "  wm/uxwm.c           ← wm_uxwm_c artifact"
echo "  wm/wm.h             ← wm_h_improved artifact"
echo "  wm/wm.c             ← wm_c_improved artifact"
echo "  wm/x11.c            ← x11_c_improved artifact"
echo "  wm/atoms.c          ← atoms_c artifact"
echo "  wm/atoms.h          ← atoms_h artifact"
echo "  wm/layout.c         ← layout_c_improved artifact"
echo "  wm/layout.h         ← layout_h_improved artifact"
echo "  wm/input.c          ← input_c_improved artifact"
echo "  wm/input.h          ← input_h artifact"
echo "  wm/util.c           ← util_c artifact"
echo "  wm/util.h           ← util_h artifact"
echo "  wm/drw.c            ← drw_c artifact"
echo "  wm/drw.h            ← drw_h artifact"
echo "  wm/config.h         ← config_h_improved artifact"
echo "  terminal/uxterm.c   ← uxterm_main artifact"
echo "  menu/uxmenu.c       ← uxmenu_main artifact"
echo "  compositor/uxcomp.c ← uxcomp_main artifact"
echo "  scripts/ux-session  ← ux_session_script artifact"
echo "  scripts/uxmenu_run  ← uxmenu_run_script artifact"
echo "  docs/INSTALL.md     ← docs_install artifact"
echo "  docs/KEYBINDINGS.md ← docs_keybindings artifact"
echo "  docs/CUSTOMIZATION.md ← docs_customization artifact"
echo ""
echo "Happy hacking! 🚀"
