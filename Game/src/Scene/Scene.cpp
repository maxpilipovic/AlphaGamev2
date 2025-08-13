#include "Scene.h"
#include <algorithm>
#include <iostream>
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
    bool checkKey5 = m_inputManager->IsKeyPressed(SDLK_5);
    bool checkKey6 = m_inputManager->IsKeyPressed(SDLK_6);
    bool checkKey7 = m_inputManager->IsKeyPressed(SDLK_7);
    bool checkKey8 = m_inputManager->IsKeyPressed(SDLK_8);

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
        if (checkKey5)
        {
            CreateTank(TankType::Orange);
        }
        if (checkKey6)
        {
            CreateTank(TankType::Red);
        }
        if (checkKey7)
        {
            CreateTank(TankType::Teal);
        }
        if (checkKey8)
        {
            CreateTank(TankType::Violet);
        }
    }
    
    //Update MOVEMENT - spawned tanks
    m_tankSpawnTimer += deltatime;
    
    if (m_tankSpawnTimer >= m_tankSpawnInterval)
    {
        SpawnEnemyTank();
        m_tankSpawnTimer = 0.0f;
    }

    
    //Update tank movement
    UpdateTankPathing(deltatime);

    //Keep moving
    UpdatePlacingEntity();

   //Place it
    if (!IsPlacingEntityColliding())
    {
        FinalizePlacingEntity();
    }

    //Update all tanks and robots in registry
	auto robots = m_registry.CreateView<Transform, Robot, RobotRange>();

	for (const auto& [entity, transform, robot, range] : robots)
	{
        RobotInteraction(deltatime);
	}

    //Update projectiles
	UpdateProjectiles(deltatime);

    //Update physics (idk if I have any for this project)
    UpdatePhysics(deltatime);
}

void Scene::UpdateTankPathing(float deltatime)
{
    auto tanks = m_registry.CreateView<Transform, Tank, PathFollower>();

    // Collect entities to destroy (deferred destruction)
    std::vector<Astra::Entity> entitiesToDestroy;

    for (const auto& [entity, transform, tank, follower] : tanks)
    {

        //End of path
        if (follower->currentWayPoint >= enemyPathL1.size())
        {
            entitiesToDestroy.push_back(entity);
            continue;
        }

        //Get the current target and calculate the distance 
        SDL_FPoint target = enemyPathL1[follower->currentWayPoint];
        float dx = target.x - transform->x;
        float dy = target.y - transform->y;
        float distance = std::sqrt(dx * dx + dy * dy);

        //Less then a pixel just move it anyway.
        if (distance < 1.0f)
        {
            follower->currentWayPoint++;
            
        }


        //Normalize direction? Was confused about this
        float normX = dx / distance;
        float normY = dy / distance;

        //Moves the tank
        transform->x += normX * follower->speed * deltatime;
        transform->y += normY * follower->speed * deltatime;

        //Moves the sprite
        auto sprite = GetComponent<SpriteRendererComponent>(entity);
        if (sprite) {
            sprite->sprite.x = transform->x;
            sprite->sprite.y = transform->y;
        }

        //Moves the circle collider
        auto circle = GetComponent<CircleCollider>(entity);
        if (circle) {
            float centerX = transform->x + (sprite ? sprite->sprite.w * 0.5f : 0.0f);
            float centerY = transform->y + (sprite ? sprite->sprite.h * 0.5f : 0.0f);
            circle->x = centerX;
            circle->y = centerY;
        }





    }

    // Destroy entities after iteration completes
    for (Astra::Entity entity : entitiesToDestroy)
    {
        DestroyEntity(entity);
    }
}

