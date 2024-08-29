#pragma once
#include <gsl/gsl-lite.hpp>
#include <common.h>
#include "structs.hpp"
#include "Entity.h"
struct SDL_Renderer;
struct SDL_Window;
struct Game_state {
    View_transform transform;
    std::list<std::unique_ptr<Entity>> entities;
    bool quit{false};
};
namespace game {
void init(V2i window_size, Game_state& game_state);
void update(const milliseconds& delta, Game_state& game_state);
template <Entity_esque T>
T& make_entity(Game_state& state) {
    auto entity = std::make_unique<T>();
    auto it = state.entities.insert(state.entities.end(), std::move(entity));
    return static_cast<T&>(*(*it));
}
}
