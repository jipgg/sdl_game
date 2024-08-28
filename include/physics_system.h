#pragma once
#include "Entity.h"

namespace physics {
void handle_physical_collisions(std::list<unique_ptr<Entity>>& entities,const milliseconds& delta, float gravity);
}
