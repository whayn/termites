{
  description = "Development environment for the ProgMOD S2 projet";
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        devShell = pkgs.mkShell {
          name = "progmod-dev-shell";
          buildInputs = [
            pkgs.clang
            pkgs.gnumake
            pkgs.bear
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
