#pragma once
#include <common.h>
#include "structs.h"
namespace draw {
void fill_circle(SDL_Renderer* renderer, SDL_Point center, int32_t radius);
void fill_circle(SDL_Renderer* renderer, const Circle& circle);
void draw_circle(SDL_Renderer* renderer, SDL_Point center, int32_t radius);
}
