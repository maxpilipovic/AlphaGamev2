#include "PathUtils.h"
#include <SDL3/SDL.h>
#include <string>

std::string PathUtils::GetAssetsPath()
{
    const char* basePath = SDL_GetBasePath();
    if (basePath)
    {
        return std::string(basePath) + "../../../assets/";
    }
    return "./assets/";
}
