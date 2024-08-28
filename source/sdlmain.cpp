#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "common.h"
#include "Game.h"
#include "Time_interval.h"
#include "collision_logic.h"
#include "utl.h"
void event_proc(not_null<Game*>game, World& world, SDL_Event& e);

int SDL_main(int argc, char **argv) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
    constexpr czstring window_name = "sdl_game";
    constexpr uint32 window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED; 
    auto window = utl::make_SDL_window(window_name,{.x = SDL_WINDOWPOS_UNDEFINED,
        .y = SDL_WINDOWPOS_UNDEFINED, .w = 1280, .h = 720 }, window_flags);
    constexpr uint32 renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    auto renderer = utl::make_SDL_renderer(window.get(), -1, renderer_flags);
    u_ptr<Game> game = make_unique<Game>();
    World world{.transform{}, .gravity = .01f}; {
        int32 w,h;
        SDL_GetWindowSize(window.get(), &w, &h);
        world.transform.viewport = V2{
            static_cast<float>(w),
            static_cast<float>(h) };
    }
    ImGuiContext* context = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window.get(), renderer.get());
    ImGui_ImplSDLRenderer2_Init(renderer.get());
    using namespace std::chrono;
    auto last_time = steady_clock::now();
    const milliseconds fixed_delta{8ms};
    Time_interval fixed_update_interval{fixed_delta};
    game->init(window.get());
    while (not game->quit) {
        /* events */ {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                ImGui_ImplSDL2_ProcessEvent(&event);
                event_proc(game.get(), world, event);
            }
        } /* update */ {
            ImGui_ImplSDLRenderer2_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
            auto curr_time = steady_clock::now();
            duration<double> delta_s = curr_time - last_time;
            const milliseconds delta = duration_cast<milliseconds>(curr_time - last_time);
            last_time = curr_time;
            auto& view_v = std::views::values;
            collision_logic::handle_physical_collisions(game->children | view_v, delta, world);
            game->update(delta);
            game->update_children(delta);
            fixed_update_interval([&game] {
                game->fixed_update();
                game->fixed_update_children();
            });
        } /* rendering */ {
            SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0xFF);
            SDL_RenderClear(renderer.get());
            game->print.assemble(window.get());
            game->explorer->assemble(window.get());
            game->render(renderer.get(), world);
            game->render_children(renderer.get(), world);
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
void event_proc(not_null<Game*> game, World& world, SDL_Event& e) {
    switch (e.type) {
        case SDL_QUIT: game->quit = true; break;
        case SDL_KEYDOWN:
            game->key_input_began->fire_signal(e.key);
            break;
        case SDL_KEYUP:
            game->key_input_ended->fire_signal(e.key);
            break;
        case SDL_MOUSEMOTION:
            game->mouse_moved->fire_signal(e.motion);
            break;
        case SDL_MOUSEBUTTONDOWN:
            game->mouse_button_input_began->fire_signal(e.button);
            break;
        case SDL_MOUSEBUTTONUP:
            game->mouse_button_input_ended->fire_signal(e.button);
            break;
        case SDL_MOUSEWHEEL:
            world.transform.scaling += V2{e.wheel.y * .01f, e.wheel.y * .01f}; 
            game->mouse_scrolled->fire_signal(e.wheel);
            break;
        case SDL_WINDOWEVENT:
            if (e.window.event == SDL_WINDOWEVENT_RESIZED){
                int32 new_width = e.window.data1;
                int32 new_height = e.window.data2;
                world.transform.viewport = V2 {
                    static_cast<float>(new_width),
                    static_cast<float>(new_height)};
            }
        break;
    }
}
