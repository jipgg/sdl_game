#include "game_components.h"
#include "utl.h"
// @ Physical
Rect Physical::collision_rect() const {
    return {.x = position.x, .y = position.y,
        .w = size.x, .h = size.y };
}
std::tuple<V2, V2, V2, V2> Physical::collision_points() const {
    V2 left = position + V2{0, size.y / 2};
    V2 right = position + V2{size.x, size.y / 2};
    V2 top = position + V2 {size.x / 2, 0};
    V2 bottom = position + V2{size.x / 2, size.y};
    return {left, right, top, bottom};
    }
// @ Platform
void Platform::render(not_null<SDL_Renderer*> renderer, const World& world) const {
    utl::set_color(renderer, Color{100, 150, 100, 0});
    SDL_Rect r = utl::from_Rect(world.transform.apply(this->collision_rect()));
    SDL_RenderFillRect(renderer, &r);
}
// @ Player
void Player::update(const milliseconds& delta) {
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
void Player::render(not_null<SDL_Renderer*> renderer, const World& world) const {
    utl::set_color(renderer, Color{0xff, 0xff, 0xaa, 0xff});
    SDL_Rect r = utl::from_Rect(world.transform.apply(collision_rect()));
    SDL_RenderFillRect(renderer, &r);
}
