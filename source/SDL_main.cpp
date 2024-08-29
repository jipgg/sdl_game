//#define CREATE_WINDOWS_CONSOLE
#ifdef CREATE_WINDOWS_CONSOLE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include "common.h"
#include "game.h"
#include "physics_system.h"
#include "utl.h"
#include "dev_tools.h"
#include <functional>

int SDL_main(int argc, char **argv) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
    constexpr czstring window_name = "sdl_game";
    constexpr uint32_t window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED; 
    auto window = utl::make_SDL_window(window_name,{.x = SDL_WINDOWPOS_UNDEFINED,
        .y = SDL_WINDOWPOS_UNDEFINED, .w = 1280, .h = 720 }, window_flags);
    constexpr uint32_t renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    auto renderer = utl::make_SDL_renderer(window.get(), -1, renderer_flags);
    #ifdef CREATE_WINDOWS_CONSOLE
    if (AllocConsole()) {
        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
        freopen_s(&fp, "CONIN$", "r", stdin);
    }
    #endif
    ImGuiContext* context = ImGui::CreateContext();
    ImGui::GetStyle() = dev_tools::create_default_style();
    ImGui_ImplSDL2_InitForSDLRenderer(window.get(), renderer.get());
    ImGui_ImplSDLRenderer2_Init(renderer.get());
    Game_state state{};
    state.transform = View_transform{.translation{0, 0}, .scaling{1, 1} };
    SDL_GetWindowSize(window.get(), &state.transform.viewport.x,
                      &state.transform.viewport.y);
    game::init(state.transform.viewport, state);
    namespace rv = std::ranges::views;
    using entity_ptr = std::unique_ptr<Entity>;
    // # should probably just make these global functions
    std::function<void(entity_ptr&)> render_entity_tree =
    [&render_entity_tree, &state, &renderer](entity_ptr& entity){
        entity->render(renderer.get(), state.transform);
        for (auto& component : entity->components | rv::values) {
            render_entity_tree(component);
        }
    };
    using namespace std::chrono;
    std::function<void(entity_ptr&, const milliseconds&)> update_entity_tree =
        [&update_entity_tree](entity_ptr& entity, const milliseconds& delta) {
            entity->update(delta);
            for(auto& component : entity->components | rv::values) {
                update_entity_tree(component, delta);
            }
    };
    auto last_time = steady_clock::now();
    SDL_Event e;
    while (not state.quit) {
        /* event loop */ {
            while (SDL_PollEvent(&e)) {
                ImGui_ImplSDL2_ProcessEvent(&e);
                switch (e.type) {
                    case SDL_QUIT:
                        state.quit = true;
                        break;
                    case SDL_KEYDOWN:
                        break;
                    case SDL_KEYUP:
                        if (e.key.keysym.sym == SDLK_o) {
                            dev_tools::visibility_toggles::output = true;}
                        break;
                    case SDL_MOUSEMOTION:
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        break;
                    case SDL_MOUSEBUTTONUP:
                        break;
                    case SDL_MOUSEWHEEL:
                        break;
                    case SDL_WINDOWEVENT:
                        if (e.window.event == SDL_WINDOWEVENT_RESIZED){
                            int new_width = e.window.data1;
                            int new_height = e.window.data2;
                            state.transform.viewport = V2i{new_width, new_height};
                        }
                        break;
                }
            }
        } /* update loop */ {
            // # maybe put delta inside game_state
            auto curr_time = steady_clock::now();
            const milliseconds delta = duration_cast<milliseconds>(curr_time - last_time);
            last_time = curr_time;
            physics::handle_physical_collisions(state.entities, delta);
            game::update(delta, state);
            for(auto& entity : state.entities){
                update_entity_tree(entity, delta);}
        } /* render loop */ {
            SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0xFF);
            SDL_RenderClear(renderer.get());
            for (auto& entity : state.entities){
                render_entity_tree(entity);}
            ImGui_ImplSDLRenderer2_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
            dev_tools::assemble_imgui_windows(state.entities | rv::all);
            ImGui::Render();
            ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer.get());
            SDL_RenderPresent(renderer.get());
        }
    }
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext(context);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}
