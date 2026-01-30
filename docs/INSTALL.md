# UX Ecosystem - Installation Guide

## Prerequisites

### System Requirements

**Minimum:**
- Any Linux distribution
- X11 display server
- 256 MB RAM
- 100 MB disk space

**Recommended:**
- Modern Linux distribution (Arch, Debian, Ubuntu, Fedora)
- 2 GB RAM
- 500 MB disk space

### Build Dependencies

**Arch Linux:**
```bash
sudo pacman -S base-devel libx11 libxinerama libxcomposite libxdamage libxrender
```

**Debian/Ubuntu:**
```bash
sudo apt install build-essential libx11-dev libxinerama-dev \
    libxcomposite-dev libxdamage-dev libxrender-dev
```

**Fedora:**
```bash
sudo dnf install @development-tools libX11-devel libXinerama-devel \
    libXcomposite-devel libXdamage-devel libXrender-devel
```

**Void Linux:**
```bash
sudo xbps-install -S base-devel libX11-devel libXinerama-devel \
    libXcomposite-devel libXdamage-devel libXrender-devel
```

## Quick Install

### Method 1: Using install.sh 

```bash
cd ~/uxwm
chmod +x install.sh
./install.sh
```

The installer will:
1. Check dependencies
2. Build all components
3. Install to `/usr/local/bin` (or `~/.local/bin` if not root)
4. Create configuration directory
5. Set up `.xinitrc`

### Method 2: Manual Installation (Recommended)

```bash
# Build all components
make clean
make all

# Install system-wide (requires root)
sudo make install

# Or install to user directory
PREFIX=$HOME/.local make install
```

## Post-Installation Setup

### 1. Configure X Session

The installer creates `~/.xinitrc`, but you can customize it:

```bash
nano ~/.xinitrc
```

Basic configuration:
```bash
#!/bin/sh

# Start compositor (optional)
uxcomp &

# Set wallpaper (if feh is available)
[ -f ~/.config/wallpaper ] && feh --bg-scale ~/.config/wallpaper &

# Status bar
while true; do
    xsetroot -name "$(date '+%Y-%m-%d %H:%M')"
    sleep 60
done &

# Start window manager
exec uxwm
```

### 2. Create Desktop Session File (Optional)

For display managers (GDM, LightDM, SDDM):

```bash
sudo nano /usr/share/xsessions/ux.desktop
```

Add:
```ini
[Desktop Entry]
Name=UX Desktop
Comment=Ultra-minimal X11 Desktop Environment
Exec=ux-session
Type=Application
DesktopNames=UX
```

### 3. Configure Autostart

Edit `~/.config/ux/autostart`:

```bash
mkdir -p ~/.config/ux
nano ~/.config/ux/autostart
```

Example:
```bash
#!/bin/sh

# Network manager applet
nm-applet &

# Volume control
volumeicon &

# Compositor with shadows
uxcomp &

# Wallpaper
feh --bg-scale ~/Pictures/wallpaper.jpg &
```

Make it executable:
```bash
chmod +x ~/.config/ux/autostart
```

## Starting UX Desktop

### From Console (TTY)

```bash
startx
```

### From Display Manager

1. Log out
2. Select "UX Desktop" from session menu
3. Log in

### Testing in Nested X Server (Xephyr)

```bash
# Install Xephyr
sudo pacman -S xorg-server-xephyr  # Arch
sudo apt install xserver-xephyr     # Debian/Ubuntu

# Run in window
Xephyr -screen 1280x720 :1 &
sleep 1
DISPLAY=:1 uxwm
```

## Component-Specific Setup

### UXTERM (Terminal)

UXTERM works out of the box, but you can customize:

**Font:** Edit `terminal/uxterm.c` and change:
```c
#define FONT "-*-fixed-medium-r-*-*-15-*-*-*-*-*-*-*"
```

**Shell:** Change default shell:
```c
#define SHELL "/bin/bash"  // or /bin/zsh, /bin/fish, etc.
```

Then rebuild:
```bash
make clean && make && sudo make install
```

### UXMENU (Application Launcher)

