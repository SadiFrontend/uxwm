# UX Ecosystem - Customization Guide

**Philosophy:** In UX, source code IS the configuration. Edit, compile, enjoy.

## Quick Start

1. Edit configuration file: `wm/config.h`
2. Rebuild: `make clean && make`
3. Reinstall: `sudo make install`
4. Restart WM: `Mod + Shift + e` then `startx`

## Window Manager Customization

### Appearance

#### Colors

Edit `wm/config.h`:

```c
static const char col_gray1[] = "#222222";  // Background
static const char col_gray2[] = "#444444";  // Inactive border
static const char col_gray3[] = "#bbbbbb";  // Text
static const char col_gray4[] = "#eeeeee";  // Selected text
static const char col_cyan[]  = "#005577";  // Active border/bg
static const char col_red[]   = "#ff0000";  // Urgent
```

**Popular themes:**

*Gruvbox Dark:*
```c
static const char col_gray1[] = "#282828";
static const char col_gray2[] = "#3c3836";
static const char col_gray3[] = "#ebdbb2";
static const char col_gray4[] = "#fbf1c7";
static const char col_cyan[]  = "#458588";
static const char col_red[]   = "#cc241d";
```

*Nord:*
```c
static const char col_gray1[] = "#2e3440";
static const char col_gray2[] = "#3b4252";
static const char col_gray3[] = "#eceff4";
static const char col_gray4[] = "#ffffff";
static const char col_cyan[]  = "#88c0d0";
static const char col_red[]   = "#bf616a";
```

*Dracula:*
```c
static const char col_gray1[] = "#282a36";
static const char col_gray2[] = "#44475a";
static const char col_gray3[] = "#f8f8f2";
static const char col_gray4[] = "#ffffff";
static const char col_cyan[]  = "#8be9fd";
static const char col_red[]   = "#ff5555";
```

#### Borders and Gaps

```c
static const unsigned int borderpx = 2;    // Window border width
static const unsigned int gappx    = 10;   // Gap between windows
static const unsigned int snap     = 32;   // Snap to edge distance
```

Try different values:
- **Minimal:** `borderpx = 1, gappx = 0`
- **Comfy:** `borderpx = 2, gappx = 10`
- **Spacious:** `borderpx = 3, gappx = 20`
- **Extreme:** `borderpx = 5, gappx = 30`

#### Bar

```c
static const int showbar     = 1;      // 0 = hide bar
static const int topbar      = 1;      // 0 = bottom bar
static const char *fonts[]   = { "monospace:size=10" };
```

**Font options:**
```c
// Larger
static const char *fonts[] = { "monospace:size=12" };

// Different font
static const char *fonts[] = { "Hack:size=10" };

// Multiple fallbacks
static const char *fonts[] = { 
    "JetBrains Mono:size=10",
    "monospace:size=10"
};
```

### Workspaces (Tags)

```c
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
```

**Custom names:**
```c
// Icons (requires font with icons)
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };

// Names
static const char *tags[] = { "web", "code", "term", "chat", "music", "sys", "7", "8", "9" };

// Minimal
static const char *tags[] = { "•", "•", "•", "•", "•" };

// Fewer tags
static const char *tags[] = { "1", "2", "3", "4", "5" };
```

### Layouts

```c
static const float mfact     = 0.55;   // Master area size (0.05 to 0.95)
static const int nmaster     = 1;      // Number of master windows
static const int resizehints = 1;      // Respect size hints
```

**Adjust ratios:**
```c
// More master area
static const float mfact = 0.65;

// Equal split
static const float mfact = 0.50;

// Two master windows
static const int nmaster = 2;
```

### Window Rules

Auto-float or tag specific applications:

```c
static const Rule rules[] = {
    /* class      instance    title       tags mask     floating   monitor */
    { "Gimp",     NULL,       NULL,       0,            1,         -1 },
    { "Firefox",  NULL,       NULL,       1 << 8,       0,         -1 },
    { "mpv",      NULL,       NULL,       0,            1,         -1 },
    { NULL,       NULL,   "scratchpad",   0,            1,         -1 },
};
```

**Examples:**

*Float all dialogs:*
```c
{ NULL, NULL, "File Upload", 0, 1, -1 },
{ NULL, NULL, "Open File",   0, 1, -1 },
```

*Assign to specific workspaces:*
```c
{ "Firefox",  NULL, NULL, 1 << 0, 0, -1 },  // Tag 1 (web)
{ "Code",     NULL, NULL, 1 << 1, 0, -1 },  // Tag 2 (code)
{ "Spotify",  NULL, NULL, 1 << 4, 0, -1 },  // Tag 5 (music)
```

*Find window class:*
```bash
xprop | grep WM_CLASS
# Click on window to see its class
```

### Keybindings

Located at the bottom of `wm/config.h`:

