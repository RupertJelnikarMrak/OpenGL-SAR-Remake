#pragma once

#include <glad/gl.h>

const char *readShaderFromFile(const char *path);
GLuint createShader(const char *source, GLenum type);
GLuint createShaderFromFile(const char *path, GLenum type);