Create launcher script (automatically done by installer):

```bash
uxmenu_run
```

Or manually:
```bash
find /usr/bin -type f -executable | uxmenu | sh &
```

### UXCOMP (Compositor)

The compositor provides:
- Window transparency
- Smooth rendering
- Reduced tearing

**Adjust transparency:** Edit `compositor/uxcomp.c`:
```c
static int opacity = 0xd0000000; // 80% opacity
// Change to 0xe0000000 for 88%
// Change to 0xc0000000 for 75%
```

**Disable compositor:** Just don't start it, or:
```bash
killall uxcomp
```

## Customization

### Window Manager

Edit `wm/config.h` and rebuild:

```bash
nano wm/config.h
make clean
make
sudo make install
```

Common customizations:
- **Gaps:** Change `gappx`
- **Borders:** Change `borderpx`
- **Colors:** Change `col_*` values
- **Keybindings:** Edit `keys[]` array
- **Workspaces:** Modify `tags[]`

See [CUSTOMIZATION.md](CUSTOMIZATION.md) for details.

## Troubleshooting

### Build Errors

**"cannot find -lX11":**
```bash
# Install X11 development files
sudo pacman -S libx11  # Arch
sudo apt install libx11-dev  # Debian/Ubuntu
```

**"X11/Xlib.h: No such file":**
```bash
# Install X11 headers
sudo pacman -S xorg-dev  # Arch
sudo apt install xorg-dev  # Debian/Ubuntu
```

**"undefined reference to forkpty":**
```bash
# libutil is missing, should auto-link but try:
make LDFLAGS="-lutil" uxterm
```

### Runtime Issues

**"cannot open display":**
```bash
# Ensure X is running
echo $DISPLAY  # Should show :0 or similar

# Start X server
startx
```

**UXWM crashes on startup:**
```bash
# Check if another WM is running
killall openbox i3 dwm awesome

# View X error log
tail -f ~/.local/share/xorg/Xorg.0.log
```

**UXTERM shows garbled text:**
```bash
# Set proper TERM variable
export TERM=xterm-256color

# Or edit uxterm.c and set TERM there
```

**UXMENU is empty:**
```bash
# Test manually
echo -e "firefox\nchromium\nalacritty" | uxmenu
```

**UXCOMP causes tearing/artifacts:**
```bash
# Disable compositor
killall uxcomp

# Or adjust settings in uxcomp.c
```

### Permission Issues

**"Permission denied" during install:**
```bash
# Use sudo for system-wide install
sudo make install

# Or install to user directory
PREFIX=$HOME/.local make install
# Then add ~/.local/bin to PATH:
echo 'export PATH=$HOME/.local/bin:$PATH' >> ~/.bashrc
```

## Updating

```bash
cd ~/uxwm
git pull  # If using git
make clean
make all
sudo make install
```

## Uninstalling

```bash
cd ~/uxwm
sudo make uninstall
```

Manual cleanup:
```bash
rm -rf ~/.config/ux
rm ~/.xinitrc  # If you want to remove it
sudo rm /usr/share/xsessions/ux.desktop
```

## Verification

Check installation:
```bash
which uxwm uxterm uxmenu uxcomp ux-session uxmenu_run
```

All should show paths in `/usr/local/bin` or `~/.local/bin`.

Test each component:
```bash
# Terminal
uxterm

# Menu (needs X running)
echo "test" | uxmenu

# WM (in Xephyr)
Xephyr :1 & DISPLAY=:1 uxwm

# Compositor (needs WM running)
uxcomp &
```

## Next Steps

1. Read [KEYBINDINGS.md](KEYBINDINGS.md) for keyboard shortcuts
2. Read [CUSTOMIZATION.md](CUSTOMIZATION.md) for configuration
3. Join the community (IRC, forums, etc.)
4. Star the project on GitHub!

## Getting Help

- **Documentation:** Check `docs/` directory
- **Issues:** Open an issue on GitHub
- **Community:** IRC channel or forum
- **Wiki:** Check the project wiki

Enjoy your minimal, fast, and powerful desktop environment!
