#pragma once

#include "engine/rendering/Texture.hpp"

namespace ecs::comp {
struct Position {
    float x;
    float y;
};

struct Velocity {
    float x;
    float y;
};

struct Renderable {
    struct Texture texture;
    float size;
};

struct Hitbox {
    float w;
    float h;
};

struct PlayerControlled {
    int upKey;
    int downKey;
    int leftKey;
    int rightKey;
};

struct AiWanderingControlled {
    float dx;
    float dy;
};

} // namespace ecs::comp
