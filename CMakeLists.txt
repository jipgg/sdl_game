cmake_minimum_required(VERSION 3.22.2)
project(game)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(imgui CONFIG REQUIRED)

file(GLOB SOURCE_FILES "${CMAKE_SOURCE_DIR}/source/*.cpp")

add_executable(game WIN32 "${SOURCE_FILES}")
target_include_directories(game PRIVATE "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/extern")

target_link_options(game PRIVATE /NODEFAULTLIB:msvcrt.lib)
target_link_libraries(game PRIVATE SDL2::SDL2main)
target_link_libraries(game PRIVATE SDL2::SDL2)
target_link_libraries(game PRIVATE SDL2_ttf::SDL2_ttf)
target_link_libraries(game PRIVATE SDL2_image::SDL2_image)
target_link_libraries(game PRIVATE imgui::imgui)
