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
      devShells.${system}.default = pkgs.mkShell {
        packages = with pkgs; [
          # To open a web server for emscripten build
          python3
          # Syntax highlighting, formatter, and linter
          clang-tools
          # Build tools
          cmake
          neocmakelsp
          emscripten
          # Debug tools
          gdb
          valgrind
          # Dev libraries
          alsa-lib
          hidapi
          ibus
          jack2
          libdecor
          libthai
          fribidi
          libGL
          libpulseaudio
          libusb1
          libX11
          libXcursor
          libXext
          libxfixes
          libxi
          libxinerama
          libxkbcommon
          libxrandr
          libxrender
          libxscrnsaver
          libxtst
          pipewire
          sndio
          vulkan-loader
          vulkan-headers
          wayland
          wayland-protocols
          libxcb
          # For editing flake
          nil
          nixd
        ];
        hardeningDisable = [ "all" ];
      };
    };
}
