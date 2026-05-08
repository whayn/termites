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

        commonBuildDeps = with pkgs; [
          clang
          gnumake
          bear
          raylib
          pkg-config
        ];

        devTools = with pkgs; [
          valgrind
          pkgs.nixgl.auto.nixGLDefault
        ];

      in
      {
        devShell = pkgs.mkShell {
          name = "projet-termites-dev-shell";
          buildInputs = commonBuildDeps ++ devTools;
          shellHook = ''
            if [ ! -f compile_commands.json ]; then
              make bear
            fi
          '';
        };
      }

    );
}
