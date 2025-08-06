# Snake
A minimal Snake game!

<img width="301" height="313" alt="Screenshot 2025-08-06 150850" src="https://github.com/user-attachments/assets/3af4b8b3-ec0f-4148-a5c8-6b74802db3fa" />

## Installation

You can either:

* Download the executable file from [itch.io](https://pepperjackdev.itch.io/snake), or
* [Build](#build-the-project) the project yourself.

## Build the Project

To build the project locally, first download it from GitHub or clone the repo:

```bash
git clone https://github.com/pepperjackdev/snake
```

### Using Convenience Methods

A utility `Makefile` is provided to simplify the build process:

* `make g` – generates the build scripts
* `make b` – builds the sources
* `make r` – runs the game

Each target depends on the preceding one. If you just want to run the game, use:

```bash
make r
```

For each of these targets, there's a WebAssembly counterpart using Emscripten:

* `make eg` – generates build scripts for WebAssembly
* `make eb` – builds for WebAssembly
* `make er` – runs the WebAssembly version

### Using CMake

You can also build the project using CMake directly:

1. Open a terminal in the local repository

2. Create a new directory for the build (e.g., `mkdir build`)

3. Move into that directory:

   ```bash
   cd build
   ```

4. Generate the build scripts with:

   ```bash
   cmake ..
   ```

5. Build the sources using your preferred build system (e.g., if you're using Ninja):

   ```bash
   ninja
   ```

## Playing the game
The game is extremely ~lazily developed~ minimal. Just run it and start playing by moving the snake with your `WASD` keys. To replay, simply close and re-run the game.
