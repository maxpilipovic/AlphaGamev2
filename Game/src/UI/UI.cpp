#include "UI.h"
#include "../Core/PathUtils.h"
#include <iostream>

UI::UI(Scene* scene, Renderer* renderer) : m_Scene(scene), m_Renderer(renderer)
{
}

UI::~UI()
{
	for (auto const& [size, font] : m_Fonts)
	{
		TTF_CloseFont(font);
	}
}

void UI::Init()
{
	//Note: You might want a more robust path solution

	//Initalize the font

	if (TTF_Init() == -1) {
		std::cerr << "Failed to initialize SDL_ttf: " << std::endl;
		return;
	}
}

void UI::Update(float dt)
{
	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);


	auto view = m_Scene->GetRegistry().CreateView<UIButtonComponent, UITransformComponent>();
	for (auto [entity, button, transform] : view)
	{	

		if (!button->Visible) continue; //False

		SDL_FRect buttonRect = { transform->Position.x, transform->Position.y, transform->Size.x, transform->Size.y };
		SDL_FPoint mousePoint = { mouseX, mouseY };

		if (SDL_PointInRectFloat(&mousePoint, &buttonRect))
		{
			if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_MASK(SDL_BUTTON_LEFT))
			{
				button->State = ButtonState::Pressed;
			}
			else
			{
				button->State = ButtonState::Hovered;
			}
		}
		else
		{
			button->State = ButtonState::Normal;
		}
	}
}

void UI::Render()
{
    auto buttonView = m_Scene->GetRegistry().CreateView<UIButtonComponent, UITransformComponent>();
    for (auto [entity, button, transform] : buttonView)
    {
        if (button->Visible)
        {
            SDL_FRect buttonRect = { transform->Position.x, transform->Position.y, transform->Size.x, transform->Size.y };

            switch (button->State)
            {
                case ButtonState::Normal:
                    m_Renderer->SetDrawColor(100, 100, 100, 255); // Grey
                    break;
                case ButtonState::Hovered:
                    m_Renderer->SetDrawColor(150, 150, 150, 255); // Lighter Grey
                    break;
                case ButtonState::Pressed:
                    m_Renderer->SetDrawColor(50, 50, 50, 255); // Darker Grey
                    break;
            }
            m_Renderer->FillRect(&buttonRect);
        }
    }

	auto imageView = m_Scene->GetRegistry().CreateView<UIImageComponent, UITransformComponent>();
	for (auto [entity, image, transform] : imageView)
	{

		if (image->Visible && image->Texture)
		{
			SDL_FRect destRect = { transform->Position.x, transform->Position.y, transform->Size.x, transform->Size.y };

			//potential bug??
			SDL_FRect srcFRect;
			const SDL_FRect* srcFRectPtr = nullptr;
			if (image->SrcRect.w > 0 && image->SrcRect.h > 0)
			{
				srcFRect.x = (float)image->SrcRect.x;
				srcFRect.y = (float)image->SrcRect.y;
				srcFRect.w = (float)image->SrcRect.w;
				srcFRect.h = (float)image->SrcRect.h;
				srcFRectPtr = &srcFRect;
			}
			m_Renderer->RenderCopy(image->Texture, srcFRectPtr, &destRect);
		}
	}

	auto textView = m_Scene->GetRegistry().CreateView<UITextComponent, UITransformComponent>();
	for (auto [entity, text, transform] : textView)
	{

		if (text->Visible && !text->Text.empty())
		{
			TTF_Font* font = GetFont(text->FontSize);
			if (!font) continue;

			SDL_Surface* surface = TTF_RenderText_Blended(font, text->Text.c_str(), text->Text.size(), text->Color);
			SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Renderer->GetSDLRenderer(), surface);
			
			//potential bug??
			SDL_FRect destRect = { transform->Position.x, transform->Position.y, (float)surface->w, (float)surface->h };
			m_Renderer->RenderCopy(texture, nullptr, &destRect);

			SDL_DestroySurface(surface);
			SDL_DestroyTexture(texture);
		}
	}
}

TTF_Font* UI::GetFont(int size)
{
	if (m_Fonts.find(size) != m_Fonts.end())
	{
		return m_Fonts[size];
	}

	std::string fontPath = PathUtils::GetAssetsPath() + "Open_Sans/OpenSans-VariableFont_wdth,wght.ttf";
	TTF_Font* font = TTF_OpenFont(fontPath.c_str(), size);
	if (!font)
	{
		std::cerr << "Failed to load font: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	m_Fonts[size] = font;
	return font;
}