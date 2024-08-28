#include "entity_derivations.h"
#include "utl.h"

Rect Physical_entity::collision_rect() const {
    return {.x = position.x, .y = position.y,
        .w = size.x, .h = size.y };
}
std::tuple<V2, V2, V2, V2> Physical_entity::collision_points() const {
    V2 left = position + V2{0, size.y / 2};
    V2 right = position + V2{size.x, size.y / 2};
    V2 top = position + V2 {size.x / 2, 0};
    V2 bottom = position + V2{size.x / 2, size.y};
    return {left, right, top, bottom};
    }
// @ Platform
void Block::render(not_null<SDL_Renderer*> renderer, const View_transform& transform) const {
    utl::set_color(renderer, color);
    SDL_Rect r = utl::from_Rect(transform.apply(this->collision_rect()));
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
        velocity.x = direction * move_speed * delta.count();
    }
}
void Player::render(not_null<SDL_Renderer*> renderer, const View_transform& transform) const {
    utl::set_color(renderer, Color{0xff, 0xff, 0xaa, 0xff});
    SDL_Rect r = utl::from_Rect(transform.apply(collision_rect()));
    SDL_RenderFillRect(renderer, &r);
}
