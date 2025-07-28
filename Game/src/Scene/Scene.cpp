#include "Scene.h"
#include <algorithm>
#include "../Core/InputManager.h"
#include <SDL3/SDL.h>

void Scene::Initialize(std::shared_ptr<InputManager> inputManager)
{
    //This is where you can set up your scene, load resources, etc.
    m_inputManager = inputManager; //Input manager is passed here

    // Load the player's sprite

}

void Scene::Shutdown()
{
    //This is where you can clean up your scene, unload resources, etc.
    m_entities.clear();
}

void Scene::Update(float deltatime)
{

    //This is where you would update your game logic, like physics, AI, etc.
    bool check1 = m_inputManager->IsKeyPressed(SDLK_1);

    if (check1 and !m_placingEntity)
    {
        m_currentPlacingEntity = CreateEntity();
        auto transform = GetComponent<Transform>(m_currentPlacingEntity); //Getting pointer from GetComponent
        auto sprite = AddComponent<SpriteRendererComponent>(m_currentPlacingEntity); //Getting pointer from GetComponent
        
        //Grabs our coords
        transform->x = m_inputManager->getX();
        transform->y = m_inputManager->getY();

        //Set w & h
        transform->width = 50.0f;
        transform->height = 50.0f;

        //Initalize and create color BLUE
        sprite->sprite = { transform->x, transform->y, transform->width, transform->height };
        sprite->red = 0;
        sprite->green = 0;
        sprite->blue = 255;
        sprite->alpha = 255;
        /*SDL_FRect rect;
        rect.x = transform->x;
        rect.y = transform->y;
        rect.w = transform->width;
        rect.h = transform->height;

        sprite->sprite = rect;*/

        m_placingEntity = true;
    }

    //Keep moving
    if (m_placingEntity)
    {
        auto transform = GetComponent<Transform>(m_currentPlacingEntity);
        auto sprite = GetComponent<SpriteRendererComponent>(m_currentPlacingEntity);

        //Follow mouse position
        transform->x = m_inputManager->getX();
        transform->y = m_inputManager->getY();

        //Update sprite rect as well
        sprite->sprite.x = transform->x;
        sprite->sprite.y = transform->y;
    }

    

    if (m_placingEntity && (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)))
    {
        auto transform = GetComponent<Transform>(m_currentPlacingEntity);
        auto sprite = GetComponent<SpriteRendererComponent>(m_currentPlacingEntity);

        transform->x = m_inputManager->getX();
        transform->y = m_inputManager->getY();
        sprite->sprite.x = transform->x;
        sprite->sprite.y = transform->y;

        m_placingEntity = false;
        check1 = false;
    }


    //Grab all players in registry(world)
    /*auto view = m_registry.GetView<Transform, PlayerComponent>();

    for (const auto& [entity, transform, player] : view)
    {
        transform.x = m_inputManager->getX();
        transform.y = m_inputManager->getY();
    }*/

    UpdatePhysics(deltatime);
}

void Scene::Render(Renderer* renderer)
{
	//This is where you would render all the entities in the scene.
    //For example, you could iterate over all entities with a TransformComponent and a SpriteComponent
    //and then render them.

    auto view = m_registry.GetView<Transform, SpriteRendererComponent>();


    for (const auto& [entity, transform, sprite] : view)
    {
        //Ask ethana about this?
        sprite.sprite.x = transform.x;
        sprite.sprite.y = transform.y;
        renderer->SetDrawColor(sprite.red, sprite.green, sprite.blue, sprite.alpha);
        renderer->FillRect(&sprite.sprite);

        //renderer->DrawSprite(&player.sprite);w
        //SDL_FRect rect;
        //rect.x = transform.x;
        //rect.y = transform.y;
        //rect.w = 100.0;
        //rect.h = 100.0;

        //renderer->SetDrawColor(255, 0, 0, 255);
        //renderer->FillRect(&rect);

    }
}

Astra::Entity Scene::CreateEntity()
{
	Astra::Entity entity = m_registry.CreateEntity();
    m_registry.AddComponent<Transform>(entity);
    m_entities.push_back(entity);
	return entity; //Returns id
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
