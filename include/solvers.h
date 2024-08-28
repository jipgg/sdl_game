#pragma once
#include <common.h>

namespace solvers {
float effective_elasticity(float e1, float e2);
std::tuple<V2, V2> velocities_after_collision(
    float e1, float m1, const V2& u1, 
    float e2, float m2, const V2& u2);
}
