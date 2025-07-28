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

