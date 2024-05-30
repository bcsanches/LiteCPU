# LiteCPU
A simple 6502 emulator based on [Ben Eater](https://eater.net/6502) 6502 computer project.

The aim of this project is to be able to run Ben Eater binaries on a simple emulator. 

This does not aims to be a perfect emulator for a 6502 CPU (and I do not expect to support all OPCODES) from it.

## Dependencies

This code needs the following libraries to be built (integrated as submodules on contrib directory):

- [Dear ImGui][1]
- [Fmt][2]
- [magicenum][3]
- [SDL][4]
- [spdlog][5]

## License

All code is licensed under the [MPLv2 License][6].

[1]: https://github.com/ocornut/imgui
[2]: https://github.com/fmtlib/fmt
[3]: https://github.com/Neargye/magic_enum
[4]: https://github.com/libsdl-org/SDL
[5]: https://github.com/gabime/spdlog
[6]: https://choosealicense.com/licenses/mpl-2.0/