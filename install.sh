#!/bin/bash
# UX Ecosystem Installer

set -e

echo "============================================"
echo "  UX Ecosystem Installer"
echo "  Better than dwm. Built from scratch."
echo "============================================"
echo ""

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

print_status() {
    echo -e "${GREEN}==>${NC} $1"
}

print_error() {
    echo -e "${RED}Error:${NC} $1" >&2
}

print_warning() {
    echo -e "${YELLOW}Warning:${NC} $1"
}

check_dependencies() {
    print_status "Checking dependencies..."
    
    local missing=""
    
    command -v cc >/dev/null 2>&1 || missing="$missing gcc/clang"
    command -v make >/dev/null 2>&1 || missing="$missing make"
    
    if ! pkg-config --exists x11 2>/dev/null; then
        missing="$missing libx11-dev"
    fi
    
    if [ -n "$missing" ]; then
        print_error "Missing dependencies:$missing"
        echo ""
        echo "Install on Arch: sudo pacman -S base-devel libx11 libxinerama libxcomposite libxdamage libxrender"
        echo "Install on Debian/Ubuntu: sudo apt install build-essential libx11-dev libxinerama-dev libxcomposite-dev libxdamage-dev libxrender-dev"
        exit 1
    fi
    
    print_status "All dependencies satisfied!"
}

PREFIX="${PREFIX:-/usr/local}"
BINDIR="$PREFIX/bin"

if [ "$PREFIX" = "/usr/local" ] && [ "$EUID" -ne 0 ]; then
    print_warning "Not running as root. Installing to $HOME/.local"
    PREFIX="$HOME/.local"
    BINDIR="$PREFIX/bin"
fi

mkdir -p "$BINDIR"

check_dependencies
echo ""

print_status "Building all components..."
make clean
make all

if [ $? -eq 0 ]; then
    print_status "Build successful!"
else
    print_error "Build failed"
    exit 1
fi

print_status "Installing to $BINDIR..."
make PREFIX="$PREFIX" install

print_status "Creating configuration directory..."
mkdir -p "$HOME/.config/ux"

if [ ! -f "$HOME/.config/ux/autostart" ]; then
    cat > "$HOME/.config/ux/autostart" << 'EOF'
#!/bin/sh
# UX Autostart - Add your startup applications here

# Example: Start compositor
# uxcomp &

# Example: Set wallpaper
# feh --bg-scale ~/Pictures/wallpaper.jpg &
EOF
    chmod +x "$HOME/.config/ux/autostart"
fi

if [ ! -f "$HOME/.xinitrc" ]; then
    print_status "Creating .xinitrc..."
    cat > "$HOME/.xinitrc" << 'EOF'
#!/bin/sh
exec ux-session
EOF
fi

echo ""
echo "============================================"
echo -e "${GREEN}Installation complete!${NC}"
echo "============================================"
echo ""
echo "Components installed:"
[ -f "$BINDIR/uxwm" ] && echo "  ✓ uxwm - Window Manager"
[ -f "$BINDIR/uxterm" ] && echo "  ✓ uxterm - Terminal Emulator"
[ -f "$BINDIR/uxmenu" ] && echo "  ✓ uxmenu - Application Launcher"
[ -f "$BINDIR/uxcomp" ] && echo "  ✓ uxcomp - Compositor"
echo ""
echo "To start using UX:"
echo "  1. Run: startx"
echo "  2. Or select 'UX Desktop' from your login manager"
echo ""
echo "Keybindings:"
echo "  Mod + Return  - Terminal"
echo "  Mod + p       - Application launcher"
echo "  Mod + Shift + e - Quit"
echo ""
