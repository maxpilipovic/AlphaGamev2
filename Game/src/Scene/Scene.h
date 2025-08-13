#pragma once  
#include "Astra/Astra.hpp"  
#include "../Renderer/Renderer.h"  
#include "Components.h"  
#include "../Core/Paths.h" 

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

class Scene  
{  
public:  
    Scene() = default;  

    void Initialize(std::shared_ptr<InputManager>);  
    void Shutdown();  
    void Update(float deltatime);  
    void Render(Renderer* renderer);  
    
    // Create entity with components added atomically
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
    void SpawnEnemyTank();  
    void UpdateTankPathing(float deltatime);  

    // Astra::Entity FindEntity(const std::string& name) const;  

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

    // Registry access  
    Astra::Registry& GetRegistry() { return m_registry; }  
    const Astra::Registry& GetRegistry() const { return m_registry; }  

    bool IsActive() const { return m_active; }  

private:  
    void UpdatePhysics(float deltaTime);  

    Astra::Registry m_registry;  
    bool m_active = false;  
    std::vector<Astra::Entity> m_entities;  
    std::shared_ptr<InputManager> m_inputManager;  
    bool m_placingEntity = false; // Flag for checking if placing or not  
    Astra::Entity m_currentPlacingEntity; // Entity state for placing  

    // Spawning Tanks  
    float m_tankSpawnTimer = 0.0f;  
    float m_tankSpawnInterval = 5.0f;  
};