#pragma once
#include <common.h>
#include "Component.hpp"
#include "gui.h"
#include "Event.hpp"
using namespace std::chrono_literals;
class Game: public Component<Game> {
    //friend int SDL_main(int argc, char **argv);
public:
    Game(): Component{ typeid(Game), *this } {}
    virtual void update(const milliseconds& delta_ms) override;
    virtual void render(not_null<SDL_Renderer*> renderer, const World& world) const override;
    virtual void fixed_update() override;
    virtual void init(not_null<SDL_Window*> window);
    void assemble_gui_elements(not_null<SDL_Window*> window);
    using Key_event = Event<const SDL_KeyboardEvent&>;
    s_ptr<Key_event> key_input_began{make_shared<Key_event>()};
    s_ptr<Key_event> key_input_ended{make_shared<Key_event>()};
    using Mouse_button_event = Event<const SDL_MouseButtonEvent&>;
    s_ptr<Mouse_button_event> mouse_button_input_began{make_shared<Mouse_button_event>()};
    s_ptr<Mouse_button_event> mouse_button_input_ended{make_shared<Mouse_button_event>()};
    using Mouse_wheel_event = Event<const SDL_MouseWheelEvent&>;
    s_ptr<Mouse_wheel_event> mouse_scrolled{make_shared<Mouse_wheel_event>()};
    using Mouse_motion_event = Event<const SDL_MouseMotionEvent&>;
    s_ptr<Mouse_motion_event> mouse_moved{make_shared<Mouse_motion_event>()};
    Output_log print;
    bool quit{false};
    u_ptr<Event<const V2&>::Connection> connection{nullptr};
    static constexpr milliseconds fixed_delta{8ms};
private:
    std::unordered_map<std::string, std::unique_ptr<Gui_element>> gui_elements;
};
