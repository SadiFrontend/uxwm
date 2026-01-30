# UX Ecosystem - Complete Setup Checklist

Follow these steps to set up your complete UX ecosystem from scratch.

## Prerequisites

- [ ] Linux system with X11
- [ ] Development tools (`gcc`, `make`)
- [ ] X11 development libraries
- [ ] Terminal access

## Step 1: Create Directory Structure

```bash
cd ~
mkdir -p uxwm/{wm,terminal,menu,compositor,scripts,docs}
cd uxwm
```

**Or use the setup script:**
```bash
chmod +x setup.sh
./setup.sh
```

## Step 2: Copy Files

Copy each artifact to its corresponding file using the mapping below.

### Root Directory Files

- [ ] `README.md` ← Copy from **ux_ecosystem_readme** artifact
- [ ] `Makefile` ← Copy from **final_makefile** artifact  
- [ ] `install.sh` ← Copy from **ux_install_script** artifact
- [ ] `LICENSE` ← Add MIT license or your choice

### Window Manager Files (`wm/` directory)

- [ ] `wm/uxwm.c` ← Copy from **wm_uxwm_c** artifact
- [ ] `wm/wm.h` ← Copy from **wm_h_improved** artifact
- [ ] `wm/wm.c` ← Copy from **wm_c_improved** artifact (your current working version)
- [ ] `wm/x11.h` ← Copy from **x11_h_improved** artifact
- [ ] `wm/x11.c` ← Copy from **x11_c_improved** artifact
- [ ] `wm/atoms.h` ← Copy from **atoms_h** artifact
- [ ] `wm/atoms.c` ← Copy from **atoms_c** artifact
- [ ] `wm/layout.h` ← Copy from **layout_h_improved** artifact
- [ ] `wm/layout.c` ← Copy from **layout_c_improved** artifact
- [ ] `wm/input.h` ← Copy from **input_h** artifact
- [ ] `wm/input.c` ← Copy from **input_c_improved** artifact
- [ ] `wm/util.h` ← Copy from **util_h** artifact
- [ ] `wm/util.c` ← Copy from **util_c** artifact
- [ ] `wm/drw.h` ← Copy from **drw_h** artifact
- [ ] `wm/drw.c` ← Copy from **drw_c** artifact
- [ ] `wm/config.def.h` ← Copy from **config_def_h** artifact
- [ ] `wm/config.h` ← Copy from **config_h_improved** artifact

### Terminal Files (`terminal/` directory)

- [ ] `terminal/uxterm.c` ← Copy from **uxterm_main** artifact

### Menu Files (`menu/` directory)

- [ ] `menu/uxmenu.c` ← Copy from **uxmenu_main** artifact

### Compositor Files (`compositor/` directory)

- [ ] `compositor/uxcomp.c` ← Copy from **uxcomp_main** artifact

### Script Files (`scripts/` directory)

- [ ] `scripts/ux-session` ← Copy from **ux_session_script** artifact
- [ ] `scripts/uxmenu_run` ← Copy from **uxmenu_run_script** artifact
- [ ] Make scripts executable: `chmod +x scripts/*`

### Documentation Files (`docs/` directory)

- [ ] `docs/INSTALL.md` ← Copy from **docs_install** artifact
- [ ] `docs/KEYBINDINGS.md` ← Copy from **docs_keybindings** artifact
- [ ] `docs/CUSTOMIZATION.md` ← Copy from **docs_customization** artifact
- [ ] `docs/ROADMAP.md` ← Copy from **uxdistro_roadmap** artifact

## Step 3: Verify File Structure

```bash
cd ~/uxwm
tree -L 2
```

Should show:
```
.
├── compositor
│   └── uxcomp.c
├── docs
│   ├── CUSTOMIZATION.md
│   ├── INSTALL.md
│   ├── KEYBINDINGS.md
│   └── ROADMAP.md
├── install.sh
├── LICENSE
├── Makefile
├── menu
│   └── uxmenu.c
├── README.md
├── scripts
│   ├── ux-session
│   └── uxmenu_run
├── terminal
│   └── uxterm.c
└── wm
    ├── atoms.c
    ├── atoms.h
    ├── config.def.h
    ├── config.h
    ├── drw.c
    ├── drw.h
    ├── input.c
    ├── input.h
    ├── layout.c
    ├── layout.h
    ├── util.c
    ├── util.h
    ├── uxwm.c
    ├── wm.c
    ├── wm.h
    ├── x11.c
    └── x11.h
```

## Step 4: Install Dependencies

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

- [ ] Dependencies installed

## Step 5: Build

```bash
cd ~/uxwm
make clean
make all
```

**Expected output:**
```
UX Ecosystem build options:
CFLAGS   = -std=c99 -pedantic -Wall -Wextra -Os
LDFLAGS  = -lX11
CC       = cc
Compiling wm/uxwm.c...
Compiling wm/x11.c...
Compiling wm/wm.c...
Compiling wm/util.c...
Compiling wm/atoms.c...
Compiling wm/layout.c...
Compiling wm/input.c...
Compiling wm/drw.c...
Linking uxwm...
Building uxterm...
Building uxmenu...
Building uxcomp...
```

- [ ] All components built successfully
- [ ] No compilation errors

**If there are errors:**
- Check that all files are copied correctly
- Verify dependencies are installed
- Check for typos in the code

## Step 6: Install

```bash
sudo make install
```

**Or for user installation:**
```bash
PREFIX=$HOME/.local make install
export PATH=$HOME/.local/bin:$PATH
```

