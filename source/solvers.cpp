#include "solvers.h"

namespace solvers {
double effective_elasticity(double e1, double e2) {
    return (e1 + e2) / 2.0;
}
std::tuple<V2, V2> velocities_after_collision(
    const Physical_properties& p1, const V2& u1,
    const Physical_properties& p2, const V2& u2) {
    const double e = effective_elasticity(p1.elasticity, p2.elasticity);
    const double m1{p1.mass}, m2{p2.mass};
    return {
        V2{ (m1 * u1 + m2 * u2 - m2 * e * (u1 - u2)) / (m1 + m2) },
        V2{ (m1 * u1 + m2 * u2 + m1 * e * (u1 - u2)) / (m1 + m2) }
    };
}
}
