#pragma once
#include "structs.hpp"
#include <unordered_map>
#include <string>
class Entity;
template <class T>
concept Entity_esque = std::is_base_of_v<Entity, T>;  
class Entity {
public:
    using Id = uint32_t;
    Entity(Entity* owner = nullptr, bool is_physical = false):
        id{curr_id_gift++}, owner{owner}, is_physical(is_physical) {}
    const uint32_t id;
    std::string name{"entity"};
    virtual ~Entity() = default;
    void add(std::unique_ptr<Entity> component);
    void remove(Id  id);
    template <Entity_esque T>
    T& use(Id id) {
        return static_cast<T&>(*components.at(id));
    }
    const bool is_physical{false};
    virtual void update(const milliseconds& delta) {};
    virtual void render(not_null<SDL_Renderer*> renderer, const View_transform& transform) const {}
private:
    Entity* owner;
    inline static uint32_t curr_id_gift{0}; 
    std::unordered_map<Id, unique_ptr<Entity>> components;
};
