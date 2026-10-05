Roguelike (C++ / raylib)
A classic dungeon-crawler roguelike in the spirit of Rogue, written in C++20 with raylib. The current focus is the procedural level generator; the gameplay systems are being built on top of it.
> **Status:** early development. Seed-based level generation and the main menu work; the player, enemy and item classes are defined but not yet playable.

Level generation
Every level is generated from a single seed, so the same seed always produces the same dungeon.
Per-level seeds. A master seed is mixed with the level number through an integer hash (golden-ratio constant plus avalanche mixing), and the result seeds a `std::mt19937` generator. Each depth gets its own independent but reproducible layout.
Rooms. The 200 × 50 tile map is split into 2–6 columns, and each column into 1–3 cells. Each cell gets a room with a 5/6 chance, with a random size and position inside the cell, so rooms never overlap.
Connecting the rooms. The room centres form a complete graph weighted by Euclidean distance. A minimum spanning tree is built with Kruskal's algorithm, using a union-find structure with path compression and union by rank. Every room is reachable, with the shortest possible total corridor length.
Corridors. Each tree edge becomes an L-shaped corridor, randomly horizontal-first or vertical-first. Walls are placed around the corridors automatically.
Tech stack
C++20 · raylib 6.0 · raygui 5.0 · CMake (presets)
Project structure
```
main.cpp            window setup and main loop
include/            class definitions: Map, Level, Player, Enemy, Item, helpers (MST, union-find, RNG)
impl/map.cpp        level generation
impl/graphics.cpp   level rendering and the main menu
impl/gplay_loop.cpp game state machine (menu, gameplay, settings)
```
Building
Requirements: CMake 3.25+, a C++20 compiler, and raylib's system dependencies (on Debian/Ubuntu: `libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev`). raylib and raygui are downloaded automatically on the first configure.
```
cmake --workflow --preset linux
```
The executable is placed in `build/bin/`.
Planned
Player movement and turn-based combat
Enemies, items, chests and an inventory
Stairs to deeper levels, each generated from the same master seed
Settings menu
