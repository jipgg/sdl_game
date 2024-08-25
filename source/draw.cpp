#include "draw.h"
#include <ranges>
#include "utl.h"
namespace draw {
void fill_circle(SDL_Renderer* renderer, SDL_Point center, int32_t radius) {
    const double overfill_scale{radius / 5.0};
    std::ranges::for_each(std::views::iota(0, gsl::narrow_cast<int>(180 * overfill_scale)),
                     [&renderer, &radius, &center, &overfill_scale](int angle) {
        constexpr const double to_rad = M_PI / 180.0;
        const float angle_rad = gsl::narrow_cast<float>(angle / overfill_scale * to_rad);
        SDL_FPoint point0 {
            cosf(angle_rad) * radius + center.x,
            sinf(angle_rad)* radius + center.y};
        SDL_FPoint point1 {
            cosf(angle_rad) * radius + center.x,
            sinf(-angle_rad) * radius + center.y};
        SDL_RenderDrawLine(renderer, point0.x, point0.y, point1.x, point1.y);
    });
}
void fill_circle(SDL_Renderer* renderer, const Circle& circle){
    fill_circle(renderer, utl::from_vector2(circle.center),
                narrow_cast<int32>(circle.radius)); 
}
void draw_circle(SDL_Renderer* renderer, SDL_Point center, int32_t radius) {
    std::ranges::for_each(std::views::iota(0, 180),
                     [&renderer, &radius, &center](int angle) {
        constexpr const double to_rad = M_PI / 180.0;
        const float angle_rad = gsl::narrow_cast<float>(angle * to_rad);
        SDL_FPoint point0 {
            cosf(angle_rad) * radius + center.x,
            sinf(angle_rad)* radius + center.y};
        SDL_FPoint point1 {
            cosf(angle_rad) * radius + center.x,
            sinf(-angle_rad) * radius + center.y};
        SDL_RenderDrawPoint(renderer, point0.x, point0.y);
        SDL_RenderDrawPoint(renderer, point1.x, point1.y);
    });
}
}
