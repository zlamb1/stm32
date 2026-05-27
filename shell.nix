let
    pkgs = import <nixpkgs> {};
    cross = pkgs.pkgsCross.armhf-embedded;
in
pkgs.mkShell {
    nativeBuildInputs = with pkgs.buildPackages; [
        bear
        cross.buildPackages.gdb
        cross.buildPackages.gcc
    ];
}