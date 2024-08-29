#pragma once
#include "Entity.h"

namespace physics {
constexpr float GRAVITY{1e-2f};
float effective_elasticity(float e1, float e2);

std::tuple<V2, V2> velocities_after_collision(
    float elasticity, float mass1, const V2& init_vel1, float mass2, const V2& init_vel2);
void handle_physical_collisions(std::list<unique_ptr<Entity>>& entities,const milliseconds& delta);
}
