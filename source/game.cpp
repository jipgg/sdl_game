#include "game.h"
#include "entity_derivations.h"
#include <SDL.h>

namespace game {
void init(V2i window_size, Game_state& state) {
    Block& block = make_entity<Block>(state);
    block.size = V2{window_size.x * 4.f, window_size.y / 4.f};
    block.welded = true;
    block.position = V2{0, 0};
    block.name = "block";
    block.elasticity = 0;
    Block& barrier = make_entity<Block>(state);
    barrier.size = V2{100, window_size.y * 1.5f};
    barrier.position = V2{-100, 0};
    barrier.welded = true;
    barrier.elasticity = 0.f;
    Player& player = make_entity<Player>(state);
    player.size = V2{50, 50};
    player.move_speed = .15f;
    player.position = V2{100, 300};
    player.name = "player";
    player.friction = .6f;
    player.elasticity = .6f;
    player.welded = false;
    Block& other_block = make_entity<Block>(state);
    other_block.size = V2{100, 50};
    other_block.position = V2{200, 300};
    other_block.name = "other block";
    other_block.friction = .5f;
    other_block.welded = false;
    auto other_block2 = std::make_unique<Block>();
    other_block2->size = V2{100, 100};
    other_block2->position = V2{600, 300};
    other_block2->name = "other block";
    other_block2->friction = .1f;
    other_block2->mass = 500.f;
    other_block2->welded = false;
    state.entities.push_back(std::move(other_block2));

    Entity::Id id = player.add(std::make_unique<Block>());
    player.use<Block>(id).size = V2{100, 100};
    player.use<Block>(id).position = V2{100, 100};
    player.use<Block>(id).welded = true;
    player.use<Block>(id).color = Color{255, 0, 0, 255};
}
void update(const milliseconds& delta, Game_state& game_state) {

}

}
