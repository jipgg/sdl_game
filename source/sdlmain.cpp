#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Game.h"

int SDL_main(int argc, char **argv) {
    std::unique_ptr<Game> game = std::make_unique<Game>();
    return game->run();
}
