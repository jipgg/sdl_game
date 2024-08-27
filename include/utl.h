#pragma once
#include <common.h>
#include "structs.h"
#include <unordered_map>
#include <ranges>
SDL_Rect operator*(const SDL_Rect& rect, float scalar);
SDL_FPoint operator+(const SDL_FPoint& a, const SDL_FPoint& b);
SDL_FPoint& operator+=(SDL_FPoint& a, const SDL_FPoint& b);
SDL_FPoint operator*(const SDL_FPoint& a, float scalar);

namespace utl {
bool is_overlapping(const Rect& a, const Rect& b);
bool is_V2_in_Rect(const V2& p, const Rect& r);
SDL_Rect from_Rect(const Rect& rect);
void set_color(SDL_Renderer* renderer, Color color);
//raii resource acquisition
SDL_Point from_V2(const V2& v);
template<class T>
using  unique_dtor_ptr = std::unique_ptr<T, void(*)(T*)>;
unique_dtor_ptr<SDL_Window> make_SDL_window(gsl::czstring title, SDL_Rect rect, Uint32 flags);
unique_dtor_ptr<SDL_Renderer> make_SDL_renderer(SDL_Window* window, int index, uint32 flags);
unique_dtor_ptr<TTF_Font> make_SDL_font(gsl::czstring filename, int ptsize);
unique_dtor_ptr<SDL_Surface> make_SDL_surface_text_solid(TTF_Font* font, gsl::czstring text, SDL_Color fg);
unique_dtor_ptr<SDL_Texture> make_SDL_texture_from_surface(SDL_Renderer* renderer, SDL_Surface* surface);
unique_dtor_ptr<SDL_Surface> make_SDL_surface_text_blended(TTF_Font* font, gsl::czstring text, SDL_Color fg);
unique_dtor_ptr<SDL_Surface> make_SDL_surface_from_img(gsl::czstring imgpath);
unique_dtor_ptr<SDL_Texture> load_SDL_img_texture(SDL_Renderer* renderer, gsl::czstring imgpath);
using SDL_Window_owner /* res: resource*/ = unique_dtor_ptr<SDL_Window>;
using SDL_Renderer_owner = unique_dtor_ptr<SDL_Renderer>;
using SDL_Font_owner = unique_dtor_ptr<TTF_Font>;
using SDL_Surface_owner = unique_dtor_ptr<SDL_Surface>;
using SDL_Texture_owner = unique_dtor_ptr<SDL_Texture>;
//util
struct Texture_bundle {
    unique_dtor_ptr<SDL_Texture> texture;
    int32 w;
    int32 h;
};
using char_texture_cache = std::unordered_map<char, Texture_bundle>;
std::unordered_map<char, Texture_bundle> create_char_texture_cache(
    SDL_Renderer* renderer,
    std::ranges::iota_view<char, char> char_range,
    TTF_Font* font,
    SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF});
void render_text(const char_texture_cache& cache, SDL_Renderer* renderer, std::string_view text, SDL_Point origin);
}
