# UX Window Manager - Keybindings Reference

**Modifier Key:** `Mod` = Super/Windows key (Mod4)

## Quick Reference Card

### Essential Bindings
| Keybinding | Action |
|------------|--------|
| `Mod + Return` | Open terminal |
| `Mod + p` | Application launcher |
| `Mod + Shift + q` | Close window |
| `Mod + Shift + e` | Quit WM |

## Complete Keybindings

### Applications

| Keybinding | Action | Description |
|------------|--------|-------------|
| `Mod + Return` | Spawn terminal | Opens uxterm |
| `Mod + p` | Application launcher | Opens uxmenu_run |
| `Mod + Shift + Return` | Spawn floating terminal | Terminal in floating mode |

### Window Management

| Keybinding | Action | Description |
|------------|--------|-------------|
| `Mod + Shift + q` | Kill client | Close focused window |
| `Mod + Space` | Toggle floating | Float/tile current window |
| `Mod + f` | Toggle fullscreen | Fullscreen current window |
| `Mod + Shift + Space` | Sink/promote | Move to master/stack |

### Focus Navigation

| Keybinding | Action | Description |
|------------|--------|-------------|
| `Mod + j` | Focus next | Focus next window in stack |
| `Mod + k` | Focus previous | Focus previous window in stack |
| `Mod + Tab` | Focus next | Cycle through windows forward |
| `Mod + Shift + Tab` | Focus previous | Cycle through windows backward |
| `Mod + m` | Focus master | Jump to master window |

### Window Movement

| Keybinding | Action | Description |
|------------|--------|-------------|
| `Mod + Shift + j` | Move down | Swap with next window |
| `Mod + Shift + k` | Move up | Swap with previous window |
| `Mod + Shift + Return` | Zoom | Swap with master window |

### Layout Control

| Keybinding | Action | Description |
|------------|--------|-------------|
| `Mod + t` | Tile layout | Traditional tiling |
| `Mod + m` | Monocle layout | Fullscreen focused window |
| `Mod + f` | Floating layout | Free-form positioning |
| `Mod + g` | Toggle gaps | Enable/disable window gaps |

### Master Area

| Keybinding | Action | Description |
|------------|--------|-------------|
| `Mod + h` | Decrease master | Shrink master area |
| `Mod + l` | Increase master | Grow master area |
| `Mod + i` | Increment nmaster | More windows in master |
| `Mod + d` | Decrement nmaster | Fewer windows in master |

### Workspaces (Tags)

| Keybinding | Action | Description |
|------------|--------|-------------|
| `Mod + 1-9` | View tag | Switch to workspace 1-9 |
| `Mod + 0` | View all | View all workspaces |
| `Mod + Shift + 1-9` | Move to tag | Move window to workspace |
| `Mod + Shift + 0` | Tag all | Window on all workspaces |
| `Mod + Ctrl + 1-9` | Toggle tag | Add/remove from workspace |
| `Mod + Ctrl + Shift + 1-9` | Toggle tag client | Toggle window on workspace |

### Monitor Control (Multi-monitor)

| Keybinding | Action | Description |
|------------|--------|-------------|
| `Mod + ,` | Focus prev monitor | Focus previous monitor |
| `Mod + .` | Focus next monitor | Focus next monitor |
| `Mod + Shift + ,` | Move to prev monitor | Send window to prev monitor |
| `Mod + Shift + .` | Move to next monitor | Send window to next monitor |

### Bar Control

| Keybinding | Action | Description |
|------------|--------|-------------|
| `Mod + b` | Toggle bar | Show/hide status bar |

### System Control

| Keybinding | Action | Description |
|------------|--------|-------------|
| `Mod + Shift + e` | Quit WM | Exit window manager |
| `Mod + Shift + r` | Restart WM | Reload configuration |

### Media Keys

| Keybinding | Action | Description |
|------------|--------|-------------|
| `XF86AudioRaiseVolume` | Volume up | Increase volume 5% |
| `XF86AudioLowerVolume` | Volume down | Decrease volume 5% |
| `XF86AudioMute` | Toggle mute | Mute/unmute audio |
| `XF86MonBrightnessUp` | Brightness up | Increase brightness |
| `XF86MonBrightnessDown` | Brightness down | Decrease brightness |

## Mouse Bindings

### Window Operations

| Button + Modifier | Action | Description |
|-------------------|--------|-------------|
| `Mod + Left Click + Drag` | Move window | Drag to move floating window |
| `Mod + Right Click + Drag` | Resize window | Drag to resize window |
| `Mod + Middle Click` | Toggle floating | Float/tile window under cursor |

### Bar Clicks

| Click Location | Action | Description |
|----------------|--------|-------------|
| Layout symbol | Cycle layout | Switch between layouts |
| Window title | Zoom | Swap with master |
| Tag number | View tag | Switch to that workspace |
| Tag number (right-click) | Toggle view | Add/remove workspace from view |
| Status text | Spawn terminal | Quick terminal access |

