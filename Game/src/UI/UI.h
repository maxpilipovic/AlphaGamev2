#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../Scene/Scene.h"
#include "../Scene/Components.h"
#include "../Renderer/Renderer.h"

class UI
{
public:
	UI(Scene* scene, Renderer* renderer, Player* player);
	~UI();

	void Init();
	void Update(float dt);
	void Render();
	void HandleEvent(const SDL_Event& e);


	

private:
	Player* m_Player;
	Scene* m_Scene;
	Renderer* m_Renderer;
	std::map<int, TTF_Font*> m_Fonts;

	TTF_Font* GetFont(int size);
};