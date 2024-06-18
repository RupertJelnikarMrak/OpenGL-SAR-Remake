#include "game/GameScene.hpp"

#include <chrono>
#include <ctime>
#include <spdlog/spdlog.h>

#include "engine/rendering/RenderWindow.hpp"
#include "engine/rendering/Texture.hpp"
#include "game/EntityComponents.hpp"
#include "utils/PathUtils.hpp"

const int gridMultiplier = 100;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void createPlayer(entt::registry &registry) {
    using namespace ecs::comp;
    Texture texture = createTextureFromFile(PathUtils::absolutePath("/assets/textures/steve.jpg"));
    auto player = registry.create();
    registry.emplace<Position>(player, 0.0f, 0.0f);
    registry.emplace<Velocity>(player, 0.0f, 0.0f);
    registry.emplace<Renderable>(player, texture, 10.0f);
    registry.emplace<PlayerControlled>(player, GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D);
}

void createEnemies(entt::registry &registry) {
    using namespace ecs::comp;
    Texture texture = createTextureFromFile(PathUtils::absolutePath("/assets/textures/zombie.jpg"));
    srand(time(nullptr));
    for(int i = 0; i < 10; i++) {
        auto enemy = registry.create();
        float x = (rand() % 20) * 10.0f - 95.0f;
        float y = (rand() % 20) * 10.0f - 95.0f;
        registry.emplace<Position>(enemy, x, y);
        registry.emplace<Velocity>(enemy, 0.0f, 0.0f);
        registry.emplace<Renderable>(enemy, texture, 10.0f);
        registry.emplace<AiWanderingControlled>(enemy, rand() % 200 - 100.0f, rand() % 200 - 100.0f);
    }
}

void GameScene::init() {
    using namespace ecs::comp;
    _window->createBackgroundTextureBuffer(200, 200);

    // Map PBO to CPU, update data, unmap PBO
    GLubyte *ptr = _window->mapPBO();
    if(ptr) {
        // Update ptr with your data, e.g., fire effect
        // Example: set all pixels to red
        srand(time(nullptr));
        for(int i = 0; i < 200 * 200 * 4; i += 4) {
            if(rand() % 1000 == 0) {
                ptr[i] = 255;     // R
                ptr[i + 1] = 0;   // G
                ptr[i + 2] = 0;   // B
                ptr[i + 3] = 255; // A
            } else {
                ptr[i] = 90;      // R
                ptr[i + 1] = 255; // G
                ptr[i + 2] = 90;  // B
                ptr[i + 3] = 255; // A
            }
        }
        _window->unmapPBO();
    }

    _window->setKeyCallback(keyCallback);
    createPlayer(_registry);
    createEnemies(_registry);

    auto view = _registry.view<Renderable>();
    for(auto entity: view) {
        auto &renderable = view.get<Renderable>(entity);
        _window->loadTexture(renderable.texture);
    }
}

void GameScene::discard() {
    _window->setKeyCallback(nullptr);
    _registry.clear();
}

void GameScene::processInput() {
    glfwPollEvents();

#define key GLFW_PRESS == _window->getKey
    using namespace ecs::comp;

    static float playerSpeed = 50.0f;

    auto view = _registry.view<PlayerControlled, Velocity>();
    auto &vel = view.get<Velocity>(view.front());

    vel.x = 0.0f;
    vel.y = 0.0f;

    int x{}, y{};

    if(key(GLFW_KEY_W)) {
        y++;
    }
    if(key(GLFW_KEY_A)) {
        x--;
    }
    if(key(GLFW_KEY_S)) {
        y--;
    }
    if(key(GLFW_KEY_D)) {
        x++;
    }

    if(x && y) {
        vel.x = x * playerSpeed / 1.41421356f;
        vel.y = y * playerSpeed / 1.41421356f;
    } else {
        vel.x = x * playerSpeed;
        vel.y = y * playerSpeed;
    }
}

void GameScene::handleMovement(float deltaTime) {
    using namespace ecs::comp;

    auto view = _registry.view<Position, Velocity>();
    for(auto entity: view) {
        auto &pos = view.get<Position>(entity);
        auto &vel = view.get<Velocity>(entity);
        if(pos.x > 100.0f)
            pos.x = 100.0f;
        if(pos.x < -100.0f)
            pos.x = -100.0f;
        if(pos.y > 100.0f)
            pos.y = 100.0f;
        if(pos.y < -100.0f)
            pos.y = -100.0f;
        pos.x += vel.x * deltaTime;
        pos.y += vel.y * deltaTime;
    }
}

void GameScene::handleEnemies(float deltaTime) {
    using namespace ecs::comp;

    static double startTime = std::clock() / (double)CLOCKS_PER_SEC;

    auto view = _registry.view<Position, Velocity, AiWanderingControlled>();

    for(auto &entity: view) {
        auto &pos = view.get<Position>(entity);
        auto &vel = view.get<Velocity>(entity);
        auto &ai = view.get<AiWanderingControlled>(entity);

        float incrementX = (rand() % 200 - 100) / 10.0f;
        if(ai.dx + incrementX > 100.0f || ai.dx + incrementX < -100.0f)
            incrementX = -incrementX;
        float incrementY = (rand() % 200 - 100) / 10.0f;
        if(ai.dy + incrementY > 100.0f || ai.dy + incrementY < -100.0f)
            incrementY = -incrementY;

        ai.dx += incrementX;
        ai.dy += incrementY;

        float dx = ai.dx - pos.x;
        float dy = ai.dy - pos.y;

        float distance = sqrt(dx * dx + dy * dy);

        float unitX = dx / distance;
        float unitY = dy / distance;

        vel.x = unitX * 30.0f;
        vel.y = unitY * 30.0f;
    }
}

