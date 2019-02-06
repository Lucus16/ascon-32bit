let
  riscv32-embedded = import <nixpkgs> {
    crossSystem = {
      config = "riscv32-elf";
      libc = "newlib";

      platform = {
        bfdEmulation = "elf32lriscv";
      };
    };
  };

in riscv32-embedded
