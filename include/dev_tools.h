#pragma once
#include <imgui.h>
#include <sstream>
#include <string>
#include <ranges>
#include <list>
#include <memory>
#include <Entity.h>
namespace dev_tools {
ImGuiStyle create_default_style();
using entity_view = std::ranges::ref_view<std::list<std::unique_ptr<Entity>>>;
void assemble_imgui_windows(entity_view view);
namespace visibility_toggles {
inline bool explorer{true};
inline bool output{true};
}
inline std::string output_log{};
template <class ...Params>
void output(Params&&...args) {
    std::stringstream stream;
    ((stream << args << " "), ...) << "\n";
    output_log = stream.str() + output_log;
}
}
