#include <engine/rendering/RenderWindow.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include <stb/stb_image.h>

#include <GLFW/glfw3.h>

#include "engine/rendering/Texture.hpp"

RenderWindow::RenderWindow()
    : _window(nullptr), _shaderProgram(0), _width(0), _height(0) {
}

RenderWindow::~RenderWindow() {
    discard();
}

void RenderWindow::close() {
    glfwSetWindowShouldClose(_window, true);
}

void RenderWindow::discard() {
    if(!_window)
        return;

    spdlog::debug("Discarding RenderWindow");
    for(auto textureId: _loadedTextures)
        glDeleteTextures(1, &textureId);

    glDeleteProgram(_shaderProgram);

    glfwDestroyWindow(_window);
    _window = nullptr;
    glfwTerminate();
}

int initializeGlfw() {
    static bool initialized = false;
    if(!initialized) {
        spdlog::debug("Initializing glfw3");
        if(!glfwInit()) {
            spdlog::error("Failed to initialize glfw3");
            return -1;
        }
        spdlog::debug("glfw3 initialized successfully");
        initialized = true;
    }
    return 0;
}

int RenderWindow::init(int width, int height, const char *title, std::initializer_list<std::pair<int, int>> hints) {
    initializeGlfw();

    for(auto hint: hints)
        glfwWindowHint(hint.first, hint.second);

    spdlog::debug("Creating window with dimensions {}x{} and title '{}'", width, height, title);
    _window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if(!_window) {
        spdlog::error("Failed to open glfw3 window");
        glfwTerminate();
        return -1;
    }
    spdlog::debug("Window created successfully");

    glfwMakeContextCurrent(_window);

    spdlog::debug("Initializing OpenGL context");
    if(!gladLoadGL(glfwGetProcAddress)) {
        spdlog::error("Failed to initialize OpenGL context");
        glfwTerminate();
        return -1;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    spdlog::debug("OpenGL context initialized successfully");

    return 0;
}

void RenderWindow::setKeyCallback(void (*function)(GLFWwindow *, int, int, int, int)) {
    glfwSetKeyCallback(_window, function);
}

void RenderWindow::setMouseButtonCallback(void (*function)(GLFWwindow *, int, int, int)) {
    glfwSetMouseButtonCallback(_window, function);
}

int RenderWindow::getKey(int key) {
    return glfwGetKey(_window, key);
}

bool RenderWindow::shouldClose() {
    return glfwWindowShouldClose(_window);
}

void RenderWindow::setShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
    _shaderProgram = glCreateProgram();
    glAttachShader(_shaderProgram, vertexShader);
    glAttachShader(_shaderProgram, fragmentShader);
    glLinkProgram(_shaderProgram);

    GLint success;
    glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        GLchar infoLog[1024];
        glGetProgramInfoLog(_shaderProgram, 1024, nullptr, infoLog);
        spdlog::error("Failed to link shader program: {}", infoLog);
    }
}

void RenderWindow::clear() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderWindow::render() {
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

void RenderWindow::loadTexture(struct Texture &texture) {
    if(!texture.data) {
        spdlog::error("Failed to load texture, no texture data provided");
    }

    glGenTextures(1, &texture.id);
    _loadedTextures.push_back(texture.id);

    glBindTexture(GL_TEXTURE_2D, texture.id);

    GLenum format = GL_RGB;
    if (texture.nrChannels == 1)
        format = GL_RED;
    else if (texture.nrChannels == 3)
        format = GL_RGB;
    else if (texture.nrChannels == 4) {
        format = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, texture.width, texture.height, 0, format, GL_UNSIGNED_BYTE, texture.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderWindow::drawTexture(float x, float y, float width, float height, struct Texture texture, bool cleanup) {
    // clang-format off
    // Vertex data
    float vertices[] = {
        // positions                // texture coords
        x + width/2, y - height/2,  1.0f, 1.0f, // top right
        x + width/2, y + height/2,  1.0f, 0.0f, // bottom right
        x - width/2, y + height/2,  0.0f, 0.0f, // bottom left
        x - width/2, y - height/2,  0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    // clang-format on

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)nullptr);
    glEnableVertexAttribArray(0);
    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO
    glBindVertexArray(0);

    glUseProgram(_shaderProgram);

    glBindTexture(GL_TEXTURE_2D, texture.id);

    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    if(cleanup) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
}

void RenderWindow::drawTexture(float x, float y, float scale, struct Texture texture, bool cleanup) {
    float width, height;
    float aspectRatio = (float)texture.width / (float)texture.height;
    float windowAspectRatio = (float)_width / (float)_height;
    width = scale;
    height = scale / aspectRatio * windowAspectRatio;
    drawTexture(x, y, width, height, texture, cleanup);
}

void RenderWindow::createBackgroundTextureBuffer(int width, int height){
    _width = width;
    _height = height;
    glGenTextures(1, &_backgroundTexture);
    glBindTexture(GL_TEXTURE_2D, _backgroundTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenBuffers(1, &_pbo);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, _pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * 4, nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

GLubyte *RenderWindow::mapPBO() {
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, _pbo);
    GLubyte *ptr = (GLubyte *)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    return ptr;
}

void RenderWindow::unmapPBO() {
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, _pbo);
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

void RenderWindow::updateTextureFromPBO() {
    glBindTexture(GL_TEXTURE_2D, _backgroundTexture);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, _pbo);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderWindow::drawBackground() {
    // clang-format off
    float quadVertices[] = {
        // positions   // texture coords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    // clang-format on

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(_shaderProgram);
    glBindTexture(GL_TEXTURE_2D, _backgroundTexture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
}
