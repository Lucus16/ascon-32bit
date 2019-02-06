{ pkgs ? import <nixpkgs> {} }:

with pkgs;

openocd.overrideAttrs (super: {
  nativeBuildInputs = [ autoreconfHook ] ++ super.nativeBuildInputs;
  src = fetchgit {
    url = https://github.com/riscv/riscv-openocd;
    rev = "c554246177bb8b2b03ca584847c4ffc2b2f7bb4b";
    sha256 = "1fvi4b1fjrbsiwpmdbc9rm7ajx521d9v1b8qhrjnzg9dhvg0mf0w";
  };
})
