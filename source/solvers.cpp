#include "solvers.h"

namespace solvers {
float effective_elasticity(float e1, float e2) {
    return (e1 + e2) / 2.0;
}
std::tuple<V2, V2> velocities_after_collision(
    float  e1, float m1, const V2& u1,
    float e2, float m2, const V2& u2) {
    const float e = effective_elasticity(e1, e2);
    return {
        V2{ (m1 * u1 + m2 * u2 - m2 * e * (u1 - u2)) / (m1 + m2) },
        V2{ (m1 * u1 + m2 * u2 + m1 * e * (u1 - u2)) / (m1 + m2) }
    };
}
}
