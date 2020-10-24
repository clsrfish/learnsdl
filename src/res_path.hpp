#if !defined(RES_PATH_H)
#define RES_PATH_H

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include "./utils/log.hpp"

std::string GetResourcePath(const std::string &subDir = "")
{
#ifdef _WIN32
    const char PATH_SEP = '\\';
#else
    const char PATH_SEP = '/';
#endif
    static std::string baseRes = "";
    if (baseRes.empty())
    {
        char *basePath = SDL_GetBasePath();
        if (basePath != nullptr)
        {
            baseRes = basePath;
            SDL_free(basePath);
        }
        else
        {
            LOG_E("Error getting resource path: %s", SDL_GetError());
            return "";
        }
        size_t pos = baseRes.rfind("build");
        baseRes = baseRes.substr(0, pos) + "assets" + PATH_SEP;
    }

    return subDir.empty() ? baseRes : baseRes + subDir + PATH_SEP;
}

#endif // RES_PATH_H
