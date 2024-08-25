# sdl_game
## summary
simple 2D game engine made with [SDL](https://www.libsdl.org/)
and [ImGui](https://github.com/ocornut/imgui) for dev-tooling.
## status
not finished
## dependencies
check `CMakeLists.txt`
## build cmd:
```ps1
md build
cd build
cmake ..
#when using vcpkg: cmake -DCMAKE_TOOL_CHAIN_FILE=C:/my/path/to/vcpkg.cmake ..
cmake --build . --target game --config Debug
```

