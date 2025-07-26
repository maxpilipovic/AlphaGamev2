#pragma once

#include <SDL3/SDL.h>

class Window;

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool Create(Window* window, const char* driver = nullptr);
    void Destroy();

    void Clear();
    void Present();

    void SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    void DrawRect(const SDL_FRect* rect);
    void FillRect(const SDL_FRect* rect);
    void DrawLine(float x1, float y1, float x2, float y2);

    SDL_Renderer* GetSDLRenderer() const { return m_renderer; }

private:
    SDL_Renderer* m_renderer;
};