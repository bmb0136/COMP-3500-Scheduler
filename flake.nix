{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachDefaultSystem (system: let
      pkgs = nixpkgs.legacyPackages.${system};
      scheduler = self.packages.${system}.scheduler;
    in {
      packages.default = scheduler;
      packages.scheduler = pkgs.stdenv.mkDerivation {
        name = "scheduler";
        src = ./src;
        buildPhase = "make -j$(nproc)";
        installPhase = ''
          mkdir -p $out/bin
          cp scheduler $out/bin/
        '';
      };
    });
}
