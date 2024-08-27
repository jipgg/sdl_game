// common definitions and include paths
#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <gsl/gsl-lite.hpp>
#include <SDL_ttf.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <stdint.h>
#include <chrono>
#include <memory>
#include <string>
#include <string_view>
#define USE_MATH_DEFINES
#include <math.h>
using namespace std::chrono_literals;
namespace chrono = std::chrono;
using chrono::milliseconds;
using chrono::duration_cast;
using chrono::duration;
using std::string;
using std::to_string;
using std::string_view;
using namespace std::string_literals;
template <class T>
using w_ptr = std::weak_ptr<T>;
template <class T>
using s_ptr = std::shared_ptr<T>;
using std::make_shared;
template <class T>
using u_ptr = std::unique_ptr<T>;
using std::make_unique;
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;
using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
using float32 = float;
using float64 = double;
