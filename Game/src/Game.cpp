#include "Game.h";
#include <iostream>
#include "Core/Window.h"
#include "Renderer/Renderer.h"
#include "Core/InputManager.h"
#include "Scene/Scene.h"
#include "UI/UI.h"
#include "Scene/Player.h"

//Constructor

Game::Game() :
	m_window(std::make_unique<Window>()),
	m_renderer(std::make_unique<Renderer>()),
	m_inputManager(std::make_shared<InputManager>()),
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

	m_scene->Initialize(m_inputManager);
	m_UI = std::make_unique<UI>(m_scene.get(), m_renderer.get(), &m_scene->GetPlayer());
	m_UI->Init();
	
	/*Astra::Entity player = m_scene->CreateEntity();
	m_scene->AddComponent<PlayerComponent>(player);
	auto* sprite = m_scene->AddComponent<SpriteRendererComponent>(player);
	sprite->red = 255;
	sprite->sprite.h = 100.0f;
	sprite->sprite.w = 100.0f;*/

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

			//Forward event to UI
			m_UI->HandleEvent(event);
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
	TTF_Quit();
	SDL_Quit();
}

void Game::Update(float deltaTime)
{
	if (m_inputManager->IsKeyPressed(SDLK_ESCAPE))
	{
		m_isRunning = false;
	}

    m_scene->Update(deltaTime);
	m_UI->Update(deltaTime);
}

void Game::Render()
{
	m_renderer->SetDrawColor(18, 18, 18, 255);
	m_renderer->Clear(); //What is this?

    m_scene->Render(m_renderer.get()); //What are we doing here? //Rendering everything to scene?

	m_UI->Render();

	m_renderer->Present(); //What is this?
}