void Scene::SpawnEnemyTank()
{
    // Create entity with all components atomically
    Astra::Entity spawnedTank = CreateEntity<Tank, OrangeTank, PathFollower, SpriteRendererComponent, CircleCollider>();
    
    // Get component pointers after all components are added
    auto transform = GetComponent<Transform>(spawnedTank);
    auto pathfinder = GetComponent<PathFollower>(spawnedTank);
    auto sprite = GetComponent<SpriteRendererComponent>(spawnedTank);
    auto circle = GetComponent<CircleCollider>(spawnedTank);

    transform->x = enemyPathL1[0].x;
    transform->y = enemyPathL1[0].y;

    circle->radius = 40.0f;

    //Orange tank
    sprite->red = 255;
    sprite->green = 165;
    sprite->blue = 0;

    //Set initial position
    transform->width = 50.0f;
    transform->height = 50.0f;

    //Path finding
    pathfinder->currentWayPoint = 1; //Update to next. Move toward next object.
    pathfinder->speed = 100.0f;

}


void Scene::RobotInteraction(float deltatime)
{
    auto robots = m_registry.CreateView<Transform, Robot, RobotRange>();
    auto tanks = m_registry.CreateView<Transform, Tank>();

    for (const auto& [robotEntityConst, robotTransform, robot, robotRange] : robots)
    {
        Astra::Entity robotEntity = robotEntityConst; //Mutable copy of the entity
  
        
        //If its not valid
        if (!m_registry.IsValid(robot->target))
        {
            for (const auto& [tankEntity, tankTransform, tank] : tanks)
            {
                if (Math::Distance(robotTransform->x, robotTransform->y, tankTransform->x, tankTransform->y) <= robotRange->radius)
                {
                    robot->target = tankEntity;
                    break;
                }
            }
        }
        else 
        {
            for (const auto& [tankEntity, tankTransform, tank] : tanks)
            {
                if (Math::Distance(robotTransform->x, robotTransform->y, tankTransform->x, tankTransform->y) > robotRange->radius)
                {
                    robot->target = Astra::Entity::Invalid();
                    break;
                }
            }
        }

        if (robot->target && robot->fireCooldown <= 0.0f && m_placingEntity == false )
        {

            CreateProjectile(robotEntity, robot->target);
            robot->fireCooldown = 1.0f / robot->fireRate;
        }

        if (robot->fireCooldown > 0.0f)
        {
            robot->fireCooldown -= deltatime;
        }
    }
}

bool Scene::IsPlacingEntityColliding()
{
    auto placingCircle = GetComponent<CircleCollider>(m_currentPlacingEntity);
    auto view = m_registry.CreateView<Robot, CircleCollider>();

    for (const auto& [entity, _, circle] : view)
    {
        if (m_currentPlacingEntity == entity)
        {
            continue;
        }
        
        //Copilot told me to add this
        if (!placingCircle) {
            return false;
        }

        if (Math::IsCircleColliding(placingCircle->x, placingCircle->y, placingCircle->radius, circle->x, circle->y, circle->radius))
        {
            return true;
        }
        
    }
    return false;
}

void Scene::FinalizePlacingEntity()
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
    sprite->sprite.w = transform->width;  // Preserve width
    sprite->sprite.h = transform->height; // Preserve height

    float centerX = transform->x + (sprite->sprite.w * 0.5);
    float centerY = transform->y + (sprite->sprite.h * 0.5);

    if (range) //Check nullptr
    {
        range->x = centerX;
        range->y = centerY;
    }

    m_placingEntity = false;
}

void Scene::UpdatePlacingEntity()
{
    if (!m_placingEntity) return;

    auto transform = GetComponent<Transform>(m_currentPlacingEntity);
    auto circle = GetComponent<CircleCollider>(m_currentPlacingEntity);
    auto sprite = GetComponent<SpriteRendererComponent>(m_currentPlacingEntity);

    if (!transform || !circle) return;

    //Follow mouse position
    transform->x = m_inputManager->getX();
    transform->y = m_inputManager->getY();

    //Update sprite rect - position and dimensions
    if (sprite) {
        sprite->sprite.x = transform->x;
        sprite->sprite.y = transform->y;
        sprite->sprite.w = transform->width;
        sprite->sprite.h = transform->height;
    }

    //Update sprite rect
    float centerX = transform->x + (sprite ? sprite->sprite.w * 0.5 : 0);
    float centerY = transform->y + (sprite ? sprite->sprite.h * 0.5 : 0);

    circle->x = centerX;
    circle->y = centerY;
}

