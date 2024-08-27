#include "collision_logic.h"

namespace collision_logic {
void handle_physical_collisions(game_component_view children_v, const milliseconds& delta, const View_transform& view_transform) {
    using u_component= std::unique_ptr<Component<Game>>;
    for (u_component& e : children_v) {
        auto& obj = static_cast<Game_component&>(*e);
        Physical_properties& obj_props{obj.physical_properties};
        int64 delta_ms = delta.count();
        V2 obj_old_pos = obj.position;
        auto [left, right, top, bottom] = obj.collision_points();
        if (obj_props.is_welded) {
            continue; }
        if (obj.is_falling) {
            //obj.acceleration.y = view_transform.scaling.y * obj.root.gravity;
            obj.acceleration.y = -obj.root.gravity;
        }
        obj.velocity += obj.acceleration * gsl::narrow_cast<float>(delta_ms);
        obj.position += obj.velocity * gsl::narrow_cast<float>(delta_ms);
        for (u_component& f : children_v) {
            auto& other = static_cast<Game_component&>(*f);
            const Rect other_rect = other.collision_rect();
            if(obj.id == other.id) continue;
            Physical_properties& other_props{other.physical_properties};
            if (utl::is_V2_in_Rect(left, other_rect)) {
                if (obj.velocity.x < 0) {
                    obj.is_obstructed = true;
                    obj.position.x = other.position.x + other.size.x;
                    if (other_props.is_welded) {
                        obj.velocity.x = -obj.velocity.x 
                            * obj.physical_properties.elasticity;
                    }
                } else {
                    obj.is_obstructed = false;
                }
            } else if (utl::is_V2_in_Rect(right, other_rect)) {
                if (obj.velocity.x > 0) {
                    obj.is_obstructed = true;
                    obj.position.x = other.position.x - obj.size.x;
                    if (other_props.is_welded) {
                        obj.velocity.x = -obj.velocity.x 
                            * obj.physical_properties.elasticity;
                    }
                } else {
                    obj.is_obstructed = false;
                }
            }
            if (utl::is_V2_in_Rect(top, other_rect)) {
                obj.is_falling = false;
                if (obj.velocity.y < 0) {
                    obj.position.y = other.position.y + other.size.y;
                    if (other_props.is_welded) {
                        obj.velocity.y = -obj.velocity.y 
                            * obj.physical_properties.elasticity;
                    }
                }
            } else if (utl::is_V2_in_Rect(bottom, other_rect)) {
                obj.is_falling = false;
                if (obj.velocity.y > 0) {
                    obj.position.y = other.position.y - obj.size.y;
                    if (other.physical_properties.is_welded) {
                        obj.velocity.y = -obj.velocity.y 
                            * obj.physical_properties.elasticity;
                    }
                }
            }
        }
    }
}
}
