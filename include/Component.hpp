#pragma once 
#include <common.h>
#include "structs.h"
template<class Base, class Derived>
concept Has_base = std::is_base_of_v<Base, Derived>;
template <class T> class Component;
template <class T, class Derived>
concept Composable = std::is_base_of_v<Component<T>, Derived>;
template <class Root>
class Component {
public:
    using Id = uint32_t; // dont go over 4,294,967,295 or scale up (doubt it)
    using Iterator = std::unordered_map<Id, std::unique_ptr<Component>>::iterator;
    using Container = std::unordered_map<Id, std::unique_ptr<Component>>;
    std::string name;
    const Id id;
    const size_t type_hash;
    Component(const type_info& type_info, Root& root):
        id(id_counter++),
        type_hash(type_info.hash_code()),
        name(type_info.name()),
        root(root) {}
	Component(const Component& other) = delete;
	Component(Component&& other) noexcept = delete;
	Component& operator=(const Component& other) = delete;
	Component& operator=(Component&& other) noexcept = delete;
    virtual ~Component() = default;
    Component* get_parent() const {
        return parent;
    }
    void set_parent(Component* _parent) {
        if (parent) {
            _parent->children.insert({id, std::move(parent->children.at(id))});
            parent->children.erase(id);
        } else {
            _parent->make_child(std::unique_ptr<Component>(this));
        }
    }
    virtual void fixed_update() {};
    virtual void update(std::chrono::milliseconds delta_ms) {};
    virtual void render(SDL_Renderer* renderer, const View_transform& transform) const {};
    void update_children(std::chrono::milliseconds delta_ms) {
        for (auto& [id, child] : children) {
            if (child->parent == nullptr) {
                orphans.push_back(child->id); }
            child->update(delta_ms);
            if (child->children.size()) {
                child->update_children(delta_ms); }
    }
    //cleanup
    std::ranges::for_each(orphans, [this](Id& e){
        auto& child = children[e];
        child->nuke_lineage();
        children.erase(e);});
    orphans.clear();
    }
    void render_children(SDL_Renderer* renderer, const View_transform& transform) const {
        for (const auto& [id, child] : children) {
            child->render(renderer, transform);
            if (child->children.size()) {
                child->render_children(renderer, transform); }
        }
    }
    void fixed_update_children() {
        for (auto& [id, child] : children) {
            child->fixed_update();
            if (child->children.size()) {
                child->fixed_update_children(); }
        }
    }
    void nuke_lineage() {
        if (children.size() != 0) {
            for (auto& [id, child] : children) {
                child->nuke_lineage(); }
        } children.clear();
    }
    /*
    using Actual_type = std::ranges::transform_view<std::ranges::subrange<typename std::unordered_map<uint32_t, std::unique_ptr<Component>>::iterator>,
        decltype([](const std::pair<const uint32_t, std::unique_ptr<Component>>& p) -> const std::unique_ptr<Component>& { return p.second; })>;
    */
    decltype(Container{} | std::views::values) get_children() {
         return children | std::views::values;
    }
    void abandon_child(Id child_id) {
        children.at(child_id)->parent = nullptr;
    }
    template <Has_base<Component> Child>
    Child* get_child_if_exists(Id id) {
        auto it = children.find(id);
        if (it == children.end()) {
            return nullptr; }
        return (*it).second.get();
    }
    template <class Child>
    Child& make_child() {
        auto child = std::make_unique<Child>(this->root);
        child->parent = this;
        const Id id = child->id;
        children.insert(std::make_pair(id, std::move(child)));
        return *static_cast<Child*>(children[id].get());
    }
    template<Has_base<Component> Child> Child* find_first(std::string_view name) {
        auto foundIt = std::ranges::find_if(children, [&name](auto& e) { e->name == name;});
        if (foundIt == children.end()) { return nullptr; }
        return static_cast<Child*>((*foundIt).get());
    }
    template <Has_base<Component> Child> Child* find_first_typed() {
        auto it = std::ranges::find_if(children, [](std::pair<Id, std::unique_ptr<Component>>& pair) {
            return pair.second->type_hash == typeid(Child).hash_code();
        });
        return (*it).second.get();
    }
    template <Has_base<Component> Child> Child& get_child(Id child_id) {
        return *children.at(child_id);
    }
    Root& root;
    Container children;
private:
    inline static Id id_counter{0};
    std::vector<Id> orphans;
    Component* parent;
};
