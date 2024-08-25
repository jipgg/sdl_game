#include "Game.h"
#include "print.hpp"
#include "utl.h"
#include "Time_interval.h"
#include "game_components.h"
#include "solvers.h"

void Game::init(SDL_Window* window) {
    int32 w,h;
    SDL_GetWindowSize(window, &w, &h);
    viewport = SDL_Rect{0, 0, w, h};
    //transformation.translation = Vector2{50, -50};
    view_transform.viewport = Vector2{
        static_cast<double>(w),
        static_cast<double>(h)
    };
    view_transform.scaling.y = -1; // sets the origin to the bottom instead of top
    Platform& base = make_child<Platform>();
    base.size = Vector2{static_cast<double>(w * 4), h / 4.};
    base.position = Vector2{0, 0};
    auto& platform = make_child<Platform>();
    platform.size = V2{100, 100};
    platform.position = V2{500, h - h / 4.0 -100};
    auto& another = make_child<Platform>();
    another.size = V2{300, 50};
    another.position = V2 {100, h/ 4.0};

    auto& player = make_child<Player>();
    player.position.x += 10;
    player.name = "player";
    player.physical_properties.elasticity = .5f;
    /*
    connection = make_unique<Event<const Vector2&>::Connection>(
        std::move(player.position_changed->connect([this](const Vector2& new_pos) {
            view_transform.translation.x = new_pos.x - view_transform.viewport.x / 2;
        })));
        */
}
void Game::update(milliseconds delta) {
    //collision detection -> need to make this a recursive function for nested child components
    using u_component = std::unique_ptr<Component>;
    ranges::for_each(children|views::values, [this,&delta](u_component& e) {
        auto& obj = static_cast<Game_component&>(*e);
        Physical_properties& obj_props{obj.physical_properties};
        int64 delta_ms = delta.count();
        Vector2 obj_old_pos = obj.position;
        auto [l, r, t, b] = obj.collision_points();
        if (obj_props.is_welded) {
            return; }
        if (obj.is_falling) {
            obj.acceleration.y = view_transform.scaling.y * gravity;
        }
        obj.velocity += obj.acceleration * delta_ms;
        obj.position += obj.velocity * delta_ms;
        ranges::for_each(children | views::values, [this,&obj,&l, &r, &t, &b](u_component& f) {
            auto& other = static_cast<Game_component&>(*f);
            const Rect other_rect = other.collision_rect();
            if(obj.id == other.id) return;
            Physical_properties& other_props{other.physical_properties};
            if (utl::is_vector2_in_rect(l, other_rect)) {
                if (obj.velocity.x < 0) {
                    obj.is_obstructed = true;
                    obj.position.x = other.position.x + other.size.x;
                    if (other_props.is_welded) {
                        obj.velocity.x = -obj.velocity.x 
                            * obj.physical_properties.elasticity;
                    }
                } else {
                    obj.is_obstructed = false;
                }
            } else if (utl::is_vector2_in_rect(r, other_rect)) {
                if (obj.velocity.x > 0) {
                    obj.is_obstructed = true;
                    obj.position.x = other.position.x - obj.size.x;
                    if (other_props.is_welded) {
                        obj.velocity.x = -obj.velocity.x 
                            * obj.physical_properties.elasticity;
                    }
                } else {
                    obj.is_obstructed = false;
                }
            }
            if (utl::is_vector2_in_rect(t, other_rect)) {
                obj.is_falling = false;
                if (obj.velocity.y < 0) {
                    obj.position.y = other.position.y + other.size.y;
                    if (other_props.is_welded) {
                        obj.velocity.y = -obj.velocity.y 
                            * obj.physical_properties.elasticity;
                    }
                }
            } else if (utl::is_vector2_in_rect(b, other_rect)) {
                obj.is_falling = false;
                if (obj.velocity.y > 0) {
                    obj.position.y = other.position.y - obj.size.y;
                    if (other.physical_properties.is_welded) {
                        obj.velocity.y = -obj.velocity.y 
                            * obj.physical_properties.elasticity;
                    }
                }
            }
        });
        if (obj_old_pos != obj.position) {
            obj.position_changed->fire_signal(obj.position);
        }
    });
}
void Game::render(SDL_Renderer* renderer, const View_transform& transform) const {
    print.assemble(window.get());
}
void Game::fixed_update() {
}
void Game::process_event(SDL_Event& event) {
    switch (event.type) {
        case SDL_QUIT: this->quit = true; break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym) {
                case SDLK_RIGHT:
                view_transform.translation.x += 10;
                break;
                case SDLK_LEFT:
                    view_transform.translation -= 10;
                break;
            }
            key_input_began->fire_signal(event.key);
            break;
        case SDL_KEYUP: key_input_ended->fire_signal(event.key); break;
        case SDL_MOUSEMOTION: mouse_moved->fire_signal(event.motion); break;
        case SDL_MOUSEBUTTONDOWN:
            mouse_button_input_began->fire_signal(event.button);
            break;
        case SDL_MOUSEBUTTONUP: mouse_button_input_ended->fire_signal(event.button); break;
        case SDL_MOUSEWHEEL:
            view_transform.scaling += Vector2{event.wheel.y * .01, event.wheel.y * .01}; 
            mouse_scrolled->fire_signal(event.wheel);
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED){
                int32 new_width = event.window.data1;
                int32 new_height = event.window.data2;
                view_transform.viewport = Vector2{
                    static_cast<double>(new_width),
                    static_cast<double>(new_height)
                };
                this->viewport = SDL_Rect{0, 0, new_width, new_height};
                SDL_RenderSetViewport(renderer.get(), &this->viewport);
                this->print(new_width, new_height);
            }
        break;
    }
}
int Game::run () {
    if (SDL_Init(SDL_INIT_VIDEO) > 0) {prerrln("failed to init sdl", SDL_GetError());}
    if (TTF_Init()) {prerrln("failed to init ttf", SDL_GetError());}
    if (not IMG_Init(IMG_INIT_PNG)) {prerrln("failed to init img", SDL_GetError()); }
    this -> window = utl::make_SDL_window("hey",{.x = SDL_WINDOWPOS_UNDEFINED,
        .y = SDL_WINDOWPOS_UNDEFINED, .w = 1280, .h = 720 }, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
    constexpr const uint32_t renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    this -> renderer =  utl::make_SDL_renderer(window.get(), -1, renderer_flags);
    ImGuiContext* context = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = io.Fonts->AddFontFromFileTTF("resources/main_font.ttf", 20);
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window.get(), renderer.get());
    ImGui_ImplSDLRenderer2_Init(renderer.get());
    Time_interval fps_refresh_interval(500ms);
    auto last_time = std::chrono::steady_clock::now();
    const std::chrono::milliseconds fixed_delta{16ms};
    Time_interval fixed_update_interval{fixed_delta};
    auto texture = utl::load_SDL_img_texture(renderer.get(), "resources/jw.png");
    this->init(window.get());
    while (not this->quit) {
        /* events */ {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                ImGui_ImplSDL2_ProcessEvent(&event);
                this->process_event(event);
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
            fixed_update_interval([this] {
                this->fixed_update();
                this->fixed_update_children();
            });
            this->update(delta_ms);
            this->update_children(delta_ms);
        } /* rendering */ {
            SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0xFF);
            SDL_RenderClear(renderer.get());
            this->render(renderer.get(), view_transform);
            this->render_children(renderer.get(), view_transform);
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
