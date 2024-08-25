#pragma once
#include <common.h>
#include "structs.h"

namespace solvers {
double effective_elasticity(double e1, double e2);
std::tuple<V2, V2> velocities_after_collision(
    const Physical_properties& p1, const V2& u1, 
    const Physical_properties& p2, const V2& u2);
}
