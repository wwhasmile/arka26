{
  description = "My spin on Arkanoid, mainly inspired by the first Touhou game.";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { self, ... }@inputs:
    let
      system = "x86_64-linux";

      pkgs = import inputs.nixpkgs {
        inherit system;
      };
    in {
      devShells.${system}.default = pkgs.mkShellNoCC {
        packages = with pkgs; [
          # To open a web server for emscripten build
          python3
          # To generate compiler_commands.json
          compiledb
          # Syntax highlighting, formatter, and linter
          clang-tools
          # Build tools
          gnumake
          gcc
          emscripten
          # Debug tools
          gdb
          valgrind
          # For editing flake
          nil
          nixd
        ];
        hardeningDisable = [ "all" ];
      };
    };
}
