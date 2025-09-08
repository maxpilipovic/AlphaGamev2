#pragma once
#include "../Scene/Scene.h"


class UI
{

public:

	UI() = default;
	void Initialize(Scene* scene, Renderer* renderer);
	void Render();
	void Update(InputManager* input);
	//Creates default constructor and deconstructor for both if not specified.


private:
	UI* m_ui;
	Scene* m_Scene;
	Renderer* m_Renderer;

};
