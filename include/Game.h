#pragma once
#include <common.h>
#include "Component.hpp"
#include "gui.h"
#include "utl.h"
#include "Event.hpp"
using namespace std::chrono_literals;
class Game: public Component<Game> {
public:
    Game(): Component{ typeid(Game), *this } {}
    virtual void update(std::chrono::milliseconds delta_ms) override;
    virtual void render(SDL_Renderer* renderer, const View_transform& transform) const override;
    virtual void fixed_update() override;
    virtual void process_event(SDL_Event& event);
    virtual void init(SDL_Window* window);
    int run();
    void assemble_gui_elements(SDL_Window* window);
    using Key_event = Event<const SDL_KeyboardEvent&>;
    std::shared_ptr<Key_event> key_input_began{std::make_shared<Key_event>()};
    std::shared_ptr<Key_event> key_input_ended{std::make_shared<Key_event>()};
    using Mouse_button_event = Event<const SDL_MouseButtonEvent&>;
    std::shared_ptr<Mouse_button_event> mouse_button_input_began{std::make_shared<Mouse_button_event>()};
    std::shared_ptr<Mouse_button_event> mouse_button_input_ended{std::make_shared<Mouse_button_event>()};
    using Mouse_wheel_event = Event<const SDL_MouseWheelEvent&>;
    std::shared_ptr<Mouse_wheel_event> mouse_scrolled{std::make_shared<Mouse_wheel_event>()};
    using Mouse_motion_event = Event<const SDL_MouseMotionEvent&>;
    std::shared_ptr<Mouse_motion_event>
    mouse_moved{std::make_shared<Mouse_motion_event>()};

    Output_log print;
    utl::unique_dtor_ptr<SDL_Window> window{nullptr, nullptr};
    utl::unique_dtor_ptr<SDL_Renderer> renderer{nullptr, nullptr};
    bool quit{false};
    double gravity{.01};
    u_ptr<Event<const Vector2&>::Connection> connection{nullptr};
    SDL_Rect viewport;
    static constexpr milliseconds fixed_delta{8ms};
private:
    View_transform view_transform;
    std::unordered_map<std::string, std::unique_ptr<Gui_element>> gui_elements;
};
