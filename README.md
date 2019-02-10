## Ascon in RISC-V

This repository will contain an implementation of
[Ascon](https://competitions.cr.yp.to/round3/asconv12.pdf) for RISC-V,
as well as a paper describing it.

### Compiling

The code in this repository was written to run on the
[HiFive1](https://www.sifive.com/boards/hifive1).

This repository uses [Nix](https://nixos.org/nix/), the functional package
manager in order to create a reliable development environment. After installing
Nix, you can run `nix-shell` in the root of this repository to enter a shell
with all required dependencies. You can then run `make` to build the project.
Run `upload foo.elf` to flash `foo.elf` on the HiFive1.

### Testing

To be implemented.
