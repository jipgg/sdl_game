#include "Game.h"
#include "game_components.h"

void Game::init(not_null<SDL_Window*> window) {
    int32 w,h;
    SDL_GetWindowSize(window, &w, &h);
    Block& base = make_child<Block>();
    base.size = V2{w * 4.f, h / 4.f};
    base.physical_properties.welded = true;
    base.position = V2{0, 0};
    auto& platform = make_child<Block>();
    platform.size = V2{100, 100};
    platform.physical_properties.mass = 10;
    platform.position = V2{500, h - h / 4.f -100};
    auto& another = make_child<Block>();
    another.size = V2{300, 50};
    another.position = V2 {100, h/ 4.f};
    auto& backplatform = make_child<Block>();
    backplatform.size = V2{50, static_cast<float>(h)};
    backplatform.physical_properties.welded = true;
    backplatform.position = V2 {-backplatform.size.x, 0};

    auto& player = make_child<Player>();
    player.position = V2{100, 500};
    player.name = "player";
    player.physical_properties.elasticity = .5f;
    player.physical_properties.friction = .1f;
    explorer = make_unique<Explorer>(&root);
}
void Game::update(const milliseconds& delta) {
}
void Game::render(not_null<SDL_Renderer*> renderer, const World& world) const {

}
void Game::fixed_update() {
}
