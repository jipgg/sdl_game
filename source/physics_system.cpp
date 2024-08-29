#include "physics_system.h"
#include "utl.h"
#include "solvers.h"
#include "entity_derivations.h"
using u_Entity = std::unique_ptr<Entity>; 

namespace physics {
// # pretty scuffed implementation atm, fix it
void handle_physical_collisions(std::list<u_Entity>& entities, const milliseconds& delta) {
    auto physical_filter = std::views::filter([](u_Entity& e){ return e->is_physical;});
    for (u_Entity& e : entities | physical_filter) {
        Physical_entity& obj = static_cast<Physical_entity&>(*e);
        V2 obj_old_pos = obj.position;
        auto [left, right, top, bottom] = obj.collision_points();
        //println(obj.is_falling, obj.name, obj.velocity.y);
        if (obj.welded) {
            continue;
        }
        if (obj.is_falling) {
            obj.acceleration.x = 0;
            obj.acceleration.y = -GRAVITY;
        } else {
            if (fabs(obj.velocity.x) > 0.1f) {
                const int dir = obj.velocity.x > 0.f ? -1 : 1;
                // # should probably change the is_falling to a pointer
                // on which the current obj is standing on to get the
                // effective friction coefficient here
                obj.acceleration.x = dir * obj.friction * GRAVITY;
            } else {
                obj.acceleration.x = 0;
                obj.velocity.x = 0;
            }
        }
        obj.velocity += obj.acceleration * gsl::narrow_cast<float>(delta.count());
        obj.position += obj.velocity * gsl::narrow_cast<float>(delta.count());
        for (u_Entity& f : entities | physical_filter) {
            auto& other = static_cast<Physical_entity&>(*f);
            const Rect other_rect = other.collision_rect();
            if(obj.id == other.id) continue;
            if (utl::is_V2_in_Rect(left, other_rect)) {
                if (obj.velocity.x < 0) {
                    obj.is_obstructed = true;
                    obj.position.x = other.position.x + other.size.x;
                    if (other.welded) {
                        obj.velocity.x = -obj.velocity.x 
                            * obj.elasticity;
                    } else {
                        auto [obj_vel, other_vel] = solvers::velocities_after_collision(obj.elasticity, obj.mass, obj.velocity,
                                                            other.elasticity, other.mass,
                                                            other.velocity);
                        obj.velocity.x = obj_vel.x;
                        other.velocity.x = other_vel.x;
                    }
                } else {
                    obj.is_obstructed = false;
                }
            } else if (utl::is_V2_in_Rect(right, other_rect)) {
                if (obj.velocity.x > 0) {
                    obj.is_obstructed = true;
                    obj.position.x = other.position.x - obj.size.x;
                    if (other.welded) {
                        obj.velocity.x = -obj.velocity.x 
                            * obj.elasticity;
                    } else {
                        auto [obj_vel, other_vel] = solvers::velocities_after_collision(obj.elasticity, obj.mass, obj.velocity,
                                                            other.elasticity, other.mass,
                                                            other.velocity);
                        obj.velocity.x = obj_vel.x;
                        other.velocity.x = other_vel.x;
                    }
                } else {
                    obj.is_obstructed = false;
                }
            }
            if (utl::is_V2_in_Rect(top, other_rect)) {
                obj.is_falling = false;
                if (obj.velocity.y < 0) {
                    obj.position.y = other.position.y + other.size.y;
                    if (other.welded) {
                        obj.velocity.y = -obj.velocity.y 
                            * obj.elasticity;
                    } else {
                        auto [obj_vel, other_vel] = solvers::velocities_after_collision(obj.elasticity, obj.mass,
                                                            obj.velocity,
                                                            other.elasticity, other.mass,
                                                            other.velocity);
                        obj.velocity.y = obj_vel.y;
                        other.velocity.y = other_vel.y;
                    }
                }
            } else if (utl::is_V2_in_Rect(bottom, other_rect)) {
                obj.is_falling = false;
                if (obj.velocity.y > 0) {
                    obj.position.y = other.position.y - obj.size.y;
                    if (other.welded) {
                        obj.velocity.y = -obj.velocity.y 
                            * obj.elasticity;
                    } else {
                        auto [obj_vel, other_vel] = solvers::velocities_after_collision(obj.elasticity, obj.mass,
                                                            obj.velocity,
                                                            other.elasticity, other.mass,
                                                            other.velocity);
                        obj.velocity.y = obj_vel.y;
                        other.velocity.y = other_vel.y;
                    }
                }
            }
        }
    }
}
}
