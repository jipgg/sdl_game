#pragma once
#include "Entity.h"
#include "common.h"
// @ Physical_entity
class Physical_entity: public Entity {
public:
    Physical_entity(Entity* owner = nullptr): Entity(owner, true) {}
    V2 position;
    V2 velocity;
    bool is_falling{true};
    bool is_obstructed{false};
    bool welded{false};
    bool collidable{true};
    float elasticity{0.5f};
    float mass{100};
    float friction{.3f};
    V2 acceleration;
    V2 size;
    Rect collision_rect() const;
    std::tuple<V2, V2, V2, V2> collision_points() const;
};
// @ Platform
class Block: public Physical_entity {
public:
    Block(): Physical_entity(nullptr) {}
    void render(not_null<SDL_Renderer*> renderer, const View_transform& transform) const override;
    Color color{180, 180, 180, 180};
};
// @ Player
class Player: public Physical_entity {
public:
    Player(): Physical_entity(nullptr) {}
    double move_speed{.2f};
    double jump_power{3.f};
    void update(const milliseconds& delta) override;
    void render(not_null<SDL_Renderer*> renderer, const View_transform& transform) const override;
};
struct Game_state {
    View_transform transform;
    std::list<unique_ptr<Entity>> entities;
};
