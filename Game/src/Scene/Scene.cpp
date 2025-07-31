#include "Scene.h"
#include <algorithm>
#include "../Core/InputManager.h"
#include <SDL3/SDL.h>
#include <core/Math.h>

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
    bool checkKey1 = m_inputManager->IsKeyPressed(SDLK_1);
    bool checkKey2 = m_inputManager->IsKeyPressed(SDLK_2);
    bool checkKey3 = m_inputManager->IsKeyPressed(SDLK_3);
    bool checkKey4 = m_inputManager->IsKeyPressed(SDLK_4);

    if (!m_placingEntity) {
        if (checkKey1)
        {
            CreateRobot(RobotType::Blue);
        }
        if (checkKey2)
        {
            CreateRobot(RobotType::Red);
        }
        if (checkKey3)
        {
            CreateRobot(RobotType::Green);
        }
        if (checkKey4)
        {
            CreateRobot(RobotType::Yellow);
        }
    }
    

    //Keep moving
    UpdatePlacingRobot();

   //Place it
    if (!IsPlacingRobotColliding())
    {
        FinalizePlacingRobot();
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

bool Scene::IsPlacingRobotColliding()
{
    auto placingCircle = GetComponent<CircleCollider>(m_currentPlacingEntity);
    auto view = m_registry.GetView<Robot, CircleCollider>();

    for (const auto& [entity, _, circle] : view)
    {
        if (m_currentPlacingEntity == entity)
        {
            continue;
        }

        if (Math::IsCircleColliding(placingCircle->x, placingCircle->y, placingCircle->radius, circle.x, circle.y, circle.radius))
        {
            return true;
        }
        
    }
    return false;
}

void Scene::FinalizePlacingRobot()
{
    if (!m_placingEntity || !m_inputManager->IsButtonPressed(SDL_BUTTON_LEFT))
        return;

    auto transform = GetComponent<Transform>(m_currentPlacingEntity);
    auto sprite = GetComponent<SpriteRendererComponent>(m_currentPlacingEntity);
    auto range = GetComponent<RobotRange>(m_currentPlacingEntity);

    if (!transform || !sprite)
        return;

    //Final position based on mouse
    transform->x = m_inputManager->getX();
    transform->y = m_inputManager->getY();
    sprite->sprite.x = transform->x;
    sprite->sprite.y = transform->y;

    float centerX = transform->x + (sprite->sprite.w * 0.5);
    float centerY = transform->y + (sprite->sprite.h * 0.5);

    range->x = centerX;
    range->y = centerY;

    m_placingEntity = false;
}

void Scene::UpdatePlacingRobot()
{
    if (!m_placingEntity) return;

    auto transform = GetComponent<Transform>(m_currentPlacingEntity);
    auto circle = GetComponent<CircleCollider>(m_currentPlacingEntity);
    auto sprite = GetComponent<SpriteRendererComponent>(m_currentPlacingEntity);

    if (!transform || !circle) return;

    //Follow mouse position
    transform->x = m_inputManager->getX();
    transform->y = m_inputManager->getY();

    //Update sprite rect
    float centerX = transform->x + (sprite->sprite.w * 0.5);
    float centerY = transform->y + (sprite->sprite.h * 0.5);

    circle->x = centerX;
    circle->y = centerY;
}

void Scene::CreateRobot(RobotType robot)
{
    m_currentPlacingEntity = CreateEntity();

    auto transform = GetComponent<Transform>(m_currentPlacingEntity);
    auto sprite = AddComponent<SpriteRendererComponent>(m_currentPlacingEntity);
    auto circle = AddComponent<CircleCollider>(m_currentPlacingEntity);
    auto range = AddComponent<RobotRange>(m_currentPlacingEntity);

    //Add to seperate other colliders...
    AddComponent<Robot>(m_currentPlacingEntity);

    circle->radius = 40.0f;

    if (!transform || !sprite) return;

    //Set initial position
    transform->x = m_inputManager->getX();
    transform->y = m_inputManager->getY();
    transform->width = 50.0f;
    transform->height = 50.0f;

    //Assign initial sprite rect
    sprite->sprite = { transform->x, transform->y, transform->width, transform->height };

    //Assign color based on key

    switch (robot)
    {
    case RobotType::Blue:
    {
        AddComponent<BlueRobot>(m_currentPlacingEntity);
        //Why do this?
        sprite->red = 0;
        sprite->green = 0;
        sprite->blue = 255;
        range->radius = 100;
        break;
    }
    case RobotType::Red:
    {
        AddComponent<RedRobot>(m_currentPlacingEntity);
        sprite->red = 255;
        sprite->green = 0;
        sprite->blue = 0;
        range->radius = 125;
        break;
    }
    case RobotType::Green:
    {
        AddComponent<GreenRobot>(m_currentPlacingEntity);
        sprite->red = 0;
        sprite->green = 255;
        sprite->blue = 0;
        range->radius = 175;
        break;
    }
    case RobotType::Yellow:
    {
        AddComponent<YellowRobot>(m_currentPlacingEntity);
        sprite->red = 255;
        sprite->green = 255;
        sprite->blue = 0;
        range->radius = 250;
        break;
    }
    default:
        break;
    }

    //sprite->alpha = 255;

    m_placingEntity = true;
}

void Scene::Render(Renderer* renderer)
{
	//This is where you would render all the entities in the scene.
    //For example, you could iterate over all entities with a TransformComponent and a SpriteComponent
    //and then render them.

    auto view = m_registry.GetView<Transform, SpriteRendererComponent, RobotRange, CircleCollider>();

    for (const auto& [entity, transform, sprite, range, circle] : view) //Structured binding
    {
        //Ask ethana about this?
        sprite.sprite.x = transform.x;
        sprite.sprite.y = transform.y;
        renderer->SetDrawColor(sprite.red, sprite.green, sprite.blue, sprite.alpha);
        renderer->FillRect(&sprite.sprite);

        renderer->SetDrawColor(0, 0, 0, 255); //Black
        renderer->DrawCircle(circle.x, circle.y, circle.radius);

        renderer->DrawCircle(range.x, range.y, range.radius);
        


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
