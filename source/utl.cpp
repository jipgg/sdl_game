#include "utl.h"
#include "print.hpp"
SDL_Rect operator*(const SDL_Rect& rect, float scalar) {
    return {
        .x = int(rect.x * scalar), 
        .y = int(rect.y * scalar),
        .w = int(rect.w * scalar), 
        .h = int(rect.h * scalar)};
}
SDL_FPoint operator*(const SDL_FPoint& a, const SDL_FPoint& b) {
    return {.x = a.x + b.x, .y = a.y + b.y };
}
SDL_FPoint operator*(const SDL_FPoint& a, float scalar) {
    return {.x = a.x * scalar, .y = a.y * scalar};
}
SDL_FPoint& operator+=(SDL_FPoint& a, const SDL_FPoint& b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}
namespace utl {
bool is_overlapping(const Rect &r1, const Rect &r2) {
// If one rectangle is on left side of the other
	if ( ( r1.x + r1.w ) < r2.x or ( r2.x + r2.w ) < r1.x ) {
		return false;}
	// If one rectangle is under the other
	if ( r1.y > ( r2.y + r2.h ) or r2.y > ( r1.y + r1.h ) ) {
		return false;}
	return true;
}
bool is_V2_in_Rect(const V2& vector, const Rect& rect) {
    bool in_bounds_x = vector.x >= rect.x and vector.x <= rect.x + rect.w;
    bool in_bounds_y = vector.y >= rect.y and vector.y <= rect.y + rect.h;
    return in_bounds_x and in_bounds_y;
}
SDL_Rect from_Rect(const Rect& rect) {
    SDL_Rect r{ static_cast<int>(rect.x),
        static_cast<int>(rect.y),
        static_cast<int>(rect.w),
        static_cast<int>(rect.h)
    };
    return r;
}
void set_color(SDL_Renderer* renderer, Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}
SDL_Point from_V2(const V2& v) {
    return {static_cast<int>(v.x), static_cast<int>(v.y)};
}
unique_dtor_ptr<SDL_Window> make_SDL_window(gsl::czstring title, SDL_Rect rect, uint32_t flags) {
    SDL_Window* window = SDL_CreateWindow(title, rect.x, rect.y, rect.w, rect.h, flags);
    if (not window) {
        prerrln(SDL_GetError()); }
    return unique_dtor_ptr<SDL_Window>{window, SDL_DestroyWindow};
}
unique_dtor_ptr<SDL_Renderer> make_SDL_renderer(SDL_Window* window, int index, uint32_t flags) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, index, flags);
    if (not renderer) {
        prerrln(SDL_GetError()); }
    return unique_dtor_ptr<SDL_Renderer>{renderer, SDL_DestroyRenderer};
}
unique_dtor_ptr<TTF_Font> make_SDL_font(gsl::czstring filename, int ptsize) {
    TTF_Font* font = TTF_OpenFont(filename, ptsize);
    if (not font) {
        prerrln(SDL_GetError()); }
    return unique_dtor_ptr<TTF_Font>{font, TTF_CloseFont};
}
unique_dtor_ptr<SDL_Surface> make_SDL_surface_text_solid(TTF_Font* font, gsl::czstring text, SDL_Color fg) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, fg);
    if (not surface) {
        prerrln(SDL_GetError()); }
    return unique_dtor_ptr<SDL_Surface>{surface, SDL_FreeSurface};
}
unique_dtor_ptr<SDL_Surface> make_SDL_surface_text_blended(TTF_Font* font, gsl::czstring text, SDL_Color fg) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, fg);
    if (not surface) {
        prerrln(SDL_GetError()); }
    return unique_dtor_ptr<SDL_Surface>{surface, SDL_FreeSurface };
}
unique_dtor_ptr<SDL_Texture> make_SDL_texture_from_surface(SDL_Renderer* renderer, SDL_Surface* surface) {
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (not texture) {
        prerrln(SDL_GetError()); }
    return unique_dtor_ptr<SDL_Texture>{texture, SDL_DestroyTexture};
}
unique_dtor_ptr<SDL_Surface> make_SDL_surface_from_img(gsl::czstring imgpath) {
    SDL_Surface* surface = IMG_Load(imgpath);
    return unique_dtor_ptr<SDL_Surface>(surface, SDL_FreeSurface);
}
unique_dtor_ptr<SDL_Texture> load_SDL_img_texture(SDL_Renderer* renderer, gsl::czstring imgpath) {
    auto surface = make_SDL_surface_from_img(imgpath);
    return make_SDL_texture_from_surface(renderer, surface.get());
}
using std::ranges::iota_view;
std::unordered_map<char, Texture_bundle> create_char_texture_cache(SDL_Renderer* renderer, iota_view<char, char> char_range,
                                                                   TTF_Font* font, SDL_Color color) {
    std::unordered_map<char, Texture_bundle> map{};
    std::string converter{};
    for (char character : char_range) {
        converter = character;
        auto surface = make_SDL_surface_text_blended(font,converter.c_str(), color);
        auto texture = make_SDL_texture_from_surface(renderer,surface.get());
        map.insert(std::make_pair(character, Texture_bundle{
            .texture = std::move(texture),.w = surface->w, .h = surface->h }));
    }
    return map;
}
void render_text(const char_texture_cache& cache, SDL_Renderer* renderer, std::string_view text, SDL_Point offset) {
    for (char character : text) {
        Expects(cache.find(character) != cache.end());
        const Texture_bundle& bundle = cache.at(character);
        SDL_Rect dest{.x = offset.x, .y = offset.y, .w = bundle.w, .h = bundle.h};
        SDL_RenderCopy(renderer, bundle.texture.get(), nullptr, &dest);
        offset.x += dest.w;
    }
}
}
