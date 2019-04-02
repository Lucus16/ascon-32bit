# Ascon for 32-bit microcontrollers

This repository contains implementations of the Ascon authenticated encryption
cipher for some 32-bit microcontrollers.

### Dependencies

This repository uses the [Nix] package manager to create a reproducable
development environment. [Nix] can be installed alongside any other package
manager and will not interfere with system directories. It uses cryptographic
hashes to ensure that all recursive package dependencies are precisely
specified, ensuring reproducability. 

This repository should be usable without [Nix], but the results may vary. You
will need to install the dependencies manually.

The nix files in this repository use a pinned version of nixpkgs for the sake of
reproducability, see `nixpkgs.nix`. You can use the nixpkgs version your system
uses by passing `--arg nixpkgs 'import <nixpkgs>'` to `nix-shell`.

### Implementations

See the `riscv` directory for RISC-V implementations.

[Nix]: https://nixos.org/nix/