void GameScene::update(float deltaTime) {
    handleEnemies(deltaTime);
    handleMovement(deltaTime);

    using namespace std::chrono;
    static auto lastFireTick = high_resolution_clock::now();
    auto currentFireTick = high_resolution_clock::now();
    static const float fireSpreadChance = 0.3f; // Configurable chance for fire to spread

    GLubyte *ptr = _window->mapPBO();
    if(duration_cast<milliseconds>(currentFireTick - lastFireTick) > milliseconds(1000)) {
        lastFireTick = currentFireTick;
        if(ptr) {
            std::vector<GLubyte> newBuffer(200 * 200 * 4);
            std::copy(ptr, ptr + 200 * 200 * 4, newBuffer.begin());

            for(int y = 0; y < 200; ++y) {
                for(int x = 0; x < 200; ++x) {
                    int i = (y * 200 + x) * 4; // Calculate the index for pixel (x, y)
                    if(ptr[i] == 255 && ptr[i + 1] == 0 && ptr[i + 2] == 0) { // Check if the current pixel is on fire (red)
                        // Spread the fire to adjacent pixels if they are not already red and pass the spread chance check
                        if(x < 199 && static_cast<float>(rand()) / RAND_MAX < fireSpreadChance) { // Right
                            newBuffer[i + 4] = 255;     // R
                            newBuffer[i + 4 + 1] = 0;   // G
                            newBuffer[i + 4 + 2] = 0;   // B
                        }
                        if(x > 0 && static_cast<float>(rand()) / RAND_MAX < fireSpreadChance) { // Left
                            newBuffer[i - 4] = 255;     // R
                            newBuffer[i - 4 + 1] = 0;   // G
                            newBuffer[i - 4 + 2] = 0;   // B
                        }
                        if(y < 199 && static_cast<float>(rand()) / RAND_MAX < fireSpreadChance) { // Down
                            int indexDown = ((y + 1) * 200 + x) * 4;
                            newBuffer[indexDown] = 255;   // R
                            newBuffer[indexDown + 1] = 0; // G
                            newBuffer[indexDown + 2] = 0; // B
                        }
                        if(y > 0 && static_cast<float>(rand()) / RAND_MAX < fireSpreadChance) { // Up
                            int indexUp = ((y - 1) * 200 + x) * 4;
                            newBuffer[indexUp] = 255;   // R
                            newBuffer[indexUp + 1] = 0; // G
                            newBuffer[indexUp + 2] = 0; // B
                        }
                    }
                }
            }
            std::copy(newBuffer.begin(), newBuffer.end(), ptr);
        }
    }

    using namespace ecs::comp;
    auto view = _registry.view<Position, Velocity>();
    for (auto entity : view) {
        auto &pos = view.get<Position>(entity);
        // Convert coordinates from [-100, 100] to [0, 199]
        int x = static_cast<int>((pos.x + 100.0f) * 0.995f);
        int y = static_cast<int>((pos.y + 100.0f) * 0.995f);
        for (int i = x - 5; i <= x + 5; ++i) {
            for (int j = y - 5; j <= y + 5; ++j) {
                // Check if (i, j) is within bounds
                if (i >= 0 && i < 200 && j >= 0 && j < 200) {
                    int index = (j * 200 + i) * 4;
                    ptr[index] = 90;
                    ptr[index + 1] = 255;
                    ptr[index + 2] = 90;
                }
            }
        }
    }
    _window->unmapPBO();
}

void GameScene::draw(float deltaTime) {
    using namespace ecs::comp;

    _window->updateTextureFromPBO();

    _window->drawBackground();

    auto staticView = _registry.view<Position, Renderable>(entt::exclude<Velocity>);
    for(auto entity: staticView) {
        auto &pos = staticView.get<Position>(entity);
        auto &renderable = staticView.get<Renderable>(entity);

        _window->drawTexture(pos.x / gridMultiplier, pos.y / gridMultiplier, renderable.size / gridMultiplier, renderable.texture);
    }

    auto view = _registry.view<Position, Velocity, Renderable>();
    for(auto entity: view) {
        auto &pos = view.get<Position>(entity);
        auto &renderable = view.get<Renderable>(entity);
        auto &vel = view.get<Velocity>(entity);

        float x = pos.x + (vel.x * deltaTime);
        float y = pos.y + (vel.y * deltaTime);

        _window->drawTexture(x / gridMultiplier, y / gridMultiplier, renderable.size / gridMultiplier, renderable.texture);
    }
}

const int FPS = 60;
const int frameDelay = 1000 / FPS;

const float timeStep = 1.0f / 60.0f;

void GameScene::runLoop() {
    using namespace std::chrono;

    static auto lastFrameTime = high_resolution_clock::now();
    static float accumulator = 0.0f;

    auto currentFrameTime = high_resolution_clock::now();
    auto lastFrameDuration = duration_cast<milliseconds>(currentFrameTime - lastFrameTime);

    if(lastFrameDuration < milliseconds(frameDelay))
        std::this_thread::sleep_for(milliseconds(frameDelay) - lastFrameDuration);

    currentFrameTime = high_resolution_clock::now();
    duration<float> frameDuration = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    float deltaTime = frameDuration.count();
    accumulator += deltaTime;

    processInput();

    while(accumulator >= timeStep) {
        update(timeStep);
        accumulator -= timeStep;
    }

    float alpha = accumulator * timeStep;
    draw(alpha);
}
