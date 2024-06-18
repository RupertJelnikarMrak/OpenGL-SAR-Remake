#pragma once

#include <initializer_list>
#include <utility>
#include <vector>

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include "engine/rendering/Texture.hpp"

class RenderWindow {
private:
    GLFWwindow *_window;
    GLuint _shaderProgram;
    GLuint _backgroundTexture;
    int _width, _height;
    GLuint _pbo;


    std::vector<GLuint> _loadedTextures;

public:
    RenderWindow();
    ~RenderWindow();

    int init(int width, int height, const char *title, std::initializer_list<std::pair<int, int>> hints);
    void close();
    void discard();

    bool shouldClose();

    void setKeyCallback(void (*function)(GLFWwindow *, int, int, int, int));
    void setMouseButtonCallback(void (*function)(GLFWwindow *, int, int, int));
    int getKey(int key);

    void setShaderProgram(GLuint vertexShader, GLuint fragmentShader);

    void clear();
    void render();

    void loadTexture(struct Texture &texture);
    void drawTexture(float x, float y, float width, float height, struct Texture texture, bool cleanup = true);
    void drawTexture(float x, float y, float scale, struct Texture texture, bool cleanup = true);

    void createBackgroundTextureBuffer(int width, int height);
    GLubyte *mapPBO();
    void unmapPBO();
    void updateTextureFromPBO();
    void drawBackground();
};
