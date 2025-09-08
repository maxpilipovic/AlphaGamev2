#include "UI/UI.h"

void UI::Initialize(Scene* scene, Renderer* renderer)
{
	m_Scene = scene;
	m_Renderer = renderer;
}

void UI::Update(InputManager* input)
{

	//Check if scene null.
	if (!m_Scene || !input)
	{
		return;
	}

	//Grab a list registry...
	//Loop through everything, CHECK if button was pressed.
	//Do something if button pressed...

	auto& registry = m_Scene->GetRegistry();

	//Grab the view?
	auto view = registry.CreateView<UIButtonComponent, UITransformComponent>();

	for (auto entity : view)
	{
		//Work with speicifc components
	}




}

void UI::Render()
{
	if (!m_Scene || !m_Renderer)
	{
		return;
	}


}