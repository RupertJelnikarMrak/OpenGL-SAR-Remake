#pragma once

class PathUtils
{
    static const char *_exePath;;
    static const char *_exeDir;
public:
    PathUtils() = delete;

    static auto exePath() -> const char *;
    static auto exeDir() -> const char *;
    static auto absolutePath(const char *path) -> const char *;
};
