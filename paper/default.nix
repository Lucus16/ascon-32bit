{ pkgs ? import <nixpkgs> {} }:

with pkgs;

let
  latexrun = callPackage ./latexrun.nix {};

in stdenvNoCC.mkDerivation {
  name = "ascon-32bit.pdf";
  src = ./.;
  nativeBuildInputs = [ latexrun texlive.combined.scheme-medium ];

  buildPhase = ''
    latexrun main.tex
  '';

  installPhase = ''
    mv main.pdf $out
  '';
}
