#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../Scene/Scene.h"
#include "../Scene/Components.h"
#include "../Renderer/Renderer.h"

class UI
{
public:
	UI(Scene* scene, Renderer* renderer);
	~UI();

	void Init();
	void Update(float dt);
	void Render();
	void HandleEvent(SDL_Event& event);

private:
	Scene* m_Scene;
	Renderer* m_Renderer;
	TTF_Font* m_Font;
};