#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <string>
#include "UI/UI.h"

class Renderer;
class Window;
class InputManager;
class Scene;
//class Player;

class Game
{
public:
    Game();
    ~Game();

    bool Initialize(const std::string& title, int width, int height);
    void Run();
    void Shutdown();

private:
    void Update(float deltaTime);
    void Render();

private:
    std::unique_ptr<Window> m_window;
    std::unique_ptr<Renderer> m_renderer;
    std::shared_ptr<InputManager> m_inputManager;
    std::unique_ptr<Scene> m_scene;
    std::unique_ptr<UI> m_UI;
    //std::unique_ptr<Player> m_player;
    //std::unique_ptr<Player> m_player;
    bool m_isRunning;

    uint64_t m_lastFrameTime;
};