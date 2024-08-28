#include "game_logic.h"
#include "entity_derivations.h"
#include <SDL.h>

namespace game {
void init(V2i window_size, gsl::not_null<Game_state *> game_state) {
    auto block = std::make_unique<Block>();
    block->size = V2{window_size.x * 4.f, window_size.y / 4.f};
    block->welded = true;
    block->position = V2{0, 0};
    block->name = "block";
    game_state->entities.push_back(std::move(block));
    auto player = std::make_unique<Player>();
    player->size = V2{50, 50};
    player->position = V2{100, 300};
    player->name = "player";
    player->friction = .5f;
    player->welded = false;
    game_state->entities.push_back(std::move(player));
}
void update(const milliseconds& delta, gsl::not_null<Game_state*> game_state) {

}

}
