{ pkgs ? import <nixpkgs> {} }:

with pkgs;

let
  latexrun = callPackage ./latexrun.nix {};

in stdenvNoCC.mkDerivation {
  name = "ascon-32bit-paper";
  nativeBuildInputs = [ latexrun texlive.combined.scheme-medium ];
}
