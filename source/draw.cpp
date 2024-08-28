#include "draw.h"
#include <ranges>
#include "utl.h"
namespace draw {
void fill_circle(SDL_Renderer* renderer, SDL_Point center, int radius) {
    const double overfill_scale{radius / 5.0};
    for (int angle : std::views::iota(0, static_cast<int>(180 * overfill_scale))) {
        constexpr const double to_rad = M_PI / 180.0;
        const float angle_rad = static_cast<float>(angle / overfill_scale * to_rad);
        SDL_FPoint point0 {
            cosf(angle_rad) * radius + center.x,
            sinf(angle_rad)* radius + center.y};
        SDL_FPoint point1 {
            cosf(angle_rad) * radius + center.x,
            sinf(-angle_rad) * radius + center.y};
        SDL_RenderDrawLine(renderer, point0.x, point0.y, point1.x, point1.y);
    }
}
void fill_circle(SDL_Renderer* renderer, const Circle& circle){
    fill_circle(renderer, utl::from_V2(circle.center),
                static_cast<int>(circle.radius)); 
}
void draw_circle(SDL_Renderer* renderer, SDL_Point center, int32_t radius) {
    for (int angle : std::views::iota(0, 180)) {
        constexpr const double to_rad = M_PI / 180.0;
        const float angle_rad = static_cast<float>(angle * to_rad);
        SDL_FPoint point0 {
            cosf(angle_rad) * radius + center.x,
            sinf(angle_rad)* radius + center.y};
        SDL_FPoint point1 {
            cosf(angle_rad) * radius + center.x,
            sinf(-angle_rad) * radius + center.y};
        SDL_RenderDrawPoint(renderer, point0.x, point0.y);
        SDL_RenderDrawPoint(renderer, point1.x, point1.y);
    }
}
}
