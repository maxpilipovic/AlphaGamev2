#pragma once

#include "../Scene/Scene.h"


class UI
{

public:

	UI() = default;
	void Initialize(Scene* scene, Renderer* renderer);
	//Creates default constructor and deconstructor for both if not specified.


private:
	UI* m_ui;

};