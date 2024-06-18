#include "engine/rendering/Shader.hpp"

#include <glad/gl.h>
#include <spdlog/spdlog.h>

#include <fstream>
#include <iostream>

const char *readShaderFromFile(const char *path) {
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        spdlog::error("Failed to open file: {}", path);
        return nullptr;
    }

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    char *buffer = new char[size + 1];
    if (!file.read(buffer, size)) {
        spdlog::error("Failed to read file: {}", path);
        delete[] buffer;
        return nullptr;
    }

    buffer[size] = '\0';

    return buffer;
}

GLuint createShader(const char *source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        spdlog::error("Failed to compile shader: {}", infoLog);
        return 0;
    }
    return shader;
}

GLuint createShaderFromFile(const char *path, GLenum type) {
    const char *source = readShaderFromFile(path);
    if (!source) {
        spdlog::error("Failed to read shader from file: {}", path);
        return 0;
    }

    GLuint shader = createShader(source, type);
    delete[] source;

    return shader;
}
