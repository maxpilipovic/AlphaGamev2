#pragma once
#include "Astra/Astra.hpp"
#include "../Renderer/Renderer.h"
#include "Components.h"

class InputManager;

class Scene
{
public:
	Scene() = default;

	void Initialize(std::shared_ptr<InputManager>);
	void Shutdown();
	void Update(float deltatime);
	void Render(Renderer* renderer);
	Astra::Entity CreateEntity();
	void DestroyEntity(Astra::Entity);
	//Astra::Entity FindEntity(const std::string& name) const;

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
    bool m_placingEntity = false; //Flag for checking if placing or not
    Astra::Entity m_currentPlacingEntity; //Entity state for placing
};