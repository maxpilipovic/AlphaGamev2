#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>

class InputManager
{
public:
    void Update();
    void ProcessEvent(const SDL_Event& event);

    bool IsKeyDown(SDL_Keycode key) const;
    bool IsKeyPressed(SDL_Keycode key) const;
    bool IsKeyReleased(SDL_Keycode key) const;

private:
    std::unordered_map<SDL_Keycode, bool> m_lastFrameKeys;
    std::unordered_map<SDL_Keycode, bool> m_currentKeys;
};
