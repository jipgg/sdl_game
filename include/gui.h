#pragma once
#include <common.h>
#include <sstream>
#include <list>
struct SDL_Window;
class Gui_element {
public:
    virtual ~Gui_element() = default;
    virtual void assemble(not_null<SDL_Window*> window) const = 0;
    bool visible{true};
};
enum class Output_type{Output, Error, Warning};
struct Output_element {
    std::string message;
    Output_type type;
};
class Output_log: public Gui_element {
public:
    Output_log();
    template <class ...Args>
    void operator()(Output_type type, Args&& ...args) {
        std::stringstream stream;
        ((stream << args << " "), ...) << "\n";
        Output_element e{};
        e.type = type;
        e.message = stream.str();
        output_elements.push_front(e);
    }
    template <class ...Args>
    void operator()(Args&&...args) {operator()(Output_type::Output, args...);}
    virtual void assemble(gsl::not_null<SDL_Window*> window) const override;
private:
    ImGuiStyle style;
    void setup_imgui_style();
    std::list<Output_element> output_elements;
};
