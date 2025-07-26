#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include "Game.h"
#include "Astra/astra.hpp"

int main(int argc, char* argv[])
{
    Game game;

    if (!game.Initialize("MaxGame", 1280, 720))
    {
        std::cerr << "Failed to initialize game" << std::endl;
        return -1;
    }

    game.Run();
    game.Shutdown();

    return 0;
}