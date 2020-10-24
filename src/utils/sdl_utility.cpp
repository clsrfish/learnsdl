#include <SDL2_image/SDL_image.h>
#include "./sdl_utility.hpp"
#include "./log.hpp"

void LogSDLError(const std::string &msg)
{
    LOG_E(msg, SDL_GetError());
}

SDL_Texture *LoadSDLTexture(const std::string &file, SDL_Renderer *ren)
{
    SDL_Texture *tex = IMG_LoadTexture(ren, file.c_str());
    if (tex == nullptr)
    {
        LogSDLError("Could not load texture");
    }
    return tex;
}

void RenderSDLTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
    int w, h;
    SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
    RenderSDLTexture(tex, ren, x, y, w, h);
}

void RenderSDLTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(ren, tex, nullptr, &dst);
}