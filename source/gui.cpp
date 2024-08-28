#include "gui.h"
#include <algorithm>
#include "game_components.h"
Output_log::Output_log() {
    setup_imgui_style();
}
void Output_log::assemble(SDL_Window* window) const {
    ImGuiStyle old_style = ImGui::GetStyle();
    ImGui::GetStyle() = this->style;
    if (visible) {
        int width, height;
        //SDL_GetWindowSize(window.get(), &width, &height);
        ImGui::SetNextWindowBgAlpha(.8f);
        ImGui::Begin("output", const_cast<bool*>(&visible), ImGuiWindowFlags_NoCollapse);
        //ImVec2 size = ImGui::GetWindowSize();
        //ImVec2 pos = ImGui::GetWindowPos();
        //ImGui::SetWindowSize(ImVec2{static_cast<float>(width), size.y});
        //ImGui::SetWindowPos(ImVec2{0.f, height - size.y});
        constexpr ImVec4 err_color{.8f, 0.f, 0.f, 1.f};
        constexpr ImVec4 warn_color{.8f, .8f, 0.f, 1.f};
        std::for_each(std::begin(output_elements), std::end(output_elements),
                      [&err_color, &warn_color](const Output_element& e){
            switch(e.type) {
                case Output_type::Error:
                    ImGui::PushStyleColor(ImGuiCol_Text, err_color);
                    ImGui::TextUnformatted(e.message.c_str());
                    ImGui::PopStyleColor();
                return;
                case Output_type::Warning:
                    ImGui::PushStyleColor(ImGuiCol_Text, warn_color);
                    ImGui::TextUnformatted(e.message.c_str());
                    ImGui::PopStyleColor();
                return;
                default:
                    ImGui::TextUnformatted(e.message.c_str());
                return;
            }
        });
        ImGui::End();
    }
    ImGui::GetStyle() = old_style;
}

void Output_log::setup_imgui_style() {
    style.WindowPadding = ImVec2(15, 15);
    style.WindowBorderSize = 1.f;
    style.WindowRounding = 10.0f;
    style.FramePadding = ImVec2(5, 5);
    style.FrameRounding = 4.0f;
    style.ItemSpacing = ImVec2(12, 8);
    style.ItemInnerSpacing = ImVec2(8, 6);
    style.IndentSpacing = 25.0f;
    style.ScrollbarSize = 15.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabMinSize = 5.0f;
    style.GrabRounding = 3.0f;
    style.Colors[ImGuiCol_Border] = ImVec4(0.2f, .2f, .2f, 1.00f);
    style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
}
void deep_tree_display(not_null<Component<Game>*> parent) {
	ImGui::PushID(parent);
	if (ImGui::TreeNodeEx(parent->name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth)) {
		for (auto& [id, child] : parent->children)
			deep_tree_display(child.get());
	    ImGui::TreePop();
	}
	ImGui::PopID();
}
void Explorer::assemble(SDL_Window* window) const {
    if (ImGui::Begin("explorer", nullptr))
        deep_tree_display(root);
    ImGui::End();
}
