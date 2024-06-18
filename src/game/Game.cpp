#include "game/Game.hpp"

#include <spdlog/spdlog.h>
#include <chrono>

#include "utils/PathUtils.hpp"
#include "engine/rendering/RenderWindow.hpp"
#include "engine/rendering/Shader.hpp"
#include "game/GameConfig.hpp"

Game::Game() = default;

Game::~Game() = default;

int Game::init() {
    conf::init();

    _window.init(conf::windowWidth.getValue(), conf::windowHeight.getValue(), "Savin Amazon Rainforest", {});
    GLuint vertexShader = createShaderFromFile(PathUtils::absolutePath("/assets/shaders/main.vert"), GL_VERTEX_SHADER);
    GLuint fragmentShader = createShaderFromFile(PathUtils::absolutePath("/assets/shaders/main.frag"), GL_FRAGMENT_SHADER);

    _window.setShaderProgram(vertexShader, fragmentShader);

    run();

    _window.discard();

    return 0;
} 

void Game::run() {
    using namespace std::chrono;

    _running = true;

    _currentScene = new GameScene(&_window);
    _currentScene->init();

    const int FPS = 120;
    const duration<double, std::milli> frameTime(1000.0 / FPS);

    while(_running && !_window.shouldClose()) {
        auto frameStart = high_resolution_clock::now();

        _window.clear();
        _currentScene->runLoop();
        _window.render();

        auto frameEnd = high_resolution_clock::now();
        auto frameDuration = frameEnd - frameStart;

        if (frameDuration < frameTime) {
            std::this_thread::sleep_for(frameTime - frameDuration);
        }
    }
}
