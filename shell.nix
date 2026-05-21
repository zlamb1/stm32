let
    pkgs = import <nixpkgs> {};
    cross = pkgs.pkgsCross.armhf-embedded;
in
pkgs.mkShell {
    nativeBuildInputs = with pkgs.buildPackages; [
        cross.buildPackages.gdb
        cross.buildPackages.gcc
    ];
}