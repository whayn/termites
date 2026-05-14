# Stigmergy in Termites

> [!WARNING]
> This project is undergoing translation from French to English. The README and game are currently in English, but the code comments and documentation may still be in French.

## Compilation

1. Clone the project and initialize submodules (required for dependencies like Raylib, ImGui, and Doctest):
   ```bash
   git clone --recursive https://github.com/whayn/termites.git
   cd termites
   ```
   *(If you already cloned without `--recursive`, run `git submodule update --init --recursive`)*

2. If you have Nix installed, you can enter the development environment to get the required C++ toolchains and CMake:
   ```bash
   nix develop --impure
   ```
   *Note: Because dependencies are now handled internally via CMake and git submodules, you no longer need Raylib installed system-wide. You just need a standard C++ compiler and CMake.*

3. Configure and compile the project using CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build . -j$(nproc)
   ```

## Usage

1. Run the compiled program from the `build` directory:
   ```bash    
   ./main
   ```
   *(Or if in the Nix development environment, you may need `nixGL ./main`)*

2. Run the tests:
   ```bash
   ctest --output-on-failure
   ```

## License
This project is licensed under the GPL 3.0 License. See the [LICENSE](LICENSE) file for details.