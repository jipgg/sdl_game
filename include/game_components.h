#pragma once
#include "Game.h"
#include "structs.h"
#include "Event.hpp"
// @ Physical
struct Physical {
public:
    virtual ~Physical() = default;
    Physical_properties physical_properties;
    using Pos_changed_event = Event<const V2&>;
    std::shared_ptr<Pos_changed_event> position_changed{
        make_shared<Event<const V2&>>()};
    V2 position;
    V2 velocity;
    bool is_falling{true};
    bool is_obstructed{false};
    V2 acceleration;
    V2 size;
    Rect collision_rect() const;
    std::tuple<V2, V2, V2, V2> collision_points() const;
};
// @ Game_component
class Game_component: public Component<Game>, public Physical {
public:
    Game_component(const std::type_info& _info, Game& _root):
        Component{_info, _root} {}
};
// @ Platform
class Platform: public Game_component {
public:
    Platform(Game& root): Game_component{typeid(Platform), root} {
        physical_properties.is_welded = true;}
    void render(SDL_Renderer* renderer, const View_transform& transform) const override;
};
// @ Player
class Player: public Game_component {
public:
    Player(Game& root): Game_component{typeid(Player), root} {
        size = V2{50, 50};
    }
    void update(std::chrono::milliseconds delta) override;
    void render(SDL_Renderer* renderer, const View_transform& transform) const override;
    double move_speed{.2};
    double elapsed{0};
    double accelerate_coefficient{.001};
    double decelerate_coefficient{.001};
    double jump_power{3};
};
// @ Variable
template <class T>
class Variable : public Component<Game> {
public:
    T value;
    Variable(T&& _value): value{std::move(_value)} {}
};
