#include <utils/PathUtils.hpp>
#include <windows.h>

const char *PathUtils::_exePath = nullptr;
const char *PathUtils::_exeDir = nullptr;

auto PathUtils::exePath() -> const char * {
    if(!_exePath) {
        char path[MAX_PATH];
        GetModuleFileName(nullptr, path, MAX_PATH);
        _exePath = path;
    }

    return _exePath;
}

auto PathUtils::exeDir() -> const char * {
    if(!_exeDir) {
        static char dir[MAX_PATH];
        if(!_exePath) {
            exePath();
        }

        strcpy(dir, _exePath);
        char *lastSlash = strrchr(dir, '\\');
        if(lastSlash) {
            *(lastSlash+1) = '\0';
        }
        _exeDir = dir;
    }
    return _exeDir;
}

auto PathUtils::absolutePath(const char *path) -> const char * {
    if(path[1] == ':')
        return path;

    static char relative[MAX_PATH];
    strcpy(relative, path);
    for(char &c: relative)
        if(c == '/')
            c = '\\';

    char *abs = new char[MAX_PATH];
    if(relative[0] == '\\') {
        strcpy(abs, exeDir());
        strcat(abs, relative+1);
    } else {
        strcpy(abs, exeDir());
        strcat(abs, relative);
    }
    return abs;
}
