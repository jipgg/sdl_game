#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "common.h"
#include "Game.h"
#include "Time_interval.h"
#include "collision_logic.h"
#include "utl.h"
void process_event(not_null<Game*>game, World& world, SDL_Event& e);

int SDL_main(int argc, char **argv) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
    constexpr gsl::czstring window_name = "sdl_game";
    constexpr uint32 window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED; 
    auto window = utl::make_SDL_window(window_name,{.x = SDL_WINDOWPOS_UNDEFINED,
        .y = SDL_WINDOWPOS_UNDEFINED, .w = 1280, .h = 720 }, window_flags);
    constexpr const uint32 renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    auto renderer = utl::make_SDL_renderer(window.get(), -1, renderer_flags);
    u_ptr<Game> game = make_unique<Game>();
    World world{.transform{}, .gravity = .01f}; {
        int32 w,h;
        SDL_GetWindowSize(window.get(), &w, &h);
        world.transform.viewport = V2{
            static_cast<float32>(w),
            static_cast<float32>(h)
        };
    }
    ImGuiContext* context = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    //ImFont* font = io.Fonts->AddFontFromFileTTF("resources/main_font.ttf", 20);
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window.get(), renderer.get());
    ImGui_ImplSDLRenderer2_Init(renderer.get());
    auto last_time = std::chrono::steady_clock::now();
    const std::chrono::milliseconds fixed_delta{16ms};
    Time_interval fixed_update_interval{fixed_delta};
    game->init(window.get());
    while (not game->quit) {
        /* events */ {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                ImGui_ImplSDL2_ProcessEvent(&event);
                process_event(game.get(), world, event);
            }
        } /* update */ {
            ImGui_ImplSDLRenderer2_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
            using namespace std::chrono;
            auto curr_time = steady_clock::now();
            duration<double> delta_s = curr_time - last_time;
            const milliseconds delta_ms = duration_cast<milliseconds>(curr_time - last_time);
            last_time = curr_time;
            fixed_update_interval([&game] {
                game->fixed_update();
                game->fixed_update_children();
            });
            auto children_v = game->children | std::views::values;
            collision_logic::handle_physical_collisions(children_v, delta_ms, world);
            game->update(delta_ms);
            game->update_children(delta_ms);
        } /* rendering */ {
            SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0xFF);
            SDL_RenderClear(renderer.get());
            game->print.assemble(window.get());
            game->render(renderer.get(), world);
            game->render_children(renderer.get(), world);
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
void process_event(not_null<Game*> game, World& world, SDL_Event& event) {
    switch (event.type) {
        case SDL_QUIT: game->quit = true; break;
        case SDL_KEYDOWN:
            game->key_input_began->fire_signal(event.key);
            break;
        case SDL_KEYUP:
            game->key_input_ended->fire_signal(event.key);
            break;
        case SDL_MOUSEMOTION:
            game->mouse_moved->fire_signal(event.motion); break;
        case SDL_MOUSEBUTTONDOWN:
            game->mouse_button_input_began->fire_signal(event.button);
            break;
        case SDL_MOUSEBUTTONUP:
            game->mouse_button_input_ended->fire_signal(event.button);
            break;
        case SDL_MOUSEWHEEL:
            world.transform.scaling += V2{event.wheel.y * .01f, event.wheel.y * .01f}; 
            game->mouse_scrolled->fire_signal(event.wheel);
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED){
                int32 new_width = event.window.data1;
                int32 new_height = event.window.data2;
                world.transform.viewport = V2{
                    static_cast<float>(new_width),
                    static_cast<float>(new_height)
                };
                //game->viewport = SDL_Rect{0, 0, new_width, new_height};
                //SDL_RenderSetViewport(renderer.get(), &this->viewport);
            }
        break;
    }
}
