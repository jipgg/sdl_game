# sdl_game
## summary
simple 2D game engine made with [SDL](https://www.libsdl.org/)
and [ImGui](https://github.com/ocornut/imgui) for dev-tooling.
## status
not finished
## to build:
If you are using vcpkg:
to automatically install all necessary dependencies;
rename `.vcpkg.json` to `vcpkg.json`.
CMakePresets.json presumes an enviroment variable `VCPKG_ROOT` is defined.
```cmd
cmake --preset ninja .
cmake --build build/ninja --config debug
#or
cmake --preset msvc .
cmake --build build/msvc --config debug
```

