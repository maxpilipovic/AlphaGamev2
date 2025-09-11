#include "Renderer.h"
#include "../Core/Window.h"
#include <iostream>

Renderer::Renderer() : m_renderer(nullptr) {}
Renderer::~Renderer()
{
	Destroy();
}

bool Renderer::Create(Window* window, const char* driver)
{

	if (!window || !window->GetSDLWindow())
	{
		std::cerr << "Invalid window provied to renderer" << std::endl;
		return false;
	}

	if (m_renderer)
	{
		std::cerr << "Renderer already created" << std::endl;
		return false;
	}

	m_renderer = SDL_CreateRenderer(window->GetSDLWindow(), driver);
	if (!m_renderer)
	{
		std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
	}

	return true;
}

void Renderer::Destroy()
{
	if (m_renderer)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void Renderer::Clear()
{
	if (m_renderer)
	{
		SDL_RenderClear(m_renderer);
	}
}

void Renderer::Present()
{

	if (m_renderer)
	{
		SDL_RenderPresent(m_renderer);
	}
}

void Renderer::SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	if (m_renderer)
	{
		SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
	}
}

void Renderer::DrawRect(const SDL_FRect* rect)
{
	if (m_renderer && rect)
	{
		SDL_RenderRect(m_renderer, rect);
	}
}

void Renderer::FillRect(const SDL_FRect* rect)
{
	if (m_renderer && rect)
	{
		SDL_RenderFillRect(m_renderer, rect);
	}
}

void Renderer::DrawLine(float x1, float y1, float x2, float y2)
{
	if (m_renderer)
	{
		SDL_RenderLine(m_renderer, x1, y1, x2, y2);
	}
}

void Renderer::DrawCircle(int centerX, int centerY, int radius)
{
	int x = radius;
	int y = 0;
	int decisionOver2 = 1 - x;   // Decision criterion divided by 2 evaluated at x=r, y=0

	while (y <= x) {
		// Draw each octant
		SDL_RenderPoint(m_renderer, centerX + x, centerY + y);
		SDL_RenderPoint(m_renderer, centerX + y, centerY + x);
		SDL_RenderPoint(m_renderer, centerX - x, centerY + y);
		SDL_RenderPoint(m_renderer, centerX - y, centerY + x);
		SDL_RenderPoint(m_renderer, centerX - x, centerY - y);
		SDL_RenderPoint(m_renderer, centerX - y, centerY - x);
		SDL_RenderPoint(m_renderer, centerX + x, centerY - y);
		SDL_RenderPoint(m_renderer, centerX + y, centerY - x);
		y++;
		if (decisionOver2 <= 0) {
			decisionOver2 += 2 * y + 1;
		}
		else {
			x--;
			decisionOver2 += 2 * (y - x) + 1;
		}
	}
}

void Renderer::RenderCopy(SDL_Texture* texture, const SDL_FRect* srcRect, const SDL_FRect* destRect)
{
    if (m_renderer && texture)
    {
        SDL_RenderTexture(m_renderer, texture, srcRect, destRect);
    }
}

SDL_Texture* Renderer::LoadTexture(const char* path)
{
    if (!m_renderer)
    {
        return nullptr;
    }
    return IMG_LoadTexture(m_renderer, path);
}



