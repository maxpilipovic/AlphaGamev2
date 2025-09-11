#include "PathUtils.h"
#include <SDL3/SDL.h>
#include <string>

std::string PathUtils::GetAssetsPath()
{
    const char* basePath = SDL_GetBasePath();
    if (basePath)
    {
        // The path from premake is bin/Debug-windows-x86_64/Game
        return std::string(basePath) + "../../../assets/";
    }
    return "./assets/";
}
