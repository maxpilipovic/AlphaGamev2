#pragma once  
#include "Astra/Astra.hpp"  
#include "../Renderer/Renderer.h"  
#include "Components.h"  
#include "../Core/Paths.h" 
#include "Player.h"

class InputManager;  

enum class RobotType {  
    Red,  
    Green,  
    Blue,  
    Yellow  
};  

enum class TankType {  
    Teal,  
    Orange,  
    Violet,  
    Red  
};  

struct EnemySpawn
{
    TankType type;
    int count;
    float spawnInterval;

};

struct Level
{
    std::vector<std::vector<EnemySpawn>> waves;
};

class Scene  
{  
public:  
    Scene() = default;  

    void Initialize(std::shared_ptr<InputManager>);  
    void Shutdown();  
    void Update(float deltatime);  
    void Render(Renderer* renderer);  
    
    // Create entity with components added atomically
    // comment
    // //sdasdas
    // Can be called with no template parameters for just Transform
    template<typename... Components>
    Astra::Entity CreateEntity()
    {
        Astra::Entity entity = m_registry.CreateEntity<Transform, Components...>(Transform{}, Components{}...);
        m_entities.push_back(entity);
        return entity;
    }
    
    void DestroyEntity(Astra::Entity);  

    // Creation  
    void CreateRobot(RobotType robot);  
    void CreateTank(TankType tank);  
    void CreateProjectile(Astra::Entity& robot, Astra::Entity& tank);  

    // Updating  
    void UpdatePlacingEntity();  
    void FinalizePlacingEntity();  
    bool IsPlacingEntityColliding();  
    void UpdateProjectiles(float deltatime);  
    void RobotInteraction(float deltatime);  

    // Tanks Spawning  
    void SpawnEnemyTank(TankType tank);  
    void UpdateTankPathing(float deltatime);

    //PathZone Entity
    bool UpdatePathZone();

    //UI
    void DrawEnemyPath(Renderer* renderer, std::vector<SDL_FPoint> path);

    //BUTTON & TEXT UI
    Astra::Entity CreateButton(float x, float y, float width, float height, std::string& text, int color);
    Astra::Entity CreateText(float x, float y, std::string& text);
    void AddTextToScreen(float x, float y, Astra::Entity textEntity, std::string text, int fontSize);

    // Astra::Entity FindEntity(const std::string& name) const;  

    //Random reference garbage
    Player& GetPlayer();

    //Templates
    template<typename T, typename... Args>  
    T* AddComponent(Astra::Entity entity, Args&&... args)  
    {  
        return m_registry.AddComponent<T>(entity, std::forward<Args>(args)...);  
    }  

    template<typename T>  
    T* GetComponent(Astra::Entity entity)  
    {  
        return m_registry.GetComponent<T>(entity);  
    }  

    //LEVELS
    void LoadLevel1();
    void LoadLevel2();
    void LoadLevel3();
    void LoadLevel4();
    void LoadLevel5();

    void LoadNextLevel();
    void UpdateLevel(float delatime);

    // Registry access  
    Astra::Registry& GetRegistry() { return m_registry; }  
    const Astra::Registry& GetRegistry() const { return m_registry; }  

    bool IsActive() const { return m_active; }  

private:  
    void UpdatePhysics(float deltaTime);  

    Player m_player;
    Astra::Registry m_registry;  
    bool m_active = false;  
    std::vector<Astra::Entity> m_entities;  
    std::shared_ptr<InputManager> m_inputManager;  
    bool m_placingEntity = false; // Flag for checking if placing or not  
    Astra::Entity m_currentPlacingEntity; // Entity state for placing  

    //Spawning Tanks  
    float m_tankSpawnTimer = 0.0f;  
    float m_tankSpawnInterval = 5.0f;  

    //UI
    Astra::Entity m_cashText;
    Astra::Entity m_levelText;

    //Create path entity
    Astra::Entity m_pathZone;

    //Draw width of path
    bool m_drawPath = true; //Toggle for visualisation
    float m_widthPath = 80.0f;

    //LEVEL SYSTEM
    Level m_currentLevel;
    int m_currentWaveIndex = 0;
    int m_currentSpawnIndex = 0;
    int m_enemiesLeftInSpawn = 0;
    float m_spawnTimer = 0.0f;
    float m_Intermission = 0.0f;
    bool m_isLevelActive = false;
    bool m_isIntermission = false;

    //LEVEL NUMBER
    int m_levelCount = 1;
};