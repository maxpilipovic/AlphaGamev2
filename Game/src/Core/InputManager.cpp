#include "InputManager.h"
#include <iostream>

void InputManager::Update()
{
    m_lastFrameKeys = m_currentKeys;
}

void InputManager::ProcessEvent(const SDL_Event& event)
{
    
    if (event.type == SDL_EVENT_MOUSE_MOTION)
    {
        SDL_GetMouseState(&m_mouseX, &m_mouseY);
    }
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        m_currentKeys[event.key.key] = true;
    }
    else if (event.type == SDL_EVENT_KEY_UP)
    {
        m_currentKeys[event.key.key] = false;
    }
}

bool InputManager::IsKeyDown(SDL_Keycode key) const
{
    auto it = m_currentKeys.find(key);
    return it != m_currentKeys.end() && it->second;
}

bool InputManager::IsKeyPressed(SDL_Keycode key) const
{
    bool lastFrame = false;
    auto it_last = m_lastFrameKeys.find(key);
    if (it_last != m_lastFrameKeys.end())
    {
        lastFrame = it_last->second;
    }

    return IsKeyDown(key) && !lastFrame;
}

bool InputManager::IsKeyReleased(SDL_Keycode key) const
{
    bool lastFrame = false;
    auto it_last = m_lastFrameKeys.find(key);
    if (it_last != m_lastFrameKeys.end())
    {
        lastFrame = it_last->second;
    }

    return !IsKeyDown(key) && lastFrame;
}
