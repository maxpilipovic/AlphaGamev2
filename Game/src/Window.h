#pragma once

#include <SDL3/SDL.h>
#include <string>

class Window
{
public:
    Window();
    ~Window();

    bool Create(const std::string& title, int width, int height, Uint32 flags = 0);
    void Destroy();

    SDL_Window* GetSDLWindow() const { return m_window; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

    void SetTitle(const std::string& title);
    void GetSize(int& width, int& height) const;

private:
    SDL_Window* m_window;
    int m_width;
    int m_height;
};