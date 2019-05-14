{ pkgs ? import <nixpkgs> {} }:

with pkgs;

let
  latexrun = callPackage ./latexrun.nix {};
  texdeps = texlive.combine {
    inherit (texlive)
      scheme-basic amsmath appendix caption dejavu latexmk listings pgf url xcolor;
  };

in stdenvNoCC.mkDerivation {
  name = "ascon-32bit.pdf";
  src = ./.;
  nativeBuildInputs = [ latexrun texdeps ];

  buildPhase = ''
    latexrun main.tex
  '';

  installPhase = ''
    mv main.pdf $out
  '';
}
