#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <string>

class Renderer;
class Window;
class InputManager;
class Scene;

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
    std::unique_ptr<InputManager> m_inputManager;
    std::unique_ptr<Scene> m_scene;
    bool m_isRunning;

    uint64_t m_lastFrameTime;
};