#include "Entity.h"
void Entity::add(std::unique_ptr<Entity> component) {
    component->owner = this;
    components.insert(std::pair{component->id, std::move(component)});
}
void Entity::remove(Id id) {
    components.erase(id);
}

