# sdl_game
## summary
simple 2D game engine made with [SDL](https://www.libsdl.org/)
and [ImGui](https://github.com/ocornut/imgui) for dev-tooling.
## status
not finished
## to build:
when using vcpkg: change the `CMAKE_TOOLCHAIN_FILE` definitions in `CMakePresets.json` to your vcpkg path.
then run:
```sh
cmake --preset ninja .
cmake --build build/ninja --config debug
#or
cmake --preset msvc .
cmake --build build/msvc --config debug
```

