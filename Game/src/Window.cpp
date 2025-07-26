#include "window.h"
#include <iostream>

//Constructor?
Window::Window() : m_window(nullptr), m_width(0), m_height(0) {}
Window::~Window()
{
	Destroy();
}

bool Window::Create(const std::string& title, int width, int height, Uint32 flags) 
{
	//Error checking
	if (m_window != nullptr)
	{
		return false;
	}

	m_window = SDL_CreateWindow(title.c_str(), width, height, flags);
	m_width = width;
	m_height = height;

	if (!m_window) {
		//In the case that the window could not be made...
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

void Window::Destroy()
{
	if (m_window)
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
		m_width = 0;
		m_height = 0;
	}
}

void Window::SetTitle(const std::string& title)
{
	if (m_window)
	{
		SDL_SetWindowTitle(m_window, title.c_str());
	}
}

void Window::GetSize(int& width, int& height) const
{
	if (m_window)
	{
		SDL_GetWindowSize(m_window, &width, &height);
	}
	else
	{
		width = m_width;
		height = m_width;
	}
}



