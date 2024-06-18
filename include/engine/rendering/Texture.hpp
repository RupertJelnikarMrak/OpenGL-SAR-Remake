#pragma once

#include <stb/stb_image.h>

struct Texture {
    unsigned int id;
    int width;
    int height;
    int nrChannels;
    unsigned char *data;
};

struct Texture createTextureFromFile(const char *path);