- [ ] Installation complete
- [ ] Binaries in `/usr/local/bin` or `~/.local/bin`

**Verify:**
```bash
which uxwm uxterm uxmenu uxcomp ux-session uxmenu_run
```

All should return paths.

## Step 7: Configure X Session

**Create .xinitrc:**
```bash
cat > ~/.xinitrc << 'EOF'
#!/bin/sh
exec ux-session
EOF
chmod +x ~/.xinitrc
```

- [ ] .xinitrc created

**Or create desktop session file:**
```bash
sudo tee /usr/share/xsessions/ux.desktop << 'EOF'
[Desktop Entry]
Name=UX Desktop
Comment=Ultra-minimal X11 Desktop Environment
Exec=ux-session
Type=Application
DesktopNames=UX
EOF
```

- [ ] Desktop session file created (optional)

## Step 8: Test in Xephyr (Optional but Recommended)

```bash
# Install Xephyr
sudo pacman -S xorg-server-xephyr  # or equivalent for your distro

# Start nested X server
Xephyr -screen 1280x720 :1 &
sleep 1

# Test uxwm
DISPLAY=:1 uxwm
```

- [ ] Tested in Xephyr
- [ ] Window manager starts without errors
- [ ] Can open terminal with Mod+Return
- [ ] All keybindings work

## Step 9: First Real Launch

**From TTY:**
```bash
# Switch to TTY (Ctrl+Alt+F2)
# Login
startx
```

**From Display Manager:**
- Log out
- Select "UX Desktop" from session menu
- Log in

- [ ] Successfully launched
- [ ] Desktop appears
- [ ] Bar shows up
- [ ] Can interact with windows

## Step 10: Test All Components

- [ ] **UXWM**: Window management works
  - [ ] Open terminal: `Mod + Return`
  - [ ] Switch workspaces: `Mod + 1-9`
  - [ ] Resize master: `Mod + h/l`
  - [ ] Toggle layout: `Mod + t/m`
  - [ ] Toggle gaps: `Mod + g`

- [ ] **UXTERM**: Terminal works
  - [ ] Opens successfully
  - [ ] Text displays correctly
  - [ ] Colors work
  - [ ] Can run commands
  - [ ] Arrow keys work

- [ ] **UXMENU**: Launcher works
  - [ ] Opens with `Mod + p`
  - [ ] Shows applications
  - [ ] Filtering works
  - [ ] Can launch apps

- [ ] **UXCOMP**: Compositor works
  - [ ] Transparency visible
  - [ ] No tearing
  - [ ] Smooth rendering

## Step 11: Customize (Optional)

- [ ] Read `docs/CUSTOMIZATION.md`
- [ ] Edit `wm/config.h` with your preferences
- [ ] Rebuild: `make clean && make && sudo make install`
- [ ] Restart WM

## Step 12: Set Up Autostart

```bash
mkdir -p ~/.config/ux
cat > ~/.config/ux/autostart << 'EOF'
#!/bin/sh

# Start compositor
uxcomp &

# Set wallpaper (if feh available)
[ -f ~/Pictures/wallpaper.jpg ] && feh --bg-scale ~/Pictures/wallpaper.jpg &

# Network manager (if available)
command -v nm-applet && nm-applet &

# Add your apps here
EOF
chmod +x ~/.config/ux/autostart
```

- [ ] Autostart script created
- [ ] Added desired applications

## Troubleshooting Checklist

If something doesn't work:

- [ ] Check all files are copied correctly
- [ ] Verify no typos in the code
- [ ] Ensure all dependencies installed
- [ ] Check build output for errors
- [ ] View X logs: `tail -f ~/.local/share/xorg/Xorg.0.log`
- [ ] Test each component individually
- [ ] Try in Xephyr first
- [ ] Check keybindings in `wm/config.h`

## Post-Setup Tasks

- [ ] Star the project on GitHub (if published)
- [ ] Read all documentation in `docs/`
- [ ] Join community (IRC, Matrix)
- [ ] Share your setup on r/unixporn
- [ ] Contribute improvements
- [ ] Tell others about UX!

## Quick Reference

**Build commands:**
```bash
make clean    # Clean build files
make all      # Build all components
make install  # Install (needs sudo)
make dist     # Create tarball
```

**File locations:**
```
Binaries:    /usr/local/bin/ or ~/.local/bin/
Config:      ~/.config/ux/
Session:     ~/.xinitrc or /usr/share/xsessions/
Logs:        ~/.local/share/xorg/
```

**Key files to customize:**
```
wm/config.h           # WM configuration
terminal/uxterm.c     # Terminal settings
menu/uxmenu.c         # Menu appearance
compositor/uxcomp.c   # Transparency level
scripts/ux-session    # Session startup
~/.config/ux/autostart # Autostart apps
```

## Success Criteria

You've successfully set up UX when:

✅ All components compile without errors  
✅ WM starts and is usable  
✅ Terminal works properly  
✅ App launcher functions  
✅ Compositor provides transparency  
✅ All keybindings respond  
✅ Can switch workspaces  
✅ Windows tile correctly  
✅ Bar shows information  
✅ Can customize and rebuild  

## Congratulations! 🎉

You now have a complete, minimal, fast, and powerful desktop environment built from scratch!

**Next steps:**
- Customize it to your liking
- Learn the keybindings
- Explore the source code
- Contribute improvements
- Spread the word!

**Remember:** You built this yourself. You control everything. Make it yours!

---

**Made with ❤️ and very big motivation.**
