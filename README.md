# Hoshi

**Hoshi** is a powerful and flexible modloader designed for Kirby Air Ride (NTSC). It allows modders to load custom mod files off disc, hook into game code with callbacks, manage mod save data, and extend game behavior with custom menus and scenes.

---

## Features

- **Mod File Loading**  
  **Hoshi** automatically detects and loads all compatible mod binaries placed in the `mods` folder on the game disc. Modders do not need to hardcode any mod paths or registration logic. **Hoshi** handles discovery and initialization automatically.

- **Code Hooking**  
  Define callback functions for injecting code at key points in game execution.

- **Unique Save File System**  
  Hoshi creates and manages its own save file, automatically handling save slots for each mod.

- **Custom Settings Menu**  
  Add your own toggles and menus to a shared in-game settings menu, allowing users to easily configure your mod.

- **Scene Injection Support**  
  Register new major and minor scenes to integrate fully with the game’s flow and scene manager.

---

## Create your own module

To develop your own module, clone the mod template repo [here](https://github.com/UnclePunch/hoshi-mod) and follow the instructions provided.

---

## Build Instructions

To-do.

---

## License

This project is licensed under the [GPLv3](./LICENSE).

---
