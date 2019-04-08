{ pkgs ? import ../nixpkgs.nix {} }:

with pkgs;

stdenv.mkDerivation {
  name = "ascon-32bit-testcases";
  buildInputs = [ python3 ];
}
