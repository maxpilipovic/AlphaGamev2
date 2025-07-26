#include "Game.h";
#include <iostream>
#include "Window.h"
#include "Renderer.h"
#include "InputManager.h"
#include "Scene.h"

//Constructor

Game::Game() :
	m_window(std::make_unique<Window>()),
	m_renderer(std::make_unique<Renderer>()),
	m_inputManager(std::make_unique<InputManager>()),
    m_scene(std::make_unique<Scene>()),
	m_isRunning(false),
	m_lastFrameTime(0)
{ 
}

Game::~Game() {}

bool Game::Initialize(const std::string& title, int width, int height)
{
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
	{
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		return false;
	}

	if (!m_window->Create(title, width, height))
	{
		std::cerr << "Failed to create window" << std::endl;
		return false;
	}

	if (!m_renderer->Create(m_window.get()))
	{
		std::cerr << "Failed to create renderer" << std::endl;
		return false;
	}

	m_scene->Initialize();

	m_isRunning = true;
	m_lastFrameTime = SDL_GetPerformanceCounter();

	return true;
}

void Game::Run()
{
	while (m_isRunning)
	{
		m_inputManager->Update();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				m_isRunning = false;
			}
			m_inputManager->ProcessEvent(event);
		}

		Uint64 currentTime = SDL_GetPerformanceCounter();
		float deltaTime = (currentTime - m_lastFrameTime) / (float)SDL_GetPerformanceFrequency();
		m_lastFrameTime = currentTime;

		Update(deltaTime);
		Render();
	}
}

void Game::Shutdown()
{
    m_scene->Shutdown();
	m_renderer.reset();
	m_window.reset();
	SDL_Quit();
}

void Game::Update(float deltaTime)
{
	if (m_inputManager->IsKeyPressed(SDLK_ESCAPE))
	{
		m_isRunning = false;
	}

    m_scene->Update(deltaTime);
}

void Game::Render()
{
	m_renderer->SetDrawColor(18, 18, 18, 255);
	m_renderer->Clear();

    m_scene->Render(m_renderer.get());

	m_renderer->Present();
}


