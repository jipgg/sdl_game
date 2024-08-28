#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "common.h"
#include "game_logic.h"
#include "physics_system.h"
#include "utl.h"
#include "print.hpp"
#include "entity_derivations.h"
constexpr float GRAVITY{.01f};

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
    if (AllocConsole()) {
        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
        freopen_s(&fp, "CONIN$", "r", stdin);
    }
    ImGuiContext* context = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window.get(), renderer.get());
    ImGui_ImplSDLRenderer2_Init(renderer.get());
    Game_state state{}; {//scoped to not have the old window size laying around
        int window_width, window_height;
        SDL_GetWindowSize(window.get(), &window_width, &window_height);
        state.transform = View_transform{
            .translation {0, 0},
            .scaling = V2{1, 1},
            .viewport{static_cast<float>(window_width), static_cast<float>(window_height)},
        };
        game::init(V2i{window_width, window_height}, &state);
    }
    using namespace std::chrono;
    auto last_time = steady_clock::now();
    bool quit = false;
    while (not quit) {
        /* events */ {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                ImGui_ImplSDL2_ProcessEvent(&e);
                switch (e.type) {
                    case SDL_QUIT:
                        quit = true;
                        break;
                    case SDL_KEYDOWN:
                        break;
                    case SDL_KEYUP:
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
                        }
                        break;
                }
            }
        } /* update */ {
            ImGui_ImplSDLRenderer2_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
            auto curr_time = steady_clock::now();
            //duration<double> delta_sec = curr_time - last_time;
            const milliseconds delta = duration_cast<milliseconds>(curr_time - last_time);
            last_time = curr_time;
            physics::handle_physical_collisions(state.entities, delta, GRAVITY);
            game::update(delta, &state);
            for(auto& entity : state.entities) {
                entity->update(delta);
            }
        } /* rendering */ {
            SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0xFF);
            SDL_RenderClear(renderer.get());
            for (auto& entity : state.entities) {
                entity->render(renderer.get(), state.transform);
            }
            ImGui::Render();
            ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer.get());
            SDL_RenderPresent(renderer.get());
        }
    }
    //save_to_file(game.get());
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext(context);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}