```c
static Key keys[] = {
    /* modifier         key             function        argument */
    { MOD,              XK_Return,      spawn,          {.v = termcmd } },
    // Add yours here
};
```

**Add custom bindings:**

*Browser:*
```c
static const char *browsercmd[] = { "firefox", NULL };
{ MOD, XK_w, spawn, {.v = browsercmd } },
```

*Screenshot:*
```c
static const char *scrotcmd[] = { "scrot", "%Y-%m-%d-%T.png", NULL };
{ 0, XK_Print, spawn, {.v = scrotcmd } },
```

*Lock screen:*
```c
static const char *lockcmd[] = { "slock", NULL };
{ MOD|ShiftMask, XK_l, spawn, {.v = lockcmd } },
```

*Volume controls (if not using media keys):*
```c
{ MOD, XK_F11, spawn, {.v = (const char*[]){ "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%", NULL } } },
{ MOD, XK_F12, spawn, {.v = (const char*[]){ "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%", NULL } } },
```

### Mouse Bindings

```c
static Button buttons[] = {
    /* click            event mask  button      function        argument */
    { ClkClientWin,     MOD,        Button1,    movemouse,      {0} },
    { ClkClientWin,     MOD,        Button3,    resizemouse,    {0} },
};
```

**Add bar clicks:**
```c
{ ClkTagBar,    0,          Button1,    view,       {0} },
{ ClkLtSymbol,  0,          Button1,    setlayout,  {0} },
```

## Terminal Customization

Edit `terminal/uxterm.c`:

### Font

```c
#define FONT "-*-fixed-medium-r-*-*-15-*-*-*-*-*-*-*"
```

**Try different fonts:**
```c
// Larger
#define FONT "-*-fixed-medium-r-*-*-18-*-*-*-*-*-*-*"

// Different font (if installed)
#define FONT "-*-terminus-medium-r-*-*-16-*-*-*-*-*-*-*"
```

### Shell

```c
#define SHELL "/bin/bash"
```

Change to your preferred shell:
```c
#define SHELL "/bin/zsh"
#define SHELL "/bin/fish"
```

### Terminal Size

```c
term.rows = 24;
term.cols = 80;
```

Adjust default size:
```c
term.rows = 30;
term.cols = 100;
```

### Colors

Find the color initialization in `x11_init()`:

```c
term.fg = BlackPixel(term.dpy, screen);
term.bg = WhitePixel(term.dpy, screen);
```

For custom colors, you'll need to add color allocation:

```c
XColor color;
Colormap cmap = DefaultColormap(term.dpy, screen);

// Dark background
XAllocNamedColor(term.dpy, cmap, "#1a1a1a", &color, &color);
term.bg = color.pixel;

// Light foreground
XAllocNamedColor(term.dpy, cmap, "#f0f0f0", &color, &color);
term.fg = color.pixel;
```

## Menu Customization

Edit `menu/uxmenu.c`:

### Font

```c
#define FONT "-*-fixed-medium-r-*-*-15-*-*-*-*-*-*-*"
```

### Colors

Find in `setup()` function:

```c
// Foreground/background
fg = BlackPixel(dpy, screen);
bg = WhitePixel(dpy, screen);

// Selection colors
if (XAllocNamedColor(dpy, cmap, "#eeeeee", &color, &color))
    selfg = color.pixel;
if (XAllocNamedColor(dpy, cmap, "#005577", &color, &color))
    selbg = color.pixel;
```

Change colors:
```c
// Match your WM theme
XAllocNamedColor(dpy, cmap, "#ebdbb2", &color, &color);  // fg
selfg = color.pixel;
XAllocNamedColor(dpy, cmap, "#458588", &color, &color);  // selected
selbg = color.pixel;
```

### Menu Height

```c
bh = font->ascent + font->descent + 4;
```

Adjust padding:
```c
bh = font->ascent + font->descent + 8;  // More padding
```

### Number of Visible Items

In `draw()` function:

```c
for (Item *it = matches; it && i < 10; it = it->next, i++) {
```

Change `10` to show more/fewer items:
```c
for (Item *it = matches; it && i < 15; it = it->next, i++) {
```

## Compositor Customization

Edit `compositor/uxcomp.c`:

### Transparency Level

```c
static int opacity = 0xd0000000; // ~80% opacity
```

**Opacity values:**
- `0xffffffff` - 100% (opaque)
- `0xe0000000` - ~88%
- `0xd0000000` - ~80% (default)
- `0xc0000000` - ~75%
- `0xb0000000` - ~69%
- `0xa0000000` - ~63%
- `0x80000000` - ~50%

### Background Color

In `paint_all()`:

```c
XRenderColor c = {0x2222, 0x2222, 0x2222, 0xffff};
```

