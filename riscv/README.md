# Ascon in RISC-V

This directory contains implementations of the Ascon authenticated encryption
cipher for the SiFive HiFive1 RISC-V development board.

### Dependencies

The following dependencies can be most easily installed with the [Nix] package
manager: If [Nix] is installed, run `nix-shell` to enter a shell session with
all dependencies available. On the first run, this may take some time as the
dependencies are built. Otherwise, take the following steps:

1. Install a version of the gcc toolchain with RISC-V support.
2. Install a version of openocd with RISC-V support for uploading to the board.
3. Install a recent version of Python 3 for generating the test vector headers.
4. Install screen to be able to view the output of board.
5. Set the `PREFIX` environment variable depending on the names of the installed
   gcc toolchain.

### Building

Nix may need to build some of these dependencies the first time you do this. In
the shell session, run `make` to build `build/ascon128-ref.elf`. The makefile
accepts parameters: `IMPL` can be `ref`, `opt32` or `opt64` and `ASCON` can be
`ascon128` or `ascon128a`. For example, `make IMPL=opt32 ASCON=ascon128a` will
build `build/ascon128a-opt32.elf`, which uses a reference implementation of the
alternate version of Ascon which is optimized for 32-bit microcontrollers.

### Testing

In order to run the following commands, you may need to configure some
permissions on your machine: You may need to add yourself to the `dialout` or
`uucp` group and you may need to add a line like the following to your udev
rules, which will match the SiFive HiFive1 USB interface and allow the group
`dialout` to connect with it:

```
ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6010", GROUP="dialout"
```

Connect the board using a micro USB cable. Then, run `screen /dev/ttyUSB1
115200` in one terminal to show any output from the RISC-V board, and in another
terminal, run `upload build/<name>.elf` for one of the ELF files built in the
`build` directory. The `upload` script is defined in `shell.nix` and will be in
your PATH after entering `nix-shell`. In case you don't have [Nix] available,
you can use `./upload.sh` instead. After uploading, the tests should be run
immediately and you can see the results in the first terminal.

[Nix]: https://nixos.org/nix/
