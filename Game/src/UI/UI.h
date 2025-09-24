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

	

private:
	Scene* m_Scene;
	Renderer* m_Renderer;
	std::map<int, TTF_Font*> m_Fonts;

	TTF_Font* GetFont(int size);
};