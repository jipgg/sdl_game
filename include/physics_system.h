#pragma once
#include "Entity.h"

namespace physics {
constexpr float GRAVITY{1e-2f};
void handle_physical_collisions(std::list<unique_ptr<Entity>>& entities,const milliseconds& delta);
}