Change background (values are 16-bit, so 0x0000-0xffff):
```c
// Darker
XRenderColor c = {0x1111, 0x1111, 0x1111, 0xffff};

// Lighter
XRenderColor c = {0x3333, 0x3333, 0x3333, 0xffff};

// Colored (e.g., dark blue)
XRenderColor c = {0x0000, 0x0000, 0x2222, 0xffff};
```

## Scripts Customization

### ux-session

Edit `scripts/ux-session`:

**Change terminal:**
```bash
UXTERM="${UXTERM:-alacritty}"  # Use alacritty instead
```

**Custom status bar:**
```bash
{
    while true; do
        # Your custom status here
        cpu=$(top -bn1 | grep Cpu | awk '{print $2}')
        mem=$(free -h | grep Mem | awk '{print $3}')
        date=$(date '+%H:%M')
        
        xsetroot -name "CPU:$cpu | RAM:$mem | $date"
        sleep 2
    done
} &
```

### uxmenu_run

Edit `scripts/uxmenu_run`:

**Add custom sources:**
```bash
# After the find commands, add:
echo "Custom App:/path/to/app"
echo "Another App:/usr/local/bin/another"
```

**Filter results:**
```bash
# Only GUI apps
grep -v "^vim:\|^nano:\|^htop:"
```

## Advanced Customization

### Add New Layout

In `wm/layout.c`, add your layout function:

```c
void grid(Monitor *m)
{
    unsigned int n, cols, rows, i;
    Client *c;
    
    for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
    if (n == 0) return;
    
    for (cols = 0; cols <= n/2; cols++)
        if (cols*cols >= n)
            break;
    
    rows = (cols && (cols - 1) * cols >= n) ? cols - 1 : cols;
    
    // Layout logic here...
}
```

In `wm/layout.h`, add to layouts array:

```c
static const Layout layouts[] = {
    { "[]=", tile },
    { "[M]", monocle },
    { "HHH", grid },     // Your new layout
    { "><>", NULL },
};
```

### Custom Status Bar Info

Create a status script:

```bash
#!/bin/bash
# ~/.config/ux/statusbar.sh

while true; do
    # Battery
    bat=$(cat /sys/class/power_supply/BAT0/capacity 2>/dev/null)
    [ -n "$bat" ] && bat="🔋$bat% "
    
    # Volume
    vol=$(pactl get-sink-volume @DEFAULT_SINK@ | grep -oP '\d+%' | head -1)
    
    # Network
    wifi=$(iwgetid -r)
    [ -n "$wifi" ] && wifi="📶$wifi "
    
    # Date/Time
    datetime=$(date '+%a %b %d %H:%M')
    
    xsetroot -name "$wifi$bat🔊$vol | $datetime"
    sleep 5
done
```

Add to autostart:
```bash
~/.config/ux/statusbar.sh &
```

### Per-Application Settings

Use window rules creatively:

```c
// Floating calculator
{ "Gnome-calculator", NULL, NULL, 0, 1, -1 },

// Chat on tag 4
{ "Discord", NULL, NULL, 1 << 3, 0, -1 },

// Video always floating
{ "mpv", NULL, NULL, 0, 1, -1 },

// Steam on last tag
{ "Steam", NULL, NULL, 1 << 8, 0, -1 },
```

## Testing Changes

After editing:

```bash
# Rebuild
cd ~/uxwm
make clean
make

# Test in Xephyr (don't affect current session)
Xephyr -screen 1280x720 :1 &
DISPLAY=:1 ./uxwm

# If good, install
sudo make install

# Restart WM
Mod + Shift + e
startx
```

## Backup Your Config

```bash
# Backup before major changes
cp wm/config.h wm/config.h.backup

# Or use git
cd ~/uxwm
git init
git add .
git commit -m "Initial config"

# After changes
git diff wm/config.h  # See what changed
git checkout wm/config.h  # Revert if needed
```

## Common Customization Recipes

### Laptop Setup
```c
// Smaller gaps on small screen
static const unsigned int gappx = 5;

// Show battery in status
// Add to ux-session script
```

### Multi-Monitor
```c
// Per-monitor layouts coming soon
// For now, use xrandr in autostart
```

### Gaming Setup
```c
// Easy fullscreen
{ MOD, XK_F11, togglefullscreen, {0} },

// Quick float for game launchers
{ "Steam", NULL, NULL, 0, 1, -1 },
```

### Developer Setup
```c
// Tags for workflow
static const char *tags[] = { "term", "edit", "test", "doc", "www", "6", "7", "8", "9" };

// Quick tools
static const char *vscode[] = { "code", NULL };
{ MOD, XK_c, spawn, {.v = vscode } },
```

## Further Customization

The sky's the limit! You have the source code. Want to add:
- Window swallowing
- Tab support
- Systray
- Different bar style
- More layouts

Just code it! That's the UX way.

## Getting Help

- Check existing configs online
- Ask in IRC/Discord
- Read the source - it's well commented
- Experiment - it's your system!

Remember: **Source code IS configuration.** Embrace it!
