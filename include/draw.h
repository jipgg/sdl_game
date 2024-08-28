#pragma once
#include <common.h>
#include "structs.hpp"
namespace draw {
void fill_circle(SDL_Renderer* renderer, SDL_Point center, int radius);
void fill_circle(SDL_Renderer* renderer, const Circle& circle);
void draw_circle(SDL_Renderer* renderer, SDL_Point center, int radius);
}
