#include "engine/rendering/Texture.hpp"

#include <stb/stb_image.h>
#include <spdlog/spdlog.h>

struct Texture createTextureFromFile(const char *path) {
    struct Texture texture;
    texture.data = stbi_load(path, &texture.width, &texture.height, &texture.nrChannels, 0);
    if(!texture.data) {
        texture.id = 0;
        texture.width = 0;
        texture.height = 0;
        texture.nrChannels = 0;
        spdlog::error("Failed to load texture from file '{}'", path);
        return texture;
    }

    return texture;
}
