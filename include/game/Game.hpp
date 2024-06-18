#pragma once

#include "engine/rendering/RenderWindow.hpp"
#include "game/GameScene.hpp"

class Game
{
private:
    RenderWindow _window;

    SceneBase *_currentScene;

    bool _running;
public:
    Game();
    ~Game();

    int init();

    void run();
    void stop();
};
