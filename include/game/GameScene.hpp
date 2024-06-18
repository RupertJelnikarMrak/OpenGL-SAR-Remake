#pragma once

#include <entt/entt.hpp>

#include "engine/rendering/RenderWindow.hpp"

class SceneBase {
protected:
    RenderWindow *_window;
    
public:
    SceneBase(RenderWindow *window) : _window(window) {}
    virtual ~SceneBase() = default;

    virtual void init() = 0;
    virtual void discard() = 0;

    virtual void runLoop() = 0;
};

class GameScene : public SceneBase {
private:
    entt::registry _registry;

    GLubyte *_backgroundTextureBuffer;

    // Probbably better to have a vector of function pointers to dynamically add systems
    void handleMovement(float deltaTime);
    void handleEnemies(float deltaTime);

    void processInput();
    void update(float deltaTime);
    void draw(float deltaTime);
public:
    GameScene(RenderWindow *window) : SceneBase(window) {}
    ~GameScene() override = default;
    
    void init() override;
    void discard() override;

    void runLoop() override;
};