## Terminal Keybindings (UXTERM)

| Keybinding | Action | Description |
|------------|--------|-------------|
| `Ctrl + C` | Interrupt | Send SIGINT |
| `Ctrl + D` | EOF | Close terminal if empty |
| `Ctrl + L` | Clear | Clear screen |
| Arrow keys | Navigation | Cursor movement |
| `Home` | Line start | Jump to beginning |
| `End` | Line end | Jump to end |
| `Backspace` | Delete char | Delete previous character |

## Menu Keybindings (UXMENU)

| Keybinding | Action | Description |
|------------|--------|-------------|
| `Type` | Filter | Filter applications |
| `Enter` | Select | Launch selected application |
| `Escape` | Cancel | Close menu |
| `Up` / `Ctrl + p` | Previous | Select previous item |
| `Down` / `Ctrl + n` | Next | Select next item |
| `Tab` | Next | Select next item |
| `Shift + Tab` | Previous | Select previous item |
| `Backspace` | Delete char | Remove last character |

## Customizing Keybindings

To customize, edit `wm/config.h`:

```c
static Key keys[] = {
    /* modifier     key             function        argument */
    { MOD,          XK_Return,      spawn,          {.v = termcmd } },
    { MOD,          XK_p,           spawn,          {.v = dmenucmd } },
    // Add your bindings here
};
```

### Example Custom Bindings

**Launch browser:**
```c
static const char *browsercmd[] = { "firefox", NULL };
{ MOD, XK_w, spawn, {.v = browsercmd } },
```

**Launch file manager:**
```c
static const char *filecmd[] = { "thunar", NULL };
{ MOD, XK_e, spawn, {.v = filecmd } },
```

**Lock screen:**
```c
static const char *lockcmd[] = { "slock", NULL };
{ MOD|ShiftMask, XK_l, spawn, {.v = lockcmd } },
```

**Screenshot:**
```c
static const char *screenshotcmd[] = { "scrot", NULL };
{ 0, XK_Print, spawn, {.v = screenshotcmd } },
```

After editing, rebuild:
```bash
cd ~/uxwm
make clean && make && sudo make install
```

## Tips & Tricks

### Workflow Optimization

1. **Use workspaces** - Organize by task (1=web, 2=code, 3=music, etc.)
2. **Master area** - Keep main window in master, utilities in stack
3. **Floating for dialogs** - Most dialogs auto-float
4. **Fullscreen for focus** - `Mod + f` for distraction-free work

### Common Workflows

**Web Development:**
- Tag 1: Browser (Firefox/Chrome)
- Tag 2: Code editor
- Tag 3: Terminal for build/test
- `Mod + h/l` to adjust editor/browser ratio

**Writing:**
- Monocle mode (`Mod + m`)
- Single window fullscreen
- Minimal distractions

**Multitasking:**
- Use 3-4 tags
- `Mod + 0` to see everything
- Quick switch with `Mod + number`

### Efficiency Tips

1. **Learn the number row** - Fastest workspace switching
2. **Master j/k** - Faster than mouse for focus
3. **Use Tab** - Quick window cycling
4. **Remember Space** - Toggle floating instantly
5. **h/l muscle memory** - Resize without thinking

## Conflicts & Compatibility

### Avoiding Conflicts

Some applications use `Mod` key combinations:

**IntelliJ IDEA:**
- Change IDEA shortcuts or UX shortcuts
- Consider using `Mod + Alt` for UX

**Browsers:**
- `Mod + number` might conflict
- Use browser in fullscreen or floating

**Games:**
- Run in floating mode
- Or temporarily disable WM shortcuts

### Debugging Key Issues

**Keys not working:**
```bash
# Check key codes
xev

# Verify grab
xdotool key --clearmodifiers Super_L+Return
```

**Wrong keys triggered:**
```bash
# Check keyboard layout
setxkbmap -query

# Reset
setxkbmap us
```

## Cheat Sheet Summary

```
LAUNCH              FOCUS               MOVE
Mod + Return        Mod + j/k           Mod + Shift + j/k
Mod + p             Mod + Tab           Mod + Shift + Return

LAYOUT              RESIZE              WORKSPACE  
Mod + t/m/f         Mod + h/l           Mod + 1-9
Mod + g (gaps)      Mod + i/d           Mod + Shift + 1-9

WINDOW              SYSTEM
Mod + Space         Mod + Shift + e
Mod + Shift + q     Mod + b
```

Print this and keep it handy until the bindings become muscle memory!

## Further Reading

- [CUSTOMIZATION.md](CUSTOMIZATION.md) - Customize keybindings
- [README.md](../README.md) - General documentation
- Configuration: `wm/config.h`
