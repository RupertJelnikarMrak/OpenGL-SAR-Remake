#pragma once

#include <vector>

class PerlinNoise {
public:
    PerlinNoise(unsigned int seed = 0);
    double noise(double x, double y, double z);

private:
    std::vector<int> p;
    double fade(double t);
    double lerp(double t, double a, double b);
    double grad(int hash, double x, double y, double z);
};
