#include "solvers.h"

namespace solvers {
float32 effective_elasticity(float32 e1, float32 e2) {
    return (e1 + e2) / 2.0;
}
std::tuple<V2, V2> velocities_after_collision(
    const Physical_properties& p1, const V2& u1,
    const Physical_properties& p2, const V2& u2) {
    const float32 e = effective_elasticity(p1.elasticity, p2.elasticity);
    const float32 m1{p1.mass}, m2{p2.mass};
    return {
        V2{ (m1 * u1 + m2 * u2 - m2 * e * (u1 - u2)) / (m1 + m2) },
        V2{ (m1 * u1 + m2 * u2 + m1 * e * (u1 - u2)) / (m1 + m2) }
    };
}
}
