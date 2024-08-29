#include "Entity.h"
Entity::Id Entity::add(std::unique_ptr<Entity> component) {
    Id id = component->id;
    component->owner = this;
    components.insert(std::pair{id, std::move(component)});
    return id;
}
void Entity::remove(Id id) {
    components.erase(id);
}

