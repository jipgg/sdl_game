// common definitions and include paths
#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <gsl/gsl-lite.hpp>
#include <SDL_ttf.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include "templated/Vector2.hpp"
#include <stdint.h>
#include <string>
using std::string;
#include <string_view>
using std::string_view;
#include <chrono>
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::duration_cast;
using namespace std::chrono_literals;
#include <memory>
using std::unique_ptr;
using std::make_unique;
#define USE_MATH_DEFINES
#include <math.h>
using gsl::not_null;
using gsl::czstring;
using gsl::narrow_cast;
using V2 = templated::Vector2<float>;
using V2i = templated::Vector2<int>;
