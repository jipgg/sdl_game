// common definitions and include paths
#pragma once
#include <gsl/assert>
#include <gsl/pointers>
#include <gsl/zstring>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <gsl/narrow>
#include <stdint.h>
#include <chrono>
#include <memory>
#include <ranges>
#include <unordered_map>
#include <vector>
#include <string>
#include <string_view>
#include <tuple>
#define USE_MATH_DEFINES
#include <math.h>
namespace ranges = std::ranges;
namespace views = std::views;
using gsl::narrow_cast;
using gsl::czstring;
using gsl::not_null;
using namespace std::chrono_literals;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::duration_cast;
using std::chrono::duration;
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
using uint64 = uint16_t;
using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
