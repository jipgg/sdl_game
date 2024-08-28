#include "collision_logic.h"
#include "utl.h"
#include "solvers.h"

namespace collision_logic {
// # pretty scuffed implementation atm, fix it
void handle_physical_collisions(game_component_view children_v, const milliseconds& delta, const World& world) {
    using u_component= std::unique_ptr<Component<Game>>;
    for (u_component& e : children_v) {
        auto& obj = static_cast<Game_component&>(*e);
        Physical_properties& obj_props{obj.physical_properties};
        int64 delta_ms = delta.count();
        V2 obj_old_pos = obj.position;
        auto [left, right, top, bottom] = obj.collision_points();
        if (obj_props.welded) {
            continue;
        }
        if (obj.is_falling) {
            obj.acceleration.x = 0;
            obj.acceleration.y = -world.gravity;
        } else {
            if (fabs(obj.velocity.x) > 0.1f) {
                const int8 dir = obj.velocity.x > 0.f ? -1 : 1;
                // # should probably change the is_falling to a pointer
                // on which the current obj is standing on to get the
                // effective friction coefficient here
                obj.acceleration.x = dir * obj_props.friction * world.gravity;
            } else {
                obj.acceleration.x = 0;
                obj.velocity.x = 0;
            }
        }
        obj.velocity += obj.acceleration * narrow_cast<float32>(delta_ms);
        obj.position += obj.velocity * narrow_cast<float32>(delta_ms);
        for (u_component& f : children_v) {
            auto& other = static_cast<Game_component&>(*f);
            const Rect other_rect = other.collision_rect();
            if(obj.id == other.id) continue;
            Physical_properties& other_props{other.physical_properties};
            if (utl::is_V2_in_Rect(left, other_rect)) {
                if (obj.velocity.x < 0) {
                    obj.is_obstructed = true;
                    obj.position.x = other.position.x + other.size.x;
                    if (other_props.welded) {
                        obj.velocity.x = -obj.velocity.x 
                            * obj.physical_properties.elasticity;
                    } else {
                        auto [obj_vel, other_vel] = solvers::velocities_after_collision(obj_props,
                                                            obj.velocity,
                                                            other_props,
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
                    if (other_props.welded) {
                        obj.velocity.x = -obj.velocity.x 
                            * obj.physical_properties.elasticity;
                    } else {
                        auto [obj_vel, other_vel] = solvers::velocities_after_collision(obj_props,
                                                            obj.velocity,
                                                            other_props,
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
                    if (other_props.welded) {
                        obj.velocity.y = -obj.velocity.y 
                            * obj.physical_properties.elasticity;
                    } else {
                        auto [obj_vel, other_vel] = solvers::velocities_after_collision(obj_props,
                                                            obj.velocity,
                                                            other_props,
                                                            other.velocity);
                        obj.velocity.y = obj_vel.y;
                        other.velocity.y = other_vel.y;
                    }
                }
            } else if (utl::is_V2_in_Rect(bottom, other_rect)) {
                obj.is_falling = false;
                if (obj.velocity.y > 0) {
                    obj.position.y = other.position.y - obj.size.y;
                    if (other.physical_properties.welded) {
                        obj.velocity.y = -obj.velocity.y 
                            * obj.physical_properties.elasticity;
                    } else {
                        auto [obj_vel, other_vel] = solvers::velocities_after_collision(obj_props,
                                                            obj.velocity,
                                                            other_props,
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