void Scene::CreateTank(TankType tank)
{
    // Create entity with all common components atomically
    m_currentPlacingEntity = CreateEntity<SpriteRendererComponent, CircleCollider, Tank>();
    
    // Add the type-specific component
    switch (tank)
    {
    case TankType::Orange:
        AddComponent<OrangeTank>(m_currentPlacingEntity);
        break;
    case TankType::Red:
        AddComponent<RedTank>(m_currentPlacingEntity);
        break;
    case TankType::Teal:
        AddComponent<TealTank>(m_currentPlacingEntity);
        break;
    case TankType::Violet:
        AddComponent<VioletTank>(m_currentPlacingEntity);
        break;
    }

    // NOW get component pointers after ALL components are added (entity is in final archetype)
    auto transform = GetComponent<Transform>(m_currentPlacingEntity);
    auto sprite = GetComponent<SpriteRendererComponent>(m_currentPlacingEntity);
    auto circle = GetComponent<CircleCollider>(m_currentPlacingEntity);

    if (!transform || !sprite || !circle) return;

    //Set initial position
    transform->x = m_inputManager->getX();
    transform->y = m_inputManager->getY();
    transform->width = 50.0f;
    transform->height = 50.0f;
    
    circle->radius = 40.0f;

    //Assign initial sprite rect - MUST include width and height!
    sprite->sprite.x = transform->x;
    sprite->sprite.y = transform->y;
    sprite->sprite.w = transform->width;
    sprite->sprite.h = transform->height;

    //Assign color based on type
    switch (tank)
    {
    case TankType::Orange:
        sprite->red = 255;
        sprite->green = 165;
        sprite->blue = 0;
        break;
    case TankType::Red:
        sprite->red = 255;
        sprite->green = 0;
        sprite->blue = 0;
        break;
    case TankType::Teal:
        sprite->red = 0;
        sprite->green = 128;
        sprite->blue = 128;
        break;
    case TankType::Violet:
        sprite->red = 238;
        sprite->green = 130;
        sprite->blue = 238;
        break;
    default:
        break;
    }

    //sprite->alpha = 255;

    m_placingEntity = true;
}

