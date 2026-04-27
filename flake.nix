{
  description = "Development environment for the ProgMOD S2 projet";
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    nixgl.url = "github:nix-community/nixGL";
  };
  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      nixgl,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs {
          inherit system;
          overlays = [ nixgl.overlay ];
        };
      in
      {
        devShell = pkgs.mkShell {
          name = "progmod-dev-shell";
          buildInputs = with pkgs; [
            clang
            gnumake
            bear
            valgrind
            raylib
            pkg-config
            nix
            pkgs.nixgl.auto.nixGLDefault
          ];
          shellHook = ''
            if [ ! -f compile_commands.json ]; then
              make bear
            fi
          '';
        };
      }
    );
}
