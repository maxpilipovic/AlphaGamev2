#include "Scene.h"
#include <algorithm>

void Scene::Initialize()
{   
	//This is where you can set up your scene, load resources, etc.
}

void Scene::Shutdown()
{
	//This is where you can clean up your scene, unload resources, etc.
    m_entities.clear();
}

void Scene::Update(float deltatime)
{
	//This is where you would update your game logic, like physics, AI, etc.
    UpdatePhysics(deltatime);
}

void Scene::Render(Renderer* renderer)
{
	//This is where you would render all the entities in the scene.
    //For example, you could iterate over all entities with a TransformComponent and a SpriteComponent
    //and then render them.
    SDL_FRect rect;
    rect.x = 100.0f;
    rect.y = 100.0f;
    rect.w = 200.0f;
    rect.h = 150.0f;

    renderer->SetDrawColor(255, 0, 0, 255); //Red
    renderer->FillRect(&rect);
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
