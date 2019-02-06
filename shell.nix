{ pkgs ? import <nixpkgs> {} }:

let
  pkgsRiscv = import ./riscv.nix;
  stdenv = pkgsRiscv.stdenv;
  openocd = pkgs.callPackage ./openocd.nix {};

in stdenv.mkDerivation {
  name = "thesis";
  nativeBuildInputs = [ openocd ];
}
