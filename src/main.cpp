#include <spdlog/spdlog.h>

#include "engine/core/Logger.hpp"
#include "game/Game.hpp"

int main() {
    initSpdlog();

    Game game;
    int success = game.init();

    if (success != 0) {
        spdlog::critical("Game exited with code {}", success);
    }

    return success;
}
