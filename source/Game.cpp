#include "Game.h"
#include "game_components.h"

void Game::init(not_null<SDL_Window*> window) {
    int32 w,h;
    SDL_GetWindowSize(window, &w, &h);
    Platform& base = make_child<Platform>();
    base.size = V2{w * 4.f, h / 4.f};
    base.position = V2{0, 0};
    auto& platform = make_child<Platform>();
    platform.size = V2{100, 100};
    platform.position = V2{500, h - h / 4.f -100};
    auto& another = make_child<Platform>();
    another.size = V2{300, 50};
    another.position = V2 {100, h/ 4.f};

    auto& player = make_child<Player>();
    player.position = V2{100, 500};
    player.name = "player";
    player.physical_properties.elasticity = .5f;
}
void Game::update(const milliseconds& delta) {
}
void Game::render(not_null<SDL_Renderer*> renderer, const World& world) const {
}
void Game::fixed_update() {
}
