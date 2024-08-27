#pragma once
#include "game_components.h"

namespace collision_logic {
using game_component_view = decltype(Game_component::children | std::views::values);
void handle_physical_collisions(game_component_view view, const milliseconds& delta, const View_transform& view_transform);
}
