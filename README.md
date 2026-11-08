# DIMENSION X — v0.4.0

**Target:** Windows · Geometry Dash 2.2081 · Geode 5.11.2

Main menu theme: dark footer, split green/cyan rails, mint wordmark, muted-gold subtitle,
optional opacity pulse. Original menu controls are preserved.

## Palette
- Void black `#0C0F12` · Panel charcoal `#141B1D`
- Portal green `#39E65B` · Portal cyan `#35D9D0`
- Evil gold `#D6BD36` · Fluid mint `#A9E6B0`

## Settings (in-game, Geode mod settings)
- **Wordmark pulse**: on/off
- **Footer opacity**: 0-255 (lower it if the footer covers the bottom menu buttons)
- **Themed pause menu**: on/off
- **Animated portal background** and **Portal brightness**
- **Show original characters**
- **Custom background image**: pick a PNG/JPG

## Build

### Option A: GitHub Actions (no local toolchain)
1. Create a GitHub repo and push this folder.
2. The **Build DIMENSION X** workflow runs on push, or run it from the Actions tab.
3. Download the **Build Output** artifact: it contains the `.geode` file.
4. Drop the `.geode` into `Geometry Dash/geode/mods/`.

### Option B: locally
Install the Geode SDK/CLI, Visual Studio 2022 Build Tools (Desktop development with C++),
Clang, Ninja, Git and CMake. Then from this folder: `geode build`.

## Changelog
### v0.4.0
- Animated portal background: rotating green/cyan vortex with a pulsing core, drawn in code.
- Two original silhouette characters: "Warden" (tall, cloaked, glowing visor) and
  "Sprocket" (hovering one-eyed drone).
- Custom background image setting (private use: you supply the image file).
- New settings: portal on/off, portal brightness, characters on/off, custom image.

### v0.3.0
- Themed pause menu: split green/cyan rails and wordmark, with a setting to turn it off.

### v0.2.1
- Fixed a compile error: `CCRepeatForever::create(pulse->copy())` passed a `CCObject*`
  where a `CCActionInterval*` is required. Each node now gets its own pulse action.
- Palette helpers use `ccc3()` instead of `constexpr` brace-init.
- CMake now uses C++20 (Geode's baseline) and Geode's expected ordering.
- Added mod settings for pulse and footer opacity.
- Added node IDs for rails and a double-init guard.
- Added a GitHub Actions workflow for Windows builds.

## Status
**Untested.** This has never been compiled or run in-game. First things to check:
the footer overlapping the stock bottom-row buttons, the wordmark position,
whether the portal draws behind the buttons (it attaches to the `main-menu-bg` node),
and whether the custom-image file setting loads correctly.

## Roadmap
- [x] Theme palette and mod skeleton
- [x] Main menu identity layer, rails and pulse
- [x] Settings
- [ ] Custom button skins and hover states
- [x] Pause menu (rails and wordmark; full restyle still to do)
- [ ] Level browser and level details
- [ ] Gameplay HUD
- [ ] Editor UI
- [ ] Settings and popup windows
- [x] Animated portal art, original characters, custom background
- [ ] Final testing
