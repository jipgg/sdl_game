#pragma once
#include <gsl/gsl-lite.hpp>
#include <common.h>
struct SDL_Renderer;
struct SDL_Window;
struct Game_state;
namespace game {
void init(V2i window_size, gsl::not_null<Game_state*> game_state);
void update(const milliseconds& delta, gsl::not_null<Game_state*> game_state);
}