void Scene::CreateRobot(RobotType robot)
{
    // Create entity with all common components atomically
    m_currentPlacingEntity = CreateEntity<SpriteRendererComponent, CircleCollider, RobotRange, Robot>();
    
    // Add the type-specific component
    switch (robot)
    {
    case RobotType::Blue:
        AddComponent<BlueRobot>(m_currentPlacingEntity);
        break;
    case RobotType::Red:
        AddComponent<RedRobot>(m_currentPlacingEntity);
        break;
    case RobotType::Green:
        AddComponent<GreenRobot>(m_currentPlacingEntity);
        break;
    case RobotType::Yellow:
        AddComponent<YellowRobot>(m_currentPlacingEntity);
        break;
    }

    // NOW get component pointers after ALL components are added (entity is in final archetype)
    auto transform = GetComponent<Transform>(m_currentPlacingEntity);
    auto sprite = GetComponent<SpriteRendererComponent>(m_currentPlacingEntity);
    auto circle = GetComponent<CircleCollider>(m_currentPlacingEntity);
    auto range = GetComponent<RobotRange>(m_currentPlacingEntity);

    if (!transform || !sprite || !circle || !range) return;

    //Set initial position
    transform->x = m_inputManager->getX();
    transform->y = m_inputManager->getY();
    transform->width = 50.0f;
    transform->height = 50.0f;
    
    circle->radius = 40.0f;

    //Assign initial sprite rect - MUST include width and height!
    sprite->sprite.x = transform->x;
    sprite->sprite.y = transform->y;
    sprite->sprite.w = transform->width;
    sprite->sprite.h = transform->height;

    //Assign color and range based on type
    switch (robot)
    {
    case RobotType::Blue:
        sprite->red = 0;
        sprite->green = 0;
        sprite->blue = 255;
        range->radius = 100;
        break;
    case RobotType::Red:
        sprite->red = 255;
        sprite->green = 0;
        sprite->blue = 0;
        range->radius = 125;
        break;
    case RobotType::Green:
        sprite->red = 0;
        sprite->green = 255;
        sprite->blue = 0;
        range->radius = 175;
        break;
    case RobotType::Yellow:
        sprite->red = 255;
        sprite->green = 255;
        sprite->blue = 0;
        range->radius = 250;
        break;
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

    //Robots
    auto view = m_registry.CreateView<Transform, SpriteRendererComponent, CircleCollider, Astra::Optional<RobotRange>, Astra::Optional<Tank>>();

    for (const auto& [entity, transform, sprite, circle, range, tank] : view) //Structured binding
    {
        //Ask ethana about this?
        sprite->sprite.x = transform->x;
        sprite->sprite.y = transform->y;
        renderer->SetDrawColor(sprite->red, sprite->green, sprite->blue, sprite->alpha);
        renderer->FillRect(&sprite->sprite);

        if (tank)
        {
            renderer->SetDrawColor(sprite->red, sprite->green, sprite->blue, sprite->alpha);
        }
        else
        {
            renderer->SetDrawColor(0, 0, 0, 255); //Black
        }
        renderer->DrawCircle(circle->x, circle->y, circle->radius);

        if (range)
        {
            renderer->DrawCircle(range->x, range->y, range->radius);
        }
    }

	auto projectiles = m_registry.CreateView<Transform, SpriteRendererComponent, Projectile>();
	for (const auto& [entity, transform, sprite, projectile] : projectiles)
	{
		sprite->sprite.x = transform->x;
		sprite->sprite.y = transform->y;
		renderer->SetDrawColor(sprite->red, sprite->green, sprite->blue, sprite->alpha);
		renderer->FillRect(&sprite->sprite);
	}
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

void Scene::CreateProjectile(Astra::Entity& robot, Astra::Entity& tank)
{
	// Create entity with all components atomically
	auto projectile = CreateEntity<Projectile, SpriteRendererComponent>();
	
	// Get component pointers after all components are added
	auto transform = GetComponent<Transform>(projectile);
	auto robotTransform = GetComponent<Transform>(robot);
	auto tankTransform = GetComponent<Transform>(tank);
	auto proj = GetComponent<Projectile>(projectile);
	auto sprite = GetComponent<SpriteRendererComponent>(projectile);

	//Check null ptr
	if (!tankTransform || !robotTransform || !transform)
	{
		return;
	}

	transform->x = robotTransform->x;
	transform->y = robotTransform->y;
	transform->width = 10.0f;
	transform->height = 10.0f;

	proj->directionAngle = atan2(tankTransform->y - robotTransform->y, tankTransform->x - robotTransform->x);
	proj->velocity = 500.0f;
	proj->lifetime = 5.0f;

	sprite->sprite = { transform->x, transform->y, transform->width, transform->height };
	sprite->red = 255;
	sprite->green = 255;
	sprite->blue = 255;
}

void Scene::UpdateProjectiles(float deltaTime)
{
	auto projectiles = m_registry.CreateView<Transform, Projectile>();
	auto tanks = m_registry.CreateView<Transform, Tank, CircleCollider>();

	// Collect entities to destroy (deferred destruction)
	std::vector<Astra::Entity> entitiesToDestroy;

	for (const auto& [projEntity, projTransform, projectile] : projectiles)
	{

		projectile->age += deltaTime;
		if (projectile->age > projectile->lifetime)
		{
			entitiesToDestroy.push_back(projEntity);
			continue;
		}

		projTransform->x += cos(projectile->directionAngle) * projectile->velocity * deltaTime;
        projTransform->y += sin(projectile->directionAngle) * projectile->velocity * deltaTime;

		//for (const auto& [tankEntity, tankTransform, tank, circle] : tanks)
		//{
  //          //Colides destroy both
		//	if (Math::IsCircleColliding(projTransform.x, projTransform.y, projTransform.width / 2, tankTransform.x, tankTransform.y, circle.radius))
		//	{
		//		entitiesToDestroy.push_back(tankEntity);
		//		entitiesToDestroy.push_back(projEntity);
		//		break;
		//	}
		//}
	}

	// Destroy entities after iteration completes
	for (Astra::Entity entity : entitiesToDestroy)
	{
		DestroyEntity(entity);
	}
}
