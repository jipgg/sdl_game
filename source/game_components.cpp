#include "game_components.h"
#include "utl.h"
// @ Physical
Rect Physical::collision_rect() const {
    return {.x = position.x, .y = position.y,
        .w = size.x, .h = size.y };
}
std::tuple<Vector2, Vector2, Vector2, Vector2> Physical::collision_points() const {
    Vector2 left = position + Vector2{0, size.y / 2};
    Vector2 right = position + Vector2{size.x, size.y / 2};
    Vector2 top = position + Vector2 {size.x / 2, 0};
    Vector2 bottom = position + Vector2{size.x / 2, size.y};
    return {left, right, top, bottom};
    }
// @ Platform
void Platform::render(SDL_Renderer* renderer, const View_transform& transform) const {
    utl::set_color(renderer, Color{100, 150, 100, 0});
    SDL_Rect r = utl::from_Rect(transform.apply(this->collision_rect()));
    SDL_RenderFillRect(renderer, &r);
}
// @ Player
void Player::update(std::chrono::milliseconds delta) {
    const uint8_t* key_states = SDL_GetKeyboardState(nullptr);
    int direction{0};
    if (key_states[SDL_SCANCODE_D]) {
        ++direction;}
    if (key_states[SDL_SCANCODE_A]) {--direction;}
    if (key_states[SDL_SCANCODE_SPACE]) {
        if (not is_falling) {
            is_falling = true;
            velocity.y = jump_power;
        }
    };
    if (is_falling) { return;}
    if (direction) {
        if (not is_obstructed) {
            elapsed += accelerate_coefficient * delta.count();
        }
    } else {
        direction = velocity.x >= 0 ? 1 : -1;
        if (not is_obstructed) {
            elapsed -= decelerate_coefficient * delta.count();
        }
    } 
    elapsed = std::clamp(elapsed, 0.0, 1.0);
    velocity.x = direction * elapsed * move_speed * delta.count();
}
void Player::render(SDL_Renderer* renderer, const View_transform& transform) const {
    utl::set_color(renderer, Color{0xff, 0xff, 0xaa, 0xff});
    //draw::fill_circle(renderer, utl::from_vector2(world.apply(position)), size.x);
    SDL_Rect r = utl::from_Rect(transform.apply(this->collision_rect()));
    SDL_RenderFillRect(renderer, &r);
}
