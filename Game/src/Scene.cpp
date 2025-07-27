#include "Scene.h"
#include <algorithm>
#include "InputManager.h"
#include <SDL3/SDL.h>

void Scene::Initialize(std::shared_ptr<InputManager> inputManager)
{   
	//This is where you can set up your scene, load resources, etc.
    Astra::Entity player = CreateEntity();
    m_registry.AddComponent<PlayerComponent>(player, 100.0f, 100.0f, 200.0f, 150.0f, 0.0001f);
    m_inputManager = inputManager;
}

void Scene::Shutdown()
{
	//This is where you can clean up your scene, unload resources, etc.
    m_entities.clear();
}

void Scene::Update(float deltatime)
{
	//This is where you would update your game logic, like physics, AI, etc.
    
    //Grab all players in registry(world)
    auto view = m_registry.GetView<PlayerComponent>();

    for (auto& [entity, player] : view)
    {
        player.x = m_inputManager->getX();
        player.y = m_inputManager->getY();
    }

    UpdatePhysics(deltatime);
}

void Scene::Render(Renderer* renderer)
{
	//This is where you would render all the entities in the scene.
    //For example, you could iterate over all entities with a TransformComponent and a SpriteComponent
    //and then render them.

    auto view = m_registry.GetView<PlayerComponent>();

    for (auto [entity, player] : view)
    {   

        SDL_Surface* surface = IMG_Load(file);



        SDL_FRect rect;
        
        rect.x = player.x;
        rect.y = player.y;
        rect.w = player.width;
        rect.h = player.height;

        renderer->SetDrawColor(255, 0, 0, 255); //Red
        renderer->FillRect(&rect);

    }
}

Astra::Entity Scene::CreateEntity()
{
	Astra::Entity entity = m_registry.CreateEntity();
    m_entities.push_back(entity);
	return entity;
}

void Scene::DestroyEntity(Astra::Entity entity)
{
	m_registry.DestroyEntity(entity);

    //Iterator to find entities...
    auto it = std::find(m_entities.begin(), m_entities.end(), entity);
    if (it != m_entities.end())
    {
        m_entities.erase(it);
    }
}

void Scene::UpdatePhysics(float deltaTime)
{
    //This is a placeholder for your physics update logic.
    //You would typically iterate over entities with physics components here.
}
