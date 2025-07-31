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
    bool IsButtonPressed(int x) const;
    float getX() const { return m_mouseX; }
    float getY() const { return m_mouseY; }

private:
    std::unordered_map<SDL_Keycode, bool> m_lastFrameKeys;
    std::unordered_map<SDL_Keycode, bool> m_currentKeys;
    float m_mouseX = 0.0f;
    float m_mouseY = 0.0f;
};
