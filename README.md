# Stigmergy in Termites

> [!WARNING]
> This project is undergoing translation from French to English. The README and game are currently in English, but the code comments and documentation may still be in French.


## Compilation
1. Clone the project:
   ```bash
    git clone https://github.com/whayn/termites.git
    cd termites
   ```
2. If you have nix installed, enter the development environment:
   ```bash
    nix develop --impure
   ```
  Note: If you don't have nix, you simply need to have raylib installed
3. Compile the project:
   ```bash
    make
   ```

## Usage
1. Run the compiled program:
   ```bash    
   ./main
   ```
  or if in the nix development environment:
   ```bash
   nixGL ./main
   ```

## License
This project is licensed under the GPL 3.0 License. See the [LICENSE](LICENSE) file for details
